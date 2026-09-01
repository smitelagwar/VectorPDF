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

#include "fdfexporter.h"
#include "../utilities/tempfileguard.h"
#include "../utilities/filenamepolicy.h"
#include "../utilities/outputverification.h"

#include <pdfdocument.h>
#include <pdfdocumentreader.h>
#include <pdfform.h>

#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QElapsedTimer>

namespace vectorpdf::conversion
{

namespace
{
QString fdfEscape(const QString& str)
{
    QString escaped;
    for (QChar c : str)
    {
        if (c == QLatin1Char('(') || c == QLatin1Char(')') || c == QLatin1Char('\\'))
        {
            escaped.append(QLatin1Char('\\'));
        }
        escaped.append(c);
    }
    return escaped;
}
} // namespace

ConversionResult FdfExporter::execute(const ConversionRequest& request,
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

    if (progress) progress(ConversionStage::Loading, 10, QStringLiteral("Reading PDF document and form fields..."));

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

    if (cancelToken && cancelToken->load()) return ConversionResult::cancelled();

    if (progress) progress(ConversionStage::Analyzing, 30, QStringLiteral("Extracting AcroForm data for FDF..."));

    TempFileGuard tempGuard(QStringLiteral("vpdf_fdf_"));
    if (!tempGuard.isValid())
    {
        return ConversionResult::failure(QStringLiteral("TempDirError"), QStringLiteral("Failed to create temporary directory."));
    }

    const QString tempFdfPath = tempGuard.createTempFilePath(QStringLiteral("fdf"));

    QFile fdfFile(tempFdfPath);
    if (!fdfFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return ConversionResult::failure(QStringLiteral("CreateFailed"), QStringLiteral("Cannot create temporary FDF file."));
    }

    QTextStream out(&fdfFile);
    out.setEncoding(QStringConverter::Utf8);

    out << "%FDF-1.2\n";
    out << "1 0 obj\n";
    out << "<<\n";
    out << "/FDF <<\n";
    out << "/Fields [\n";

    int fieldsExported = 0;
    const pdf::PDFForm* form = document.getCatalog()->getForm();
    if (form)
    {
        for (const auto& fieldPtr : form->getFields())
        {
            if (!fieldPtr) continue;

            const QString name = fieldPtr->getName(pdf::PDFFormField::NameType::FullyQualified);
            const QString val = fieldPtr->getStringValue();

            if (!name.isEmpty())
            {
                out << "<< /T (" << fdfEscape(name) << ") /V (" << fdfEscape(val) << ") >>\n";
                fieldsExported++;
            }
        }
    }

    out << "]\n";
    out << "/F (" << fdfEscape(QFileInfo(request.sourcePath).fileName()) << ")\n";
    out << ">>\n";
    out << ">>\n";
    out << "endobj\n";
    out << "trailer\n";
    out << "<<\n";
    out << "/Root 1 0 R\n";
    out << ">>\n";
    out << "%%EOF\n";

    fdfFile.close();

    if (progress) progress(ConversionStage::Validating, 85, QStringLiteral("Validating FDF output..."));

    QString verifyErr;
    if (!OutputVerification::verifyFdf(tempFdfPath, &verifyErr))
    {
        return ConversionResult::failure(QStringLiteral("ValidationFailed"), QStringLiteral("FDF validation failed: %1").arg(verifyErr));
    }

    QString finalOutPath = request.outputPath.isEmpty()
                               ? FilenamePolicy::computeDefaultOutputPath(request.sourcePath, ConversionFormat::Fdf, request.outputDirectory)
                               : request.outputPath;
    finalOutPath = FilenamePolicy::resolveCollision(finalOutPath, request.overwriteExisting);

    if (progress) progress(ConversionStage::Committing, 95, QStringLiteral("Committing FDF file..."));

    QString commitErr;
    if (!TempFileGuard::atomicCommit(tempFdfPath, finalOutPath, &commitErr))
    {
        return ConversionResult::failure(QStringLiteral("CommitFailed"), QStringLiteral("Failed to commit final FDF: %1").arg(commitErr));
    }

    if (progress) progress(ConversionStage::Completed, 100, QStringLiteral("FDF export completed."));

    ConversionResult res = ConversionResult::success(finalOutPath);
    res.metrics[QStringLiteral("FieldsExported")] = fieldsExported;
    res.elapsedMilliseconds = timer.elapsed();
    res.outputSizeBytes = QFileInfo(finalOutPath).size();
    return res;
}

} // namespace vectorpdf::conversion
