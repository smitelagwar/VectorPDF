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

#include "markdownpdfcreator.h"
#include "../utilities/tempfileguard.h"
#include "../utilities/filenamepolicy.h"
#include "../utilities/outputverification.h"

#include <QTextDocument>
#include <QPdfWriter>
#include <QPainter>
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

QString MarkdownPdfCreator::generateThemeCss(MarkdownTheme theme,
                                             const QString& baseFontFamily,
                                             const QString& codeFontFamily,
                                             qreal baseFontSize)
{
    QString bodyFont = baseFontFamily.isEmpty() ? QStringLiteral("Segoe UI, Noto Sans, sans-serif") : baseFontFamily;
    QString codeFont = codeFontFamily.isEmpty() ? QStringLiteral("Consolas, Cascadia Mono, monospace") : codeFontFamily;
    int fs = qMax(8, qRound(baseFontSize));

    switch (theme)
    {
    case MarkdownTheme::Academic:
        return QStringLiteral(
            "body { font-family: 'Times New Roman', serif; font-size: %1pt; line-height: 1.5; color: #111; }\n"
            "h1 { font-size: %2pt; color: #000; text-align: center; margin-top: 24pt; margin-bottom: 12pt; }\n"
            "h2 { font-size: %3pt; color: #111; margin-top: 18pt; margin-bottom: 8pt; }\n"
            "h3 { font-size: %4pt; color: #222; margin-top: 14pt; margin-bottom: 6pt; }\n"
            "p { text-align: justify; margin-bottom: 10pt; }\n"
            "code, pre { font-family: %5; font-size: %6pt; background-color: #f4f4f4; }\n"
            "table { border-collapse: collapse; width: 100%; margin: 12pt 0; }\n"
            "th, td { border: 1px solid #999; padding: 6pt 8pt; }\n"
            "th { background-color: #e9e9e9; font-weight: bold; }\n"
            "blockquote { border-left: 3px solid #666; margin: 10pt 0 10pt 16pt; padding-left: 8pt; color: #444; font-style: italic; }\n"
        ).arg(fs).arg(fs + 10).arg(fs + 6).arg(fs + 3).arg(codeFont).arg(fs - 1);

    case MarkdownTheme::Compact:
        return QStringLiteral(
            "body { font-family: %1; font-size: %2pt; line-height: 1.25; color: #222; }\n"
            "h1 { font-size: %3pt; color: #003366; margin-top: 12pt; margin-bottom: 6pt; }\n"
            "h2 { font-size: %4pt; color: #333; margin-top: 10pt; margin-bottom: 4pt; }\n"
            "h3 { font-size: %5pt; color: #444; margin-top: 8pt; margin-bottom: 3pt; }\n"
            "p { margin-bottom: 6pt; }\n"
            "code, pre { font-family: %6; font-size: %7pt; background-color: #f8f8f8; }\n"
            "table { border-collapse: collapse; width: 100%; margin: 6pt 0; }\n"
            "th, td { border: 1px solid #ccc; padding: 3pt 5pt; }\n"
            "th { background-color: #f0f0f0; }\n"
            "blockquote { border-left: 2px solid #003366; margin-left: 8pt; padding-left: 6pt; color: #555; }\n"
        ).arg(bodyFont).arg(fs - 1).arg(fs + 6).arg(fs + 3).arg(fs + 1).arg(codeFont).arg(fs - 2);

    case MarkdownTheme::DarkOnLight:
        return QStringLiteral(
            "body { font-family: %1; font-size: %2pt; line-height: 1.4; color: #0f172a; }\n"
            "h1 { font-size: %3pt; color: #0f172a; border-bottom: 2px solid #0f172a; padding-bottom: 4pt; }\n"
            "h2 { font-size: %4pt; color: #1e293b; border-bottom: 1px solid #cbd5e1; padding-bottom: 3pt; }\n"
            "h3 { font-size: %5pt; color: #334155; }\n"
            "code, pre { font-family: %6; font-size: %7pt; background-color: #1e293b; color: #f8fafc; border-radius: 3pt; padding: 2pt 4pt; }\n"
            "table { border-collapse: collapse; width: 100%; border: 2px solid #0f172a; }\n"
            "th, td { border: 1px solid #94a3b8; padding: 6pt 8pt; }\n"
            "th { background-color: #0f172a; color: #ffffff; font-weight: bold; }\n"
            "blockquote { border-left: 4px solid #3b82f6; background-color: #f8fafc; padding: 6pt 10pt; color: #334155; }\n"
        ).arg(bodyFont).arg(fs).arg(fs + 12).arg(fs + 7).arg(fs + 4).arg(codeFont).arg(fs - 1);

    case MarkdownTheme::Clean:
    default:
        return QStringLiteral(
            "body { font-family: %1; font-size: %2pt; line-height: 1.45; color: #24292f; }\n"
            "h1 { font-size: %3pt; color: #1f497d; margin-top: 18pt; margin-bottom: 10pt; font-weight: 600; }\n"
            "h2 { font-size: %4pt; color: #2e5b82; margin-top: 14pt; margin-bottom: 8pt; font-weight: 600; border-bottom: 1px solid #eaecef; padding-bottom: 3pt; }\n"
            "h3 { font-size: %5pt; color: #333333; margin-top: 12pt; margin-bottom: 6pt; font-weight: 600; }\n"
            "p { margin-top: 0; margin-bottom: 10pt; }\n"
            "a { color: #0969da; text-decoration: underline; }\n"
            "code, pre { font-family: %6; font-size: %7pt; background-color: #f6f8fa; border-radius: 4pt; padding: 2pt 4pt; }\n"
            "table { border-collapse: collapse; width: 100%; margin: 12pt 0; }\n"
            "th, td { border: 1px solid #d0d7de; padding: 6pt 10pt; text-align: left; }\n"
            "th { background-color: #f6f8fa; font-weight: 600; }\n"
            "tr:nth-child(even) { background-color: #fcfcfc; }\n"
            "blockquote { border-left: 4px solid #1f497d; margin: 10pt 0 10pt 12pt; padding: 4pt 10pt; color: #57606a; background-color: #fbfbfb; }\n"
            "ul, ol { padding-left: 20pt; margin-bottom: 10pt; }\n"
            "li { margin-bottom: 3pt; }\n"
        ).arg(bodyFont).arg(fs).arg(fs + 11).arg(fs + 6).arg(fs + 3).arg(codeFont).arg(fs - 1);
    }
}

ConversionResult MarkdownPdfCreator::execute(const ConversionRequest& request,
                                             const ProgressCallback& progress,
                                             CancelToken* cancelToken)
{
    QElapsedTimer timer;
    timer.start();

    if (!QFile::exists(request.sourcePath))
    {
        return ConversionResult::failure(QStringLiteral("FileNotFound"),
                                         QStringLiteral("Source Markdown file does not exist: '%1'").arg(request.sourcePath));
    }

    if (progress) progress(ConversionStage::Loading, 10, QStringLiteral("Reading Markdown document..."));

    QFile file(request.sourcePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return ConversionResult::failure(QStringLiteral("OpenFailed"),
                                         QStringLiteral("Cannot open Markdown file for reading: '%1'").arg(request.sourcePath));
    }

    QString markdownText = QString::fromUtf8(file.readAll());
    file.close();

    if (cancelToken && cancelToken->load()) return ConversionResult::cancelled();

    if (progress) progress(ConversionStage::Analyzing, 25, QStringLiteral("Parsing Markdown and applying typographic theme..."));

    // Handle YAML front matter if present (strip it from body to keep PDF clean)
    if (markdownText.startsWith(QStringLiteral("---")))
    {
        int secondDash = markdownText.indexOf(QStringLiteral("---"), 3);
        if (secondDash > 3)
        {
            markdownText = markdownText.mid(secondDash + 3).trimmed();
        }
    }

    QTextDocument doc;
    doc.setMarkdown(markdownText, QTextDocument::MarkdownDialectGitHub);
    doc.setDefaultStyleSheet(generateThemeCss(request.markdownTheme, request.baseFontFamily, request.codeFontFamily, request.baseFontSize));

    // Resolve local relative images
    if (request.allowLocalImages)
    {
        QDir srcDir = QFileInfo(request.sourcePath).dir();
        static const QRegularExpression imgRegex(QStringLiteral("!\\[.*?\\]\\((.*?)\\)"));
        auto it = imgRegex.globalMatch(markdownText);
        while (it.hasNext())
        {
            auto match = it.next();
            QString imgPath = match.captured(1).trimmed();

            // Ignore remote URLs for offline security
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

    TempFileGuard tempGuard(QStringLiteral("vpdf_md_"));
    if (!tempGuard.isValid())
    {
        return ConversionResult::failure(QStringLiteral("TempDirError"), QStringLiteral("Failed to create temporary directory."));
    }

    const QString tempPdfPath = tempGuard.createTempFilePath(QStringLiteral("pdf"));

    if (progress) progress(ConversionStage::Rendering, 50, QStringLiteral("Printing styled pages to PDF..."));

    QPdfWriter pdfWriter(tempPdfPath);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setResolution(300);

    qreal marginMm = request.pageMarginMm > 0 ? request.pageMarginMm : 20.0;
    pdfWriter.setPageMargins(QMarginsF(marginMm, marginMm, marginMm, marginMm), QPageLayout::Millimeter);

    // Set page size on text document (A4 in points minus margins)
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

    if (progress) progress(ConversionStage::Completed, 100, QStringLiteral("Markdown to PDF conversion completed."));

    ConversionResult res = ConversionResult::success(finalOutPath);
    res.totalPagesProcessed = doc.pageCount();
    res.elapsedMilliseconds = timer.elapsed();
    res.outputSizeBytes = QFileInfo(finalOutPath).size();
    return res;
}

} // namespace vectorpdf::conversion
