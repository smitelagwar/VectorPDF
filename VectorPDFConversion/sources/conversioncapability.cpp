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

#include "conversioncapability.h"
#include <QStandardPaths>
#include <QFileInfo>
#include <QDir>
#include <QProcessEnvironment>

namespace vectorpdf::conversion
{

ConversionCapability& ConversionCapability::instance()
{
    static ConversionCapability s_instance;
    return s_instance;
}

ConversionCapability::ConversionCapability()
{
    refreshCapabilities();
}

void ConversionCapability::refreshCapabilities()
{
    detectLibreOffice();
    detectVeraPdf();
    detectWia();
}

void ConversionCapability::detectLibreOffice()
{
    if (!m_libreOfficePath.isEmpty() && QFileInfo::exists(m_libreOfficePath))
    {
        m_libreOfficeAvailable = true;
        return;
    }

    // Check environment PATH
    QString path = QStandardPaths::findExecutable(QStringLiteral("soffice"));
    if (path.isEmpty())
    {
        path = QStandardPaths::findExecutable(QStringLiteral("soffice.exe"));
    }

    // Check standard Windows install locations
    if (path.isEmpty())
    {
        const QStringList candidates = {
            QStringLiteral("C:/Program Files/LibreOffice/program/soffice.exe"),
            QStringLiteral("C:/Program Files (x86)/LibreOffice/program/soffice.exe")
        };
        for (const QString& candidate : candidates)
        {
            if (QFileInfo::exists(candidate))
            {
                path = candidate;
                break;
            }
        }
    }

    m_libreOfficePath = path;
    m_libreOfficeAvailable = !path.isEmpty();
}

void ConversionCapability::detectVeraPdf()
{
    if (!m_veraPdfPath.isEmpty() && QFileInfo::exists(m_veraPdfPath))
    {
        m_veraPdfAvailable = true;
        return;
    }

    QString path = QStandardPaths::findExecutable(QStringLiteral("verapdf"));
    if (path.isEmpty())
    {
        path = QStandardPaths::findExecutable(QStringLiteral("verapdf.bat"));
    }

    m_veraPdfPath = path;
    m_veraPdfAvailable = !path.isEmpty();
}

void ConversionCapability::detectWia()
{
#if defined(Q_OS_WIN)
    m_wiaAvailable = true;
#else
    m_wiaAvailable = false;
#endif
}

bool ConversionCapability::isExportSupported(ConversionFormat format) const
{
    switch (format)
    {
    case ConversionFormat::Docx:
    case ConversionFormat::Xlsx:
    case ConversionFormat::Pptx:
    case ConversionFormat::Png:
    case ConversionFormat::Jpeg:
    case ConversionFormat::Tiff:
    case ConversionFormat::WebP:
    case ConversionFormat::Bmp:
    case ConversionFormat::PdfA1:
    case ConversionFormat::PdfA2:
    case ConversionFormat::PdfA3:
    case ConversionFormat::PdfA4:
    case ConversionFormat::MonochromePdf:
    case ConversionFormat::Xfdf:
    case ConversionFormat::Fdf:
        return true;
    default:
        return false;
    }
}

bool ConversionCapability::isCreationSupported(const QString& extension) const
{
    const QString ext = extension.trimmed().toLower();
    if (ext == QLatin1String("png") || ext == QLatin1String("jpg") || ext == QLatin1String("jpeg") ||
        ext == QLatin1String("tif") || ext == QLatin1String("tiff") || ext == QLatin1String("webp") ||
        ext == QLatin1String("bmp") || ext == QLatin1String("html") || ext == QLatin1String("htm") ||
        ext == QLatin1String("md")  || ext == QLatin1String("markdown") || ext == QLatin1String("pdf"))
    {
        return true;
    }

    if (ext == QLatin1String("doc") || ext == QLatin1String("docx"))
    {
        return isLibreOfficeAvailable();
    }

    return false;
}

CapabilityInfo ConversionCapability::getExportCapability(ConversionFormat format) const
{
    CapabilityInfo info;
    info.isAvailable = true;

    switch (format)
    {
    case ConversionFormat::Png:
    case ConversionFormat::Bmp:
        info.engineName = QStringLiteral("PDF4QT Native Rasterizer + Qt Image");
        info.isLossy = false;
        info.note = QStringLiteral("Lossless raster page export.");
        break;
    case ConversionFormat::Jpeg:
    case ConversionFormat::WebP:
        info.engineName = QStringLiteral("PDF4QT Native Rasterizer + Compressed Image");
        info.isLossy = true;
        info.note = QStringLiteral("Quality-controlled compressed image export.");
        break;
    case ConversionFormat::Tiff:
        info.engineName = QStringLiteral("PDF4QT Native Rasterizer + Multi-Page TIFF");
        info.isLossy = false;
        info.note = QStringLiteral("High fidelity multi-page TIFF export.");
        break;
    case ConversionFormat::Docx:
        info.engineName = QStringLiteral("PDF4QT TextFlow + OOXML Package Writer");
        info.isLossy = false;
        info.note = QStringLiteral("Reflow editable or Visual page fidelity Word export.");
        break;
    case ConversionFormat::Xlsx:
        info.engineName = QStringLiteral("TableDetector + SpreadsheetML Writer");
        info.isLossy = false;
        info.note = QStringLiteral("Semantic table and data extraction into Excel.");
        break;
    case ConversionFormat::Pptx:
        info.engineName = QStringLiteral("PDF4QT Rasterizer + PPTX Slide Packager");
        info.isLossy = false;
        info.note = QStringLiteral("Exact visual slide-per-page presentation export.");
        break;
    case ConversionFormat::PdfA1:
    case ConversionFormat::PdfA2:
    case ConversionFormat::PdfA3:
    case ConversionFormat::PdfA4:
        info.engineName = QStringLiteral("PDF4QT Builder + veraPDF Validator");
        info.isLossy = false;
        info.note = isVeraPdfAvailable() ? QStringLiteral("Standard conformance with veraPDF validation.")
                                        : QStringLiteral("Standard conformance (veraPDF not installed).");
        break;
    case ConversionFormat::MonochromePdf:
        info.engineName = QStringLiteral("300 DPI Otsu Binarizer + 1-Bit Stream");
        info.isLossy = true;
        info.note = QStringLiteral("Compact bilevel monochrome document output.");
        break;
    case ConversionFormat::Xfdf:
        info.engineName = QStringLiteral("Native XFDF XML Serializer");
        info.isLossy = false;
        info.note = QStringLiteral("AcroForm and annotation XML data export.");
        break;
    case ConversionFormat::Fdf:
        info.engineName = QStringLiteral("Native FDF Syntax Serializer");
        info.isLossy = false;
        info.note = QStringLiteral("Standard FDF form dataset export.");
        break;
    default:
        info.isAvailable = false;
        break;
    }

    return info;
}

CapabilityInfo ConversionCapability::getCreationCapability(const QString& extension) const
{
    CapabilityInfo info;
    const QString ext = extension.trimmed().toLower();

    if (ext == QLatin1String("png") || ext == QLatin1String("jpg") || ext == QLatin1String("jpeg") ||
        ext == QLatin1String("tif") || ext == QLatin1String("tiff") || ext == QLatin1String("webp") ||
        ext == QLatin1String("bmp"))
    {
        info.isAvailable = true;
        info.isLossy = false;
        info.engineName = QStringLiteral("ImagePdfCreator + PDFDocumentBuilder");
        info.note = QStringLiteral("Converts images to high-quality PDF with customizable page layout.");
    }
    else if (ext == QLatin1String("md") || ext == QLatin1String("markdown"))
    {
        info.isAvailable = true;
        info.isLossy = false;
        info.engineName = QStringLiteral("MarkdownPdfCreator (GitHub Dialect + Typographic Themes)");
        info.note = QStringLiteral("Renders formatted Markdown into elegant, human-readable PDF.");
    }
    else if (ext == QLatin1String("html") || ext == QLatin1String("htm"))
    {
        info.isAvailable = true;
        info.isLossy = false;
        info.engineName = QStringLiteral("HtmlPdfCreator (Safe Offline Renderer)");
        info.note = QStringLiteral("Renders HTML documents offline into PDF without network leakage.");
    }
    else if (ext == QLatin1String("doc") || ext == QLatin1String("docx"))
    {
        info.isAvailable = isLibreOfficeAvailable();
        info.isLossy = false;
        info.engineName = QStringLiteral("LibreOffice Isolated Headless Worker");
        info.toolExecutablePath = m_libreOfficePath;
        info.note = isLibreOfficeAvailable() ? QStringLiteral("Converts Word documents via sandboxed LibreOffice.")
                                             : QStringLiteral("Requires LibreOffice to be installed.");
    }
    else if (ext == QLatin1String("pdf"))
    {
        info.isAvailable = true;
        info.isLossy = false;
        info.engineName = QStringLiteral("PDFDocumentManipulator (Native Merge/Passthrough)");
        info.note = QStringLiteral("Passes through or normalizes PDF files.");
    }
    else
    {
        info.isAvailable = false;
        info.note = QStringLiteral("Unsupported file type.");
    }

    return info;
}

bool ConversionCapability::isLibreOfficeAvailable() const
{
    return m_libreOfficeAvailable;
}

QString ConversionCapability::getLibreOfficePath() const
{
    return m_libreOfficePath;
}

void ConversionCapability::setLibreOfficePath(const QString& path)
{
    m_libreOfficePath = path;
    m_libreOfficeAvailable = !path.isEmpty() && QFileInfo::exists(path);
}

bool ConversionCapability::isVeraPdfAvailable() const
{
    return m_veraPdfAvailable;
}

QString ConversionCapability::getVeraPdfPath() const
{
    return m_veraPdfPath;
}

void ConversionCapability::setVeraPdfPath(const QString& path)
{
    m_veraPdfPath = path;
    m_veraPdfAvailable = !path.isEmpty() && QFileInfo::exists(path);
}

bool ConversionCapability::isWiaScannerAvailable() const
{
    return m_wiaAvailable;
}

QList<ConversionFormat> ConversionCapability::getSupportedExportFormats() const
{
    return {
        ConversionFormat::Docx,
        ConversionFormat::Xlsx,
        ConversionFormat::Pptx,
        ConversionFormat::Png,
        ConversionFormat::Jpeg,
        ConversionFormat::Tiff,
        ConversionFormat::WebP,
        ConversionFormat::Bmp,
        ConversionFormat::PdfA1,
        ConversionFormat::PdfA2,
        ConversionFormat::PdfA3,
        ConversionFormat::PdfA4,
        ConversionFormat::MonochromePdf,
        ConversionFormat::Xfdf,
        ConversionFormat::Fdf
    };
}

QStringList ConversionCapability::getSupportedCreationExtensions() const
{
    QStringList exts = {
        QStringLiteral("pdf"),
        QStringLiteral("png"),
        QStringLiteral("jpg"),
        QStringLiteral("jpeg"),
        QStringLiteral("tif"),
        QStringLiteral("tiff"),
        QStringLiteral("webp"),
        QStringLiteral("bmp"),
        QStringLiteral("html"),
        QStringLiteral("htm"),
        QStringLiteral("md"),
        QStringLiteral("markdown")
    };

    if (isLibreOfficeAvailable())
    {
        exts.append(QStringLiteral("docx"));
        exts.append(QStringLiteral("doc"));
    }

    return exts;
}

} // namespace vectorpdf::conversion
