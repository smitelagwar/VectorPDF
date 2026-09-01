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

#include "monochromepdfexporter.h"
#include "pdfimageexporter.h"
#include "../utilities/imageprocessing.h"
#include "../utilities/tempfileguard.h"
#include "../utilities/filenamepolicy.h"
#include "../utilities/pagerangeparser.h"
#include "../utilities/outputverification.h"

#include <pdfdocument.h>
#include <pdfdocumentreader.h>
#include <pdfdocumenttextflow.h>

#include <QPdfWriter>
#include <QPainter>
#include <QPageSize>
#include <QPageLayout>
#include <QFile>
#include <QFileInfo>
#include <QElapsedTimer>

namespace vectorpdf::conversion
{

ConversionResult MonochromePdfExporter::execute(const ConversionRequest& request,
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

    if (progress) progress(ConversionStage::Loading, 5, QStringLiteral("Reading source PDF document..."));

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

    TempFileGuard tempGuard(QStringLiteral("vpdf_mono_"));
    if (!tempGuard.isValid())
    {
        return ConversionResult::failure(QStringLiteral("TempDirError"), QStringLiteral("Failed to create temporary directory."));
    }

    const QString tempPdfPath = tempGuard.createTempFilePath(QStringLiteral("pdf"));
    const int targetDpi = request.dpi > 0 ? request.dpi : 300;

    QPdfWriter pdfWriter(tempPdfPath);
    pdfWriter.setResolution(targetDpi);
    pdfWriter.setPageMargins(QMarginsF(0, 0, 0, 0), QPageLayout::Point);

    QPainter painter;
    const int pageCount = pagesToProcess.size();

    pdf::PDFDocumentTextFlowFactory textFlowFactory;
    textFlowFactory.setCalculateBoundingBoxes(true);

    for (int i = 0; i < pageCount; ++i)
    {
        if (cancelToken && cancelToken->load()) return ConversionResult::cancelled();

        int pageIdx = pagesToProcess[i];
        const pdf::PDFPage* page = document.getCatalog()->getPage(static_cast<pdf::PDFInteger>(pageIdx));
        QRectF mediaBox = page ? page->getRotatedMediaBox() : QRectF(0, 0, 595, 842);

        QSizeF pageSizePoints = mediaBox.size();
        QPageSize pageSize(pageSizePoints, QPageSize::Point);
        pdfWriter.setPageSize(pageSize);

        if (i == 0)
        {
            if (!painter.begin(&pdfWriter))
            {
                return ConversionResult::failure(QStringLiteral("PainterFailed"), QStringLiteral("Failed to begin PDF writer painter."));
            }
        }
        else
        {
            pdfWriter.newPage();
        }

        // 1. Render page to high-res grayscale image
        QImage pageImg = PdfImageExporter::renderPageToImage(&document, pageIdx, targetDpi, false, true);

        // 2. Convert to 1-bit monochrome using Otsu / Fixed thresholding
        QImage monoImg = ImageProcessing::toMonochrome(pageImg, request.monochromeMethod, request.fixedThreshold);

        // 3. Draw monochrome raster onto PDF page (scaled to exact point dimensions * targetDpi / 72.0)
        QRect paintRect(0, 0, qRound(pageSizePoints.width() * targetDpi / 72.0),
                              qRound(pageSizePoints.height() * targetDpi / 72.0));
        painter.drawImage(paintRect, monoImg);

        // 4. If searchable text is requested, extract and draw invisible text layer
        if (request.preserveSearchableText)
        {
            std::vector<pdf::PDFInteger> singlePage = { static_cast<pdf::PDFInteger>(pageIdx) };
            pdf::PDFDocumentTextFlow flow = textFlowFactory.create(&document, singlePage, pdf::PDFDocumentTextFlowFactory::Algorithm::Layout);

            painter.save();
            painter.setPen(Qt::transparent); // Invisible text
            for (const pdf::PDFDocumentTextFlow::Item& item : flow.getItems())
            {
                if (item.isText() && !item.text.trimmed().isEmpty())
                {
                    qreal x = item.boundingRect.left() * targetDpi / 72.0;
                    qreal y = item.boundingRect.bottom() * targetDpi / 72.0;
                    qreal fontSize = item.boundingRect.height() * targetDpi / 72.0;
                    QFont f(QStringLiteral("Segoe UI"));
                    f.setPixelSize(qMax(1, qRound(fontSize)));
                    painter.setFont(f);
                    painter.drawText(QPointF(x, y), item.text);
                }
            }
            painter.restore();
        }

        if (progress)
        {
            int p = 10 + ((i + 1) * 75) / pageCount;
            progress(ConversionStage::Rendering, p, QStringLiteral("Binarized page %1 of %2").arg(i + 1).arg(pageCount));
        }
    }

    if (painter.isActive())
    {
        painter.end();
    }

    if (progress) progress(ConversionStage::Validating, 90, QStringLiteral("Validating monochrome PDF..."));

    QString verifyErr;
    if (!OutputVerification::verifyPdf(tempPdfPath, pageCount, &verifyErr))
    {
        return ConversionResult::failure(QStringLiteral("ValidationFailed"), QStringLiteral("Monochrome PDF validation failed: %1").arg(verifyErr));
    }

    QString finalOutPath = request.outputPath.isEmpty()
                               ? FilenamePolicy::computeDefaultOutputPath(request.sourcePath, ConversionFormat::MonochromePdf, request.outputDirectory)
                               : request.outputPath;
    finalOutPath = FilenamePolicy::resolveCollision(finalOutPath, request.overwriteExisting);

    if (progress) progress(ConversionStage::Committing, 95, QStringLiteral("Committing monochrome PDF..."));

    QString commitErr;
    if (!TempFileGuard::atomicCommit(tempPdfPath, finalOutPath, &commitErr))
    {
        return ConversionResult::failure(QStringLiteral("CommitFailed"), QStringLiteral("Failed to commit final PDF: %1").arg(commitErr));
    }

    if (progress) progress(ConversionStage::Completed, 100, QStringLiteral("Monochrome conversion completed."));

    ConversionResult res = ConversionResult::success(finalOutPath);
    res.totalPagesProcessed = pageCount;
    res.elapsedMilliseconds = timer.elapsed();
    res.outputSizeBytes = QFileInfo(finalOutPath).size();
    return res;
}

} // namespace vectorpdf::conversion
