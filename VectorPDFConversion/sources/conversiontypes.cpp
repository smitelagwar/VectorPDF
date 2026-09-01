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

#include "conversiontypes.h"

namespace vectorpdf::conversion
{

QString formatToString(ConversionFormat format)
{
    switch (format)
    {
    case ConversionFormat::Docx:          return QStringLiteral("DOCX");
    case ConversionFormat::Xlsx:          return QStringLiteral("XLSX");
    case ConversionFormat::Pptx:          return QStringLiteral("PPTX");
    case ConversionFormat::Png:           return QStringLiteral("PNG");
    case ConversionFormat::Jpeg:          return QStringLiteral("JPEG");
    case ConversionFormat::Tiff:          return QStringLiteral("TIFF");
    case ConversionFormat::WebP:          return QStringLiteral("WebP");
    case ConversionFormat::Bmp:           return QStringLiteral("BMP");
    case ConversionFormat::PdfA1:         return QStringLiteral("PDF/A-1");
    case ConversionFormat::PdfA2:         return QStringLiteral("PDF/A-2");
    case ConversionFormat::PdfA3:         return QStringLiteral("PDF/A-3");
    case ConversionFormat::PdfA4:         return QStringLiteral("PDF/A-4");
    case ConversionFormat::MonochromePdf: return QStringLiteral("Monochrome PDF");
    case ConversionFormat::Xfdf:          return QStringLiteral("XFDF");
    case ConversionFormat::Fdf:           return QStringLiteral("FDF");
    case ConversionFormat::Pdf:           return QStringLiteral("PDF");
    default:                              return QStringLiteral("Unknown");
    }
}

ConversionFormat stringToFormat(const QString& formatStr, bool* ok)
{
    if (ok) *ok = true;
    const QString s = formatStr.trimmed().toUpper();
    if (s == QLatin1String("DOCX") || s == QLatin1String("DOC")) return ConversionFormat::Docx;
    if (s == QLatin1String("XLSX") || s == QLatin1String("XLS")) return ConversionFormat::Xlsx;
    if (s == QLatin1String("PPTX") || s == QLatin1String("PPT")) return ConversionFormat::Pptx;
    if (s == QLatin1String("PNG")) return ConversionFormat::Png;
    if (s == QLatin1String("JPEG") || s == QLatin1String("JPG")) return ConversionFormat::Jpeg;
    if (s == QLatin1String("TIFF") || s == QLatin1String("TIF")) return ConversionFormat::Tiff;
    if (s == QLatin1String("WEBP")) return ConversionFormat::WebP;
    if (s == QLatin1String("BMP")) return ConversionFormat::Bmp;
    if (s == QLatin1String("PDF/A-1") || s == QLatin1String("PDFA1")) return ConversionFormat::PdfA1;
    if (s == QLatin1String("PDF/A-2") || s == QLatin1String("PDFA2")) return ConversionFormat::PdfA2;
    if (s == QLatin1String("PDF/A-3") || s == QLatin1String("PDFA3")) return ConversionFormat::PdfA3;
    if (s == QLatin1String("PDF/A-4") || s == QLatin1String("PDFA4") || s == QLatin1String("PDFA")) return ConversionFormat::PdfA4;
    if (s == QLatin1String("MONOCHROME") || s == QLatin1String("MONOCHROME PDF")) return ConversionFormat::MonochromePdf;
    if (s == QLatin1String("XFDF")) return ConversionFormat::Xfdf;
    if (s == QLatin1String("FDF")) return ConversionFormat::Fdf;
    if (s == QLatin1String("PDF")) return ConversionFormat::Pdf;

    if (ok) *ok = false;
    return ConversionFormat::Pdf;
}

QString formatDefaultExtension(ConversionFormat format)
{
    switch (format)
    {
    case ConversionFormat::Docx:          return QStringLiteral("docx");
    case ConversionFormat::Xlsx:          return QStringLiteral("xlsx");
    case ConversionFormat::Pptx:          return QStringLiteral("pptx");
    case ConversionFormat::Png:           return QStringLiteral("png");
    case ConversionFormat::Jpeg:          return QStringLiteral("jpg");
    case ConversionFormat::Tiff:          return QStringLiteral("tiff");
    case ConversionFormat::WebP:          return QStringLiteral("webp");
    case ConversionFormat::Bmp:           return QStringLiteral("bmp");
    case ConversionFormat::PdfA1:
    case ConversionFormat::PdfA2:
    case ConversionFormat::PdfA3:
    case ConversionFormat::PdfA4:
    case ConversionFormat::MonochromePdf:
    case ConversionFormat::Pdf:           return QStringLiteral("pdf");
    case ConversionFormat::Xfdf:          return QStringLiteral("xfdf");
    case ConversionFormat::Fdf:           return QStringLiteral("fdf");
    default:                              return QStringLiteral("bin");
    }
}

QString statusToString(ConversionStatus status)
{
    switch (status)
    {
    case ConversionStatus::Queued:               return QStringLiteral("Queued");
    case ConversionStatus::Running:              return QStringLiteral("Running");
    case ConversionStatus::Success:              return QStringLiteral("Success");
    case ConversionStatus::SuccessWithWarnings:  return QStringLiteral("Success with warnings");
    case ConversionStatus::Cancelled:            return QStringLiteral("Cancelled");
    case ConversionStatus::Unsupported:          return QStringLiteral("Unsupported");
    case ConversionStatus::ValidationFailed:     return QStringLiteral("Validation failed");
    case ConversionStatus::Failed:               return QStringLiteral("Failed");
    default:                                     return QStringLiteral("Unknown");
    }
}

QString stageToString(ConversionStage stage)
{
    switch (stage)
    {
    case ConversionStage::Idle:       return QStringLiteral("Idle");
    case ConversionStage::Preparing:  return QStringLiteral("Preparing");
    case ConversionStage::Loading:    return QStringLiteral("Loading");
    case ConversionStage::Analyzing:  return QStringLiteral("Analyzing");
    case ConversionStage::Rendering:  return QStringLiteral("Rendering");
    case ConversionStage::Encoding:   return QStringLiteral("Encoding");
    case ConversionStage::Validating: return QStringLiteral("Validating");
    case ConversionStage::Committing: return QStringLiteral("Committing");
    case ConversionStage::Completed:  return QStringLiteral("Completed");
    case ConversionStage::Failed:     return QStringLiteral("Failed");
    case ConversionStage::Cancelled:  return QStringLiteral("Cancelled");
    default:                          return QStringLiteral("Unknown");
    }
}

} // namespace vectorpdf::conversion
