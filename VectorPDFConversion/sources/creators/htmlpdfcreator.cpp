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

#include "htmlpdfcreator.h"
#include "../utilities/tempfileguard.h"
#include "../utilities/filenamepolicy.h"
#include "../utilities/outputverification.h"

#include <QTextDocument>
#include <QPdfWriter>
#include <QPageSize>
#include <QPageLayout>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QUrl>
#include <QImage>
#include <QRegularExpression>
#include <QElapsedTimer>

namespace vectorpdf::conversion
{

ConversionResult HtmlPdfCreator::execute(const ConversionRequest& request,
                                         const ProgressCallback& progress,
                                         CancelToken* cancelToken)
{
    QElapsedTimer timer;
    timer.start();

    if (!QFile::exists(request.sourcePath))
    {
        return ConversionResult::failure(QStringLiteral("FileNotFound"),
                                         QStringLiteral("Source HTML file does not exist: '%1'").arg(request.sourcePath));
    }

    if (progress) progress(ConversionStage::Loading, 10, QStringLiteral("Reading HTML document..."));

    QFile file(request.sourcePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return ConversionResult::failure(QStringLiteral("OpenFailed"),
                                         QStringLiteral("Cannot open HTML file for reading: '%1'").arg(request.sourcePath));
    }

    QString htmlText = QString::fromUtf8(file.readAll());
    file.close();

    if (cancelToken && cancelToken->load()) return ConversionResult::cancelled();

    if (progress) progress(ConversionStage::Analyzing, 25, QStringLiteral("Processing HTML layout..."));

    QTextDocument doc;
    doc.setHtml(htmlText);

    // Resolve local relative images
    if (request.allowLocalImages)
    {
        QDir srcDir = QFileInfo(request.sourcePath).dir();
        static const QRegularExpression imgRegex(QStringLiteral("<img[^>]+src=[\"'](.*?)[\"']"), QRegularExpression::CaseInsensitiveOption);
        auto it = imgRegex.globalMatch(htmlText);
        while (it.hasNext())
        {
            auto match = it.next();
            QString imgPath = match.captured(1).trimmed();

            if (imgPath.startsWith(QLatin1String("http://"), Qt::CaseInsensitive) ||
                imgPath.startsWith(QLatin1String("https://"), Qt::CaseInsensitive))
            {
                continue;
            }

            QString localImgPath = srcDir.filePath(imgPath);
            if (QFile::exists(localImgPath))
            {
                QImage img(localImgPath);
                if (!img.isNull())
                {
                    doc.addResource(QTextDocument::ImageResource, QUrl(imgPath), img);
                    doc.addResource(QTextDocument::ImageResource, QUrl::fromLocalFile(localImgPath), img);
                }
            }
        }
    }

    TempFileGuard tempGuard(QStringLiteral("vpdf_html_"));
    if (!tempGuard.isValid())
    {
        return ConversionResult::failure(QStringLiteral("TempDirError"), QStringLiteral("Failed to create temporary directory."));
    }

    const QString tempPdfPath = tempGuard.createTempFilePath(QStringLiteral("pdf"));

    if (progress) progress(ConversionStage::Rendering, 50, QStringLiteral("Printing HTML pages to PDF..."));

    QPdfWriter pdfWriter(tempPdfPath);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setResolution(300);

    qreal marginMm = request.pageMarginMm > 0 ? request.pageMarginMm : 20.0;
    pdfWriter.setPageMargins(QMarginsF(marginMm, marginMm, marginMm, marginMm), QPageLayout::Millimeter);

    const QSizeF pagePoints = pdfWriter.pageLayout().paintRectPixels(300).size();
    doc.setPageSize(pagePoints);

    doc.print(&pdfWriter);

    if (progress) progress(ConversionStage::Validating, 85, QStringLiteral("Validating generated PDF..."));

    QString verifyErr;
    if (!OutputVerification::verifyPdf(tempPdfPath, 0, &verifyErr))
    {
        return ConversionResult::failure(QStringLiteral("ValidationFailed"), QStringLiteral("Generated PDF validation failed: %1").arg(verifyErr));
    }

    QString finalOutPath = request.outputPath.isEmpty()
                               ? FilenamePolicy::computeDefaultOutputPath(request.sourcePath, ConversionFormat::Pdf, request.outputDirectory)
                               : request.outputPath;
    finalOutPath = FilenamePolicy::resolveCollision(finalOutPath, request.overwriteExisting);

    if (progress) progress(ConversionStage::Committing, 95, QStringLiteral("Committing PDF file..."));

    QString commitErr;
    if (!TempFileGuard::atomicCommit(tempPdfPath, finalOutPath, &commitErr))
    {
        return ConversionResult::failure(QStringLiteral("CommitFailed"), QStringLiteral("Failed to commit final PDF: %1").arg(commitErr));
    }

    if (progress) progress(ConversionStage::Completed, 100, QStringLiteral("HTML to PDF conversion completed."));

    ConversionResult res = ConversionResult::success(finalOutPath);
    res.totalPagesProcessed = doc.pageCount();
    res.elapsedMilliseconds = timer.elapsed();
    res.outputSizeBytes = QFileInfo(finalOutPath).size();
    return res;
}

} // namespace vectorpdf::conversion
