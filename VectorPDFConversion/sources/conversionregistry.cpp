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

#include "conversionregistry.h"
#include "exporters/pdfimageexporter.h"
#include "exporters/pdfdocxexporter.h"
#include "exporters/pdfxlsxexporter.h"
#include "exporters/pdfpptxexporter.h"
#include "exporters/pdfaexporter.h"
#include "exporters/monochromepdfexporter.h"
#include "exporters/xfdfexporter.h"
#include "exporters/fdfexporter.h"

#include "creators/imagepdfcreator.h"
#include "creators/markdownpdfcreator.h"
#include "creators/htmlpdfcreator.h"
#include "creators/officepdfcreator.h"
#include "creators/folderpdfcreator.h"

#include <QFileInfo>

namespace vectorpdf::conversion
{

ConversionRegistry& ConversionRegistry::instance()
{
    static ConversionRegistry s_instance;
    return s_instance;
}

ConversionRegistry::ConversionRegistry()
{
    registerBuiltInWorkers();
}

void ConversionRegistry::registerBuiltInWorkers()
{
    // Exporters
    auto imgExporter = std::make_shared<PdfImageExporter>();
    m_exporters[ConversionFormat::Png] = imgExporter;
    m_exporters[ConversionFormat::Jpeg] = imgExporter;
    m_exporters[ConversionFormat::Tiff] = imgExporter;
    m_exporters[ConversionFormat::WebP] = imgExporter;
    m_exporters[ConversionFormat::Bmp] = imgExporter;

    m_exporters[ConversionFormat::Docx] = std::make_shared<PdfDocxExporter>();
    m_exporters[ConversionFormat::Xlsx] = std::make_shared<PdfXlsxExporter>();
    m_exporters[ConversionFormat::Pptx] = std::make_shared<PdfPptxExporter>();

    auto pdfaExporter = std::make_shared<PdfAExporter>();
    m_exporters[ConversionFormat::PdfA1] = pdfaExporter;
    m_exporters[ConversionFormat::PdfA2] = pdfaExporter;
    m_exporters[ConversionFormat::PdfA3] = pdfaExporter;
    m_exporters[ConversionFormat::PdfA4] = pdfaExporter;

    m_exporters[ConversionFormat::MonochromePdf] = std::make_shared<MonochromePdfExporter>();
    m_exporters[ConversionFormat::Xfdf] = std::make_shared<XfdfExporter>();
    m_exporters[ConversionFormat::Fdf] = std::make_shared<FdfExporter>();

    // Creators
    auto imgCreator = std::make_shared<ImagePdfCreator>();
    m_creators[QStringLiteral("png")] = imgCreator;
    m_creators[QStringLiteral("jpg")] = imgCreator;
    m_creators[QStringLiteral("jpeg")] = imgCreator;
    m_creators[QStringLiteral("tif")] = imgCreator;
    m_creators[QStringLiteral("tiff")] = imgCreator;
    m_creators[QStringLiteral("webp")] = imgCreator;
    m_creators[QStringLiteral("bmp")] = imgCreator;

    auto mdCreator = std::make_shared<MarkdownPdfCreator>();
    m_creators[QStringLiteral("md")] = mdCreator;
    m_creators[QStringLiteral("markdown")] = mdCreator;

    auto htmlCreator = std::make_shared<HtmlPdfCreator>();
    m_creators[QStringLiteral("html")] = htmlCreator;
    m_creators[QStringLiteral("htm")] = htmlCreator;

    auto offCreator = std::make_shared<OfficePdfCreator>();
    m_creators[QStringLiteral("doc")] = offCreator;
    m_creators[QStringLiteral("docx")] = offCreator;

    auto fldCreator = std::make_shared<FolderPdfCreator>();
    m_creators[QStringLiteral("folder")] = fldCreator;
}

std::shared_ptr<IConversionWorker> ConversionRegistry::getExporter(ConversionFormat format) const
{
    return m_exporters.value(format, nullptr);
}

std::shared_ptr<IConversionWorker> ConversionRegistry::getCreator(const QString& sourceExtensionOrFolder) const
{
    const QString ext = sourceExtensionOrFolder.trimmed().toLower();
    return m_creators.value(ext, nullptr);
}

void ConversionRegistry::registerExporter(ConversionFormat format, std::shared_ptr<IConversionWorker> exporter)
{
    m_exporters[format] = exporter;
}

void ConversionRegistry::registerCreator(const QString& extension, std::shared_ptr<IConversionWorker> creator)
{
    m_creators[extension.trimmed().toLower()] = creator;
}

} // namespace vectorpdf::conversion
