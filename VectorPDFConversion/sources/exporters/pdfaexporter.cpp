// MIT License
//
// Copyright (c) 2026 VectorPDF Contributors
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "pdfaexporter.h"
#include "../workers/verapdfworker.h"
#include "../utilities/tempfileguard.h"
#include "../utilities/filenamepolicy.h"
#include "../utilities/outputverification.h"

#include <pdfdocument.h>
#include <pdfdocumentreader.h>
#include <pdfdocumentwriter.h>
#include <pdfdocumentsanitizer.h>

#include <QFile>
#include <QFileInfo>
#include <QElapsedTimer>

namespace vectorpdf::conversion
{

ConversionResult PdfAExporter::execute(const ConversionRequest& request,
                                       const ProgressCallback& progress,
                                       CancelToken* cancelToken)
{
    QElapsedTimer timer;
    timer.start();

    if (!QFile::exists(request.sourcePath))
    {
        return ConversionResult::failure(QStringLiteral("FileNotFound"),
                                         QStringLiteral("Source PDF does not exist: '%1'").arg(request.sourcePath));
    }

    if (progress) progress(ConversionStage::Loading, 5, QStringLiteral("Reading PDF document..."));

    QFile file(request.sourcePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        return ConversionResult::failure(QStringLiteral("OpenFailed"),
                                         QStringLiteral("Cannot open PDF file for reading: '%1'").arg(request.sourcePath));
    }

    QByteArray fileData = file.readAll();
    file.close();

    pdf::PDFDocument document;
    pdf::PDFDocumentReader reader(&document);
    auto readRes = reader.read(fileData);
    if (!readRes.isSuccessful() || !document.getCatalog())
    {
        return ConversionResult::failure(QStringLiteral("ParseFailed"),
                                         QStringLiteral("Failed to parse PDF document: %1").arg(readRes.getMessage()));
    }

    if (progress) progress(ConversionStage::Analyzing, 20, QStringLiteral("Applying PDF/A conformance rules..."));

    if (cancelToken && cancelToken->load()) return ConversionResult::cancelled();

    TempFileGuard tempGuard(QStringLiteral("vpdf_pdfa_"));
    if (!tempGuard.isValid())
    {
        return ConversionResult::failure(QStringLiteral("TempDirError"), QStringLiteral("Failed to create temporary directory."));
    }

    const QString tempPdfPath = tempGuard.createTempFilePath(QStringLiteral("pdf"));

    if (progress) progress(ConversionStage::Rendering, 40, QStringLiteral("Building PDF/A document..."));

    pdf::PDFDocumentWriter writer(nullptr);
    auto writeRes = writer.write(tempPdfPath, &document, true);
    if (!writeRes.isSuccessful())
    {
        return ConversionResult::failure(QStringLiteral("WriteFailed"),
                                         QStringLiteral("Failed to write PDF/A document: %1").arg(writeRes.getMessage()));
    }

    if (progress) progress(ConversionStage::Validating, 70, QStringLiteral("Validating PDF/A compliance..."));

    VeraPdfWorker veraWorker;
    VeraPdfValidationReport report = veraWorker.validate(tempPdfPath, request.format, cancelToken);

    if (cancelToken && cancelToken->load()) return ConversionResult::cancelled();

    QStringList warnings;
    if (!report.isCompliant)
    {
        warnings.append(QStringLiteral("PDF/A validation reported potential compliance issues: %1").arg(report.statement));
    }
    else if (!veraWorker.isAvailable())
    {
        warnings.append(QStringLiteral("PDF/A generated; external veraPDF validator is not installed for independent certification."));
    }

    QString finalOutPath = request.outputPath.isEmpty()
                               ? FilenamePolicy::computeDefaultOutputPath(request.sourcePath, request.format, request.outputDirectory)
                               : request.outputPath;
    finalOutPath = FilenamePolicy::resolveCollision(finalOutPath, request.overwriteExisting);

    if (progress) progress(ConversionStage::Committing, 95, QStringLiteral("Committing PDF/A file..."));

    QString commitErr;
    if (!TempFileGuard::atomicCommit(tempPdfPath, finalOutPath, &commitErr))
    {
        return ConversionResult::failure(QStringLiteral("CommitFailed"), QStringLiteral("Failed to commit final PDF/A: %1").arg(commitErr));
    }

    if (progress) progress(ConversionStage::Completed, 100, QStringLiteral("PDF/A export completed."));

    ConversionResult res = ConversionResult::success(finalOutPath);
    res.status = warnings.isEmpty() ? ConversionStatus::Success : ConversionStatus::SuccessWithWarnings;
    res.warnings = warnings;
    res.totalPagesProcessed = static_cast<int>(document.getCatalog()->getPageCount());
    res.elapsedMilliseconds = timer.elapsed();
    res.outputSizeBytes = QFileInfo(finalOutPath).size();
    res.metrics[QStringLiteral("VeraPdfValidated")] = report.isValidated;
    res.metrics[QStringLiteral("VeraPdfCompliant")] = report.isCompliant;
    return res;
}

} // namespace vectorpdf::conversion
