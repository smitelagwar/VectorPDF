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

#include "pdfdocxexporter.h"
#include "pdfimageexporter.h"
#include "../office/docxpackagewriter.h"
#include "../office/tabledetector.h"
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

ConversionResult PdfDocxExporter::execute(const ConversionRequest& request,
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

    TempFileGuard tempGuard(QStringLiteral("vpdf_docx_"));
    if (!tempGuard.isValid())
    {
        return ConversionResult::failure(QStringLiteral("TempDirError"), QStringLiteral("Failed to create temporary directory."));
    }

    const QString tempDocxPath = tempGuard.createTempFilePath(QStringLiteral("docx"));
    DocxPackageWriter docxWriter;

    const int pageCount = pagesToProcess.size();

    if (request.docxMode == DocxExportMode::Visual)
    {
        // Visual Fidelity Mode
        for (int i = 0; i < pageCount; ++i)
        {
            if (cancelToken && cancelToken->load()) return ConversionResult::cancelled();

            int pageIdx = pagesToProcess[i];
            const pdf::PDFPage* page = document.getCatalog()->getPage(static_cast<pdf::PDFInteger>(pageIdx));
            QSizeF pagePt = page ? page->getRotatedMediaBox().size() : QSizeF(595, 842);

            QImage pageImg = PdfImageExporter::renderPageToImage(&document, pageIdx, request.dpi > 0 ? request.dpi : 200);
            if (!pageImg.isNull())
            {
                docxWriter.addFullPageImage(pageImg, pagePt);
            }

            if (i < pageCount - 1)
            {
                docxWriter.addPageBreak();
            }

            if (progress)
            {
                int p = 10 + ((i + 1) * 70) / pageCount;
                progress(ConversionStage::Rendering, p, QStringLiteral("Exported page %1 of %2 (Visual Mode)").arg(i + 1).arg(pageCount));
            }
        }
    }
    else
    {
        // Reflow Editable Mode
        pdf::PDFDocumentTextFlowFactory textFlowFactory;
        textFlowFactory.setCalculateBoundingBoxes(true);

        for (int i = 0; i < pageCount; ++i)
        {
            if (cancelToken && cancelToken->load()) return ConversionResult::cancelled();

            int pageIdx = pagesToProcess[i];

            // 1. Check for tables on this page
            QList<DetectedTable> tables = TableDetector::detectTables(&document, pageIdx);

            // 2. Extract text flow for this page
            std::vector<pdf::PDFInteger> singlePage = { static_cast<pdf::PDFInteger>(pageIdx) };
            pdf::PDFDocumentTextFlow flow = textFlowFactory.create(&document, singlePage, pdf::PDFDocumentTextFlowFactory::Algorithm::Layout);

            QString currentParagraphText;
            for (const pdf::PDFDocumentTextFlow::Item& item : flow.getItems())
            {
                if (item.isText() && !item.text.trimmed().isEmpty())
                {
                    // Check if part of a detected table
                    bool inTable = false;
                    for (const DetectedTable& t : tables)
                    {
                        if (t.boundingBox.contains(item.boundingRect.center()))
                        {
                            inTable = true;
                            break;
                        }
                    }

                    if (!inTable)
                    {
                        if (!currentParagraphText.isEmpty())
                        {
                            currentParagraphText.append(QLatin1Char(' '));
                        }
                        currentParagraphText.append(item.text.trimmed());

                        // Simple line/paragraph break heuristic
                        if (item.text.endsWith(QLatin1Char('.')) || item.text.endsWith(QLatin1Char(':')) || item.boundingRect.height() > 18.0)
                        {
                            bool isHeading = item.boundingRect.height() > 14.0;
                            int headingLvl = item.boundingRect.height() > 18.0 ? 1 : (isHeading ? 2 : 0);
                            docxWriter.addSimpleParagraph(currentParagraphText, isHeading, headingLvl);
                            currentParagraphText.clear();
                        }
                    }
                }
            }

            if (!currentParagraphText.isEmpty())
            {
                docxWriter.addSimpleParagraph(currentParagraphText);
                currentParagraphText.clear();
            }

            // Write detected tables
            for (const DetectedTable& t : tables)
            {
                DocxTable docxTable;
                for (int r = 0; r < t.rowCount; ++r)
                {
                    DocxTableRow docxRow;
                    docxRow.isHeader = (r == 0);
                    for (int c = 0; c < t.colCount; ++c)
                    {
                        DocxTableCell docxCell;
                        docxCell.text = t.getCellText(r, c);
                        docxCell.bold = (r == 0);
                        docxRow.cells.append(docxCell);
                    }
                    docxTable.rows.append(docxRow);
                }
                docxWriter.addTable(docxTable);
            }

            if (i < pageCount - 1)
            {
                docxWriter.addPageBreak();
            }

            if (progress)
            {
                int p = 10 + ((i + 1) * 70) / pageCount;
                progress(ConversionStage::Analyzing, p, QStringLiteral("Processed page %1 of %2 (Reflow Mode)").arg(i + 1).arg(pageCount));
            }
        }
    }

    if (progress) progress(ConversionStage::Encoding, 85, QStringLiteral("Packaging DOCX archive..."));

    QString saveErr;
    if (!docxWriter.save(tempDocxPath, &saveErr))
    {
        return ConversionResult::failure(QStringLiteral("DocxWriteFailed"), QStringLiteral("Failed to write DOCX archive: %1").arg(saveErr));
    }

    if (progress) progress(ConversionStage::Validating, 90, QStringLiteral("Validating DOCX package..."));

    QString verifyErr;
    if (!OutputVerification::verifyDocx(tempDocxPath, &verifyErr))
    {
        return ConversionResult::failure(QStringLiteral("ValidationFailed"), QStringLiteral("DOCX verification failed: %1").arg(verifyErr));
    }

    QString finalOutPath = request.outputPath.isEmpty()
                               ? FilenamePolicy::computeDefaultOutputPath(request.sourcePath, ConversionFormat::Docx, request.outputDirectory)
                               : request.outputPath;
    finalOutPath = FilenamePolicy::resolveCollision(finalOutPath, request.overwriteExisting);

    if (progress) progress(ConversionStage::Committing, 95, QStringLiteral("Committing DOCX file..."));

    QString commitErr;
    if (!TempFileGuard::atomicCommit(tempDocxPath, finalOutPath, &commitErr))
    {
        return ConversionResult::failure(QStringLiteral("CommitFailed"), QStringLiteral("Failed to commit final DOCX: %1").arg(commitErr));
    }

    if (progress) progress(ConversionStage::Completed, 100, QStringLiteral("DOCX export completed."));

    ConversionResult res = ConversionResult::success(finalOutPath);
    res.totalPagesProcessed = pageCount;
    res.elapsedMilliseconds = timer.elapsed();
    res.outputSizeBytes = QFileInfo(finalOutPath).size();
    return res;
}

} // namespace vectorpdf::conversion
