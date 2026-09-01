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

#ifndef VECTORPDF_CONVERSIONREQUEST_H
#define VECTORPDF_CONVERSIONREQUEST_H

#include "conversiontypes.h"
#include <QString>
#include <QStringList>
#include <QList>
#include <QVariantMap>
#include <QUuid>

namespace vectorpdf::conversion
{

struct VECTORPDF_CONVERSION_EXPORT ConversionRequest
{
    QString jobId;
    QString sourcePath;
    QStringList sourcePaths; // For multi-image or folder conversion
    QString outputPath;
    QString outputDirectory;
    QString fileNameTemplate; // e.g. "{document}_page_{page:000}.png"

    ConversionFormat format = ConversionFormat::Pdf;
    ConversionQuality quality = ConversionQuality::Balanced;
    
    QList<int> pages; // 0-indexed page indices, empty means all pages
    QString pageRangeString; // e.g. "1-5, 8, 10-12"

    // Image options
    int dpi = 300;
    int pixelResolution = 0;
    bool dpiMode = true;
    int imageQuality = 90; // 1-100
    int imageCompression = 6; // 0-9
    bool transparentBackground = false;
    bool grayscale = false;
    bool multiPageTiff = true;

    // Office options
    DocxExportMode docxMode = DocxExportMode::Reflow;
    PptxExportMode pptxMode = PptxExportMode::Visual;
    XlsxExportMode xlsxMode = XlsxExportMode::DetectedTables;

    // Monochrome options
    MonochromeThresholdMethod monochromeMethod = MonochromeThresholdMethod::Otsu;
    int fixedThreshold = 128;
    bool preserveSearchableText = true;
    bool runOcrIfNoText = false;

    // Markdown/HTML options
    MarkdownTheme markdownTheme = MarkdownTheme::Clean;
    QString baseFontFamily = QStringLiteral("Segoe UI");
    QString codeFontFamily = QStringLiteral("Consolas");
    qreal baseFontSize = 11.0;
    qreal pageMarginMm = 20.0;
    bool includePageNumbers = true;
    bool allowLocalImages = true;

    // Form options
    bool exportFormFields = true;
    bool exportAnnotations = false;

    // Folder options
    bool recursiveFolder = true;
    bool createBookmarksFromFilenames = true;
    bool overwriteExisting = false;

    // Tracked temporary input files to be automatically deleted when job reaches terminal state
    QStringList ownedTemporaryInputPaths;

    // Generic options map for extensibility
    QVariantMap options;

    ConversionRequest()
    {
        jobId = QUuid::createUuid().toString(QUuid::WithoutBraces);
    }
};

} // namespace vectorpdf::conversion

#endif // VECTORPDF_CONVERSIONREQUEST_H
