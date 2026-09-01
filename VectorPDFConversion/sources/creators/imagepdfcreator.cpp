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

#include "imagepdfcreator.h"
#include "../utilities/tempfileguard.h"
#include "../utilities/filenamepolicy.h"
#include "../utilities/outputverification.h"

#include <QPdfWriter>
#include <QPainter>
#include <QPageSize>
#include <QPageLayout>
#include <QImageReader>
#include <QFileInfo>
#include <QDir>
#include <QElapsedTimer>

namespace vectorpdf::conversion
{

bool ImagePdfCreator::createPdfFromImages(const QList<QImage>& images,
                                          const QString& targetPdfPath,
                                          int dpi,
                                          bool fitToA4,
                                          QString* errorMessage)
{
    if (images.isEmpty())
    {
        if (errorMessage) *errorMessage = QStringLiteral("No images provided to create PDF.");
        return false;
    }

    const int targetDpi = dpi > 0 ? dpi : 300;
    QPdfWriter writer(targetPdfPath);
    writer.setResolution(targetDpi);
    writer.setPageMargins(QMarginsF(0, 0, 0, 0), QPageLayout::Point);

    QPainter painter;
    for (int i = 0; i < images.size(); ++i)
    {
        const QImage& img = images[i];
        if (img.isNull()) continue;

        QSizeF pagePoints;
        if (fitToA4)
        {
            // A4 is 595.28 x 841.89 points
            bool isLandscape = img.width() > img.height();
            pagePoints = isLandscape ? QSizeF(841.89, 595.28) : QSizeF(595.28, 841.89);
        }
        else
        {
            // Native points based on image DPI or targetDpi
            pagePoints = QSizeF((img.width() * 72.0) / targetDpi, (img.height() * 72.0) / targetDpi);
        }

        QPageSize pageSize(pagePoints, QPageSize::Point);
        writer.setPageSize(pageSize);

        if (i == 0)
        {
            if (!painter.begin(&writer))
            {
                if (errorMessage) *errorMessage = QStringLiteral("Failed to start PDF painter.");
                return false;
            }
        }
        else
        {
            writer.newPage();
        }

        QRect paintRect(0, 0, qRound((pagePoints.width() * targetDpi) / 72.0),
                              qRound((pagePoints.height() * targetDpi) / 72.0));

        // Scale and center image inside page
        QImage scaled = img.scaled(paintRect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        int offsetX = (paintRect.width() - scaled.width()) / 2;
        int offsetY = (paintRect.height() - scaled.height()) / 2;

        painter.drawImage(QPoint(offsetX, offsetY), scaled);
    }

    if (painter.isActive())
    {
        painter.end();
    }

    return true;
}

ConversionResult ImagePdfCreator::execute(const ConversionRequest& request,
                                          const ProgressCallback& progress,
                                          CancelToken* cancelToken)
{
    QElapsedTimer timer;
    timer.start();

    QStringList inputFiles = request.sourcePaths;
    if (inputFiles.isEmpty() && !request.sourcePath.isEmpty())
    {
        inputFiles.append(request.sourcePath);
    }

    if (inputFiles.isEmpty())
    {
        return ConversionResult::failure(QStringLiteral("NoInput"), QStringLiteral("No image files specified for PDF creation."));
    }

    if (progress) progress(ConversionStage::Loading, 10, QStringLiteral("Loading and decoding images..."));

    QList<QImage> loadedImages;
    for (int i = 0; i < inputFiles.size(); ++i)
    {
        if (cancelToken && cancelToken->load()) return ConversionResult::cancelled();

        const QString& filePath = inputFiles[i];
        QImageReader reader(filePath);

        // Check if multi-image/TIFF
        int count = reader.imageCount();
        if (count > 1)
        {
            for (int f = 0; f < count; ++f)
            {
                reader.jumpToImage(f);
                QImage frame = reader.read();
                if (!frame.isNull())
                {
                    loadedImages.append(frame);
                }
            }
        }
        else
        {
            QImage img = reader.read();
            if (!img.isNull())
            {
                loadedImages.append(img);
            }
        }

        if (progress)
        {
            int p = 10 + ((i + 1) * 30) / inputFiles.size();
            progress(ConversionStage::Loading, p, QStringLiteral("Loaded %1 of %2 image files").arg(i + 1).arg(inputFiles.size()));
        }
    }

    if (loadedImages.isEmpty())
    {
        return ConversionResult::failure(QStringLiteral("ImageDecodeFailed"), QStringLiteral("Failed to decode any images from the specified inputs."));
    }

    TempFileGuard tempGuard(QStringLiteral("vpdf_imgpdf_"));
    if (!tempGuard.isValid())
    {
        return ConversionResult::failure(QStringLiteral("TempDirError"), QStringLiteral("Failed to create temporary directory."));
    }

    const QString tempPdfPath = tempGuard.createTempFilePath(QStringLiteral("pdf"));

    if (progress) progress(ConversionStage::Rendering, 50, QStringLiteral("Composing PDF pages from images..."));

    QString createErr;
    if (!createPdfFromImages(loadedImages, tempPdfPath, request.dpi > 0 ? request.dpi : 300, true, &createErr))
    {
        return ConversionResult::failure(QStringLiteral("PdfCreateFailed"), QStringLiteral("Failed to create PDF from images: %1").arg(createErr));
    }

    if (progress) progress(ConversionStage::Validating, 85, QStringLiteral("Validating generated PDF..."));

    QString verifyErr;
    if (!OutputVerification::verifyPdf(tempPdfPath, loadedImages.size(), &verifyErr))
    {
        return ConversionResult::failure(QStringLiteral("ValidationFailed"), QStringLiteral("Generated PDF validation failed: %1").arg(verifyErr));
    }

    QString finalOutPath = request.outputPath;
    if (finalOutPath.isEmpty())
    {
        const QString firstSrc = inputFiles.first();
        finalOutPath = FilenamePolicy::computeDefaultOutputPath(firstSrc, ConversionFormat::Pdf, request.outputDirectory);
    }
    finalOutPath = FilenamePolicy::resolveCollision(finalOutPath, request.overwriteExisting);

    if (progress) progress(ConversionStage::Committing, 95, QStringLiteral("Committing PDF file..."));

    QString commitErr;
    if (!TempFileGuard::atomicCommit(tempPdfPath, finalOutPath, &commitErr))
    {
        return ConversionResult::failure(QStringLiteral("CommitFailed"), QStringLiteral("Failed to commit final PDF: %1").arg(commitErr));
    }

    if (progress) progress(ConversionStage::Completed, 100, QStringLiteral("PDF created successfully."));

    ConversionResult res = ConversionResult::success(finalOutPath);
    res.totalPagesProcessed = loadedImages.size();
    res.elapsedMilliseconds = timer.elapsed();
    res.outputSizeBytes = QFileInfo(finalOutPath).size();
    return res;
}

} // namespace vectorpdf::conversion
