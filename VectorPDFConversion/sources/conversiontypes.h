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

#ifndef VECTORPDF_CONVERSIONTYPES_H
#define VECTORPDF_CONVERSIONTYPES_H

#include "conversionglobal.h"
#include <QString>
#include <QMetaType>

namespace vectorpdf::conversion
{

enum class ConversionFormat
{
    Docx,
    Xlsx,
    Pptx,
    Png,
    Jpeg,
    Tiff,
    WebP,
    Bmp,
    PdfA1,
    PdfA2,
    PdfA3,
    PdfA4,
    MonochromePdf,
    Xfdf,
    Fdf,
    Pdf
};

enum class ConversionDirection
{
    ExportFromPdf,
    CreatePdf
};

enum class ConversionCategory
{
    Document,
    Spreadsheet,
    Presentation,
    Image,
    Standard,
    FormData,
    Special
};

enum class ConversionQuality
{
    Fast,
    Balanced,
    High
};

enum class ConversionStatus
{
    Queued,
    Running,
    Success,
    SuccessWithWarnings,
    Cancelled,
    Unsupported,
    ValidationFailed,
    Failed
};

enum class ConversionStage
{
    Idle,
    Preparing,
    Loading,
    Analyzing,
    Rendering,
    Encoding,
    Validating,
    Committing,
    Completed,
    Failed,
    Cancelled
};

enum class DocxExportMode
{
    Reflow,
    Visual
};

enum class PptxExportMode
{
    Visual,
    EditablePreview
};

enum class XlsxExportMode
{
    DetectedTables,
    PageLayout
};

enum class MonochromeThresholdMethod
{
    Otsu,
    Fixed,
    Adaptive
};

enum class MarkdownTheme
{
    Clean,
    Academic,
    Compact,
    DarkOnLight
};

VECTORPDF_CONVERSION_EXPORT QString formatToString(ConversionFormat format);
VECTORPDF_CONVERSION_EXPORT ConversionFormat stringToFormat(const QString& formatStr, bool* ok = nullptr);
VECTORPDF_CONVERSION_EXPORT QString formatDefaultExtension(ConversionFormat format);
VECTORPDF_CONVERSION_EXPORT QString statusToString(ConversionStatus status);
VECTORPDF_CONVERSION_EXPORT QString stageToString(ConversionStage stage);

} // namespace vectorpdf::conversion

Q_DECLARE_METATYPE(vectorpdf::conversion::ConversionFormat)
Q_DECLARE_METATYPE(vectorpdf::conversion::ConversionStatus)
Q_DECLARE_METATYPE(vectorpdf::conversion::ConversionStage)

#endif // VECTORPDF_CONVERSIONTYPES_H
