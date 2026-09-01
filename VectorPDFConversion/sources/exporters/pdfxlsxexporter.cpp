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

#include "pdfxlsxexporter.h"
#include "../office/tabledetector.h"
#include "../office/xlsxpackagewriter.h"
#include "../utilities/tempfileguard.h"
#include "../utilities/filenamepolicy.h"
#include "../utilities/pagerangeparser.h"
#include "../utilities/outputverification.h"

#include <pdfdocument.h>
#include <pdfdocumentreader.h>
#include <pdfdocumenttextflow.h>

#include <QFile>
#include <QFileInfo>
#include <QElapsedTimer>

namespace vectorpdf::conversion
{

ConversionResult PdfXlsxExporter::execute(const ConversionRequest& request,
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

    const int totalDocPages = static_cast<int>(document.getCatalog()->getPageCount());
    if (totalDocPages <= 0)
    {
        return ConversionResult::failure(QStringLiteral("EmptyDocument"), QStringLiteral("PDF document contains no pages."));
    }

    QList<int> pagesToProcess = request.pages;
    if (!request.pageRangeString.isEmpty())
    {
        pagesToProcess = PageRangeParser::parse(request.pageRangeString, totalDocPages);
    }
    if (pagesToProcess.isEmpty())
    {
        for (int i = 0; i < totalDocPages; ++i)
        {
            pagesToProcess.append(i);
        }
    }

    TempFileGuard tempGuard(QStringLiteral("vpdf_xlsx_"));
    if (!tempGuard.isValid())
    {
        return ConversionResult::failure(QStringLiteral("TempDirError"), QStringLiteral("Failed to create temporary directory."));
    }

    const QString tempXlsxPath = tempGuard.createTempFilePath(QStringLiteral("xlsx"));
    XlsxPackageWriter xlsxWriter;

    int totalTablesFound = 0;
    int currentRowInSheet = 1;
    const int pageCount = pagesToProcess.size();

    for (int i = 0; i < pageCount; ++i)
    {
        if (cancelToken && cancelToken->load()) return ConversionResult::cancelled();

        int pageIdx = pagesToProcess[i];
        QList<DetectedTable> tables = TableDetector::detectTables(&document, pageIdx);

        if (!tables.isEmpty())
        {
            for (const DetectedTable& table : tables)
            {
                totalTablesFound++;
                xlsxWriter.addDetectedTable(0, table, currentRowInSheet, 1);
                currentRowInSheet += table.rowCount + 2; // leave 1 empty row
            }
        }
        else if (request.xlsxMode == XlsxExportMode::PageLayout)
        {
            // Fallback: place text lines in rows
            pdf::PDFDocumentTextFlowFactory factory;
            std::vector<pdf::PDFInteger> singlePage = { static_cast<pdf::PDFInteger>(pageIdx) };
            pdf::PDFDocumentTextFlow flow = factory.create(&document, singlePage, pdf::PDFDocumentTextFlowFactory::Algorithm::Layout);

            for (const pdf::PDFDocumentTextFlow::Item& item : flow.getItems())
            {
                if (item.isText() && !item.text.trimmed().isEmpty())
                {
                    bool isNum = false;
                    double numVal = item.text.trimmed().toDouble(&isNum);
                    if (isNum)
                    {
                        xlsxWriter.setCell(0, currentRowInSheet, 1, numVal, false);
                    }
                    else
                    {
                        xlsxWriter.setCell(0, currentRowInSheet, 1, item.text.trimmed(), false);
                    }
                    currentRowInSheet++;
                }
            }
            currentRowInSheet += 2;
        }

        if (progress)
        {
            int p = 10 + ((i + 1) * 70) / pageCount;
            progress(ConversionStage::Analyzing, p, QStringLiteral("Analyzed tables on page %1 of %2").arg(i + 1).arg(pageCount));
        }
    }

    if (totalTablesFound == 0 && request.xlsxMode == XlsxExportMode::DetectedTables)
    {
        // No tables found: add informational row
        xlsxWriter.setCell(0, 1, 1, QStringLiteral("No structured tables were detected in the source document."), false);
    }

    if (progress) progress(ConversionStage::Encoding, 85, QStringLiteral("Packaging XLSX archive..."));

    QString saveErr;
    if (!xlsxWriter.save(tempXlsxPath, &saveErr))
    {
        return ConversionResult::failure(QStringLiteral("XlsxWriteFailed"), QStringLiteral("Failed to write XLSX archive: %1").arg(saveErr));
    }

    if (progress) progress(ConversionStage::Validating, 90, QStringLiteral("Validating XLSX package..."));

    QString verifyErr;
    if (!OutputVerification::verifyXlsx(tempXlsxPath, &verifyErr))
    {
        return ConversionResult::failure(QStringLiteral("ValidationFailed"), QStringLiteral("XLSX verification failed: %1").arg(verifyErr));
    }

    QString finalOutPath = request.outputPath.isEmpty()
                               ? FilenamePolicy::computeDefaultOutputPath(request.sourcePath, ConversionFormat::Xlsx, request.outputDirectory)
                               : request.outputPath;
    finalOutPath = FilenamePolicy::resolveCollision(finalOutPath, request.overwriteExisting);

    if (progress) progress(ConversionStage::Committing, 95, QStringLiteral("Committing XLSX file..."));

    QString commitErr;
    if (!TempFileGuard::atomicCommit(tempXlsxPath, finalOutPath, &commitErr))
    {
        return ConversionResult::failure(QStringLiteral("CommitFailed"), QStringLiteral("Failed to commit final XLSX: %1").arg(commitErr));
    }

    if (progress) progress(ConversionStage::Completed, 100, QStringLiteral("XLSX export completed."));

    ConversionResult res = ConversionResult::success(finalOutPath);
    res.totalPagesProcessed = pageCount;
    res.elapsedMilliseconds = timer.elapsed();
    res.outputSizeBytes = QFileInfo(finalOutPath).size();
    if (totalTablesFound == 0 && request.xlsxMode == XlsxExportMode::DetectedTables)
    {
        res.warnings.append(QStringLiteral("No structured tables were detected in the selected pages."));
    }
    return res;
}

} // namespace vectorpdf::conversion
