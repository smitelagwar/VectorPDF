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

#include "pdfimageexporter.h"
#include "../utilities/tempfileguard.h"
#include "../utilities/filenamepolicy.h"
#include "../utilities/pagerangeparser.h"
#include "../utilities/outputverification.h"
#include "../utilities/imageprocessing.h"

#include <pdfdocument.h>
#include <pdfdocumentreader.h>
#include <pdfrenderer.h>
#include <pdffont.h>
#include <pdfcms.h>
#include <pdfoptionalcontent.h>

#include <QImageWriter>
#include <QPainter>
#include <QFileInfo>
#include <QDir>
#include <QElapsedTimer>

namespace vectorpdf::conversion
{

QImage PdfImageExporter::renderPageToImage(const pdf::PDFDocument* document,
                                           int pageIndex,
                                           int dpi,
                                           bool transparentBackground,
                                           bool grayscale)
{
    if (!document || !document->getCatalog()) return QImage();

    const pdf::PDFPage* page = document->getCatalog()->getPage(static_cast<pdf::PDFInteger>(pageIndex));
    if (!page) return QImage();

    QRectF mediaBox = page->getRotatedMediaBox();
    if (mediaBox.width() <= 0 || mediaBox.height() <= 0) return QImage();

    int targetDpi = dpi > 0 ? dpi : 300;
    int width = qMax(1, qRound(mediaBox.width() * targetDpi / 72.0));
    int height = qMax(1, qRound(mediaBox.height() * targetDpi / 72.0));

    QImage image(width, height, QImage::Format_ARGB32_Premultiplied);
    image.fill(transparentBackground ? Qt::transparent : Qt::white);

    pdf::PDFOptionalContentActivity optionalContentActivity(document, pdf::OCUsage::Export, nullptr);
    pdf::PDFCMSManager cmsManager(nullptr);
    cmsManager.setDocument(const_cast<pdf::PDFDocument*>(document));
    pdf::PDFCMSPointer cms = cmsManager.getCurrentCMS();
    pdf::PDFMeshQualitySettings meshQualitySettings;
    pdf::PDFFontCache fontCache(pdf::DEFAULT_FONT_CACHE_LIMIT, pdf::DEFAULT_REALIZED_FONT_CACHE_LIMIT);
    pdf::PDFModifiedDocument md(const_cast<pdf::PDFDocument*>(document), &optionalContentActivity);
    fontCache.setDocument(md);

    pdf::PDFRenderer::Features features = pdf::PDFRenderer::getDefaultFeatures();
    if (grayscale)
    {
        features |= pdf::PDFRenderer::ColorAdjust_Grayscale;
    }

    pdf::PDFRenderer renderer(document, &fontCache, cms.data(), &optionalContentActivity, features, meshQualitySettings);

    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    QRectF targetRect(0, 0, width, height);
    renderer.render(&painter, targetRect, static_cast<size_t>(pageIndex));
    painter.end();

    image = ImageProcessing::setDpi(image, targetDpi);
    return image;
}

ConversionResult PdfImageExporter::execute(const ConversionRequest& request,
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

    QList<int> pagesToRender = request.pages;
    if (!request.pageRangeString.isEmpty())
    {
        pagesToRender = PageRangeParser::parse(request.pageRangeString, totalDocPages);
    }
    if (pagesToRender.isEmpty())
    {
        for (int i = 0; i < totalDocPages; ++i)
        {
            pagesToRender.append(i);
        }
    }

    const QString ext = formatDefaultExtension(request.format);
    const QFileInfo srcInfo(request.sourcePath);
    const QString baseName = srcInfo.completeBaseName();
    const QString outDir = request.outputDirectory.isEmpty() ? srcInfo.dir().path() : request.outputDirectory;

    TempFileGuard tempGuard(QStringLiteral("vpdf_img_"));
    if (!tempGuard.isValid())
    {
        return ConversionResult::failure(QStringLiteral("TempDirError"), QStringLiteral("Failed to create temporary directory."));
    }

    QStringList committedFiles;
    const int totalPages = pagesToRender.size();

    // Check for Multi-page TIFF export
    if (request.format == ConversionFormat::Tiff && request.multiPageTiff && totalPages > 1)
    {
        if (progress) progress(ConversionStage::Rendering, 20, QStringLiteral("Rendering pages for multi-page TIFF..."));

        const QString tempTiffPath = tempGuard.createTempFilePath(QStringLiteral("tiff"));
        QImageWriter writer(tempTiffPath, "TIFF");
        writer.setCompression(request.imageCompression);

        for (int i = 0; i < totalPages; ++i)
        {
            if (cancelToken && cancelToken->load()) return ConversionResult::cancelled();

            int pageIdx = pagesToRender[i];
            QImage img = renderPageToImage(&document, pageIdx, request.dpi, request.transparentBackground, request.grayscale);
            if (img.isNull()) continue;

            if (!writer.write(img))
            {
                return ConversionResult::failure(QStringLiteral("TiffWriteError"),
                                                 QStringLiteral("Failed to write TIFF page %1: %2").arg(i + 1).arg(writer.errorString()));
            }

            if (progress)
            {
                int p = 20 + ((i + 1) * 60) / totalPages;
                progress(ConversionStage::Rendering, p, QStringLiteral("Rendered page %1 of %2").arg(i + 1).arg(totalPages));
            }
        }

        if (progress) progress(ConversionStage::Validating, 85, QStringLiteral("Validating TIFF file..."));

        QString verifyErr;
        if (!OutputVerification::verifyImage(tempTiffPath, &verifyErr))
        {
            return ConversionResult::failure(QStringLiteral("ValidationFailed"), QStringLiteral("TIFF validation failed: %1").arg(verifyErr));
        }

        QString finalTiffPath = request.outputPath.isEmpty()
                                    ? QDir(outDir).filePath(baseName + QStringLiteral(".tiff"))
                                    : request.outputPath;
        finalTiffPath = FilenamePolicy::resolveCollision(finalTiffPath, request.overwriteExisting);

        if (progress) progress(ConversionStage::Committing, 95, QStringLiteral("Committing TIFF file..."));

        QString commitErr;
        if (!TempFileGuard::atomicCommit(tempTiffPath, finalTiffPath, &commitErr))
        {
            return ConversionResult::failure(QStringLiteral("CommitFailed"), QStringLiteral("Failed to commit final TIFF: %1").arg(commitErr));
        }

        committedFiles.append(finalTiffPath);
    }
    else
    {
        // Standard single image per page
        for (int i = 0; i < totalPages; ++i)
        {
            if (cancelToken && cancelToken->load()) return ConversionResult::cancelled();

            int pageIdx = pagesToRender[i];
            QImage img = renderPageToImage(&document, pageIdx, request.dpi, request.transparentBackground, request.grayscale);
            if (img.isNull()) continue;

            const QString tempImgPath = tempGuard.createTempFilePath(ext);
            const QByteArray formatUpper = ext.toUpper().toLatin1();

            QImageWriter writer(tempImgPath, formatUpper);
            writer.setQuality(request.imageQuality);
            writer.setCompression(request.imageCompression);

            if (!writer.write(img))
            {
                return ConversionResult::failure(QStringLiteral("ImageWriteError"),
                                                 QStringLiteral("Failed to write image for page %1: %2").arg(pageIdx + 1).arg(writer.errorString()));
            }

            QString verifyErr;
            if (!OutputVerification::verifyImage(tempImgPath, &verifyErr))
            {
                return ConversionResult::failure(QStringLiteral("ValidationFailed"), QStringLiteral("Image validation failed: %1").arg(verifyErr));
            }

            // Compute final page file name
            QString finalFileName;
            if (totalPages == 1 && !request.outputPath.isEmpty() && !QFileInfo(request.outputPath).isDir())
            {
                finalFileName = request.outputPath;
            }
            else
            {
                const QString pagePattern = request.fileNameTemplate.isEmpty()
                                                ? QStringLiteral("{document}_page_{page:000}.{ext}")
                                                : request.fileNameTemplate;
                const QString relName = FilenamePolicy::formatPageFileName(pagePattern, baseName, pageIdx, ext);
                finalFileName = QDir(outDir).filePath(relName);
            }

            finalFileName = FilenamePolicy::resolveCollision(finalFileName, request.overwriteExisting);

            QString commitErr;
            if (!TempFileGuard::atomicCommit(tempImgPath, finalFileName, &commitErr))
            {
                return ConversionResult::failure(QStringLiteral("CommitFailed"), QStringLiteral("Failed to commit image: %1").arg(commitErr));
            }

            committedFiles.append(finalFileName);

            if (progress)
            {
                int p = 10 + ((i + 1) * 80) / totalPages;
                progress(ConversionStage::Rendering, p, QStringLiteral("Exported page %1 of %2").arg(i + 1).arg(totalPages));
            }
        }
    }

    if (progress) progress(ConversionStage::Completed, 100, QStringLiteral("Export completed."));

    ConversionResult res = ConversionResult::success(committedFiles.isEmpty() ? QString() : committedFiles.first(), committedFiles);
    res.totalPagesProcessed = totalPages;
    res.elapsedMilliseconds = timer.elapsed();
    if (!committedFiles.isEmpty())
    {
        res.outputSizeBytes = QFileInfo(committedFiles.first()).size();
    }
    return res;
}

} // namespace vectorpdf::conversion
