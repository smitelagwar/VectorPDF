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

#include "outputverification.h"
#include "../office/ooxmlpackagevalidator.h"
#include <QFile>
#include <QFileInfo>
#include <QImageReader>
#include <QXmlStreamReader>
#include <pdfdocumentreader.h>
#include <pdfdocument.h>

namespace vectorpdf::conversion
{

bool OutputVerification::verifyOutput(const QString& filePath, ConversionFormat format, QString* errorMessage)
{
    if (!QFile::exists(filePath))
    {
        if (errorMessage) *errorMessage = QStringLiteral("Output file does not exist: '%1'").arg(filePath);
        return false;
    }

    QFileInfo info(filePath);
    if (info.size() <= 0)
    {
        if (errorMessage) *errorMessage = QStringLiteral("Output file is empty: '%1'").arg(filePath);
        return false;
    }

    switch (format)
    {
    case ConversionFormat::Pdf:
    case ConversionFormat::PdfA1:
    case ConversionFormat::PdfA2:
    case ConversionFormat::PdfA3:
    case ConversionFormat::PdfA4:
    case ConversionFormat::MonochromePdf:
        return verifyPdf(filePath, 0, errorMessage);

    case ConversionFormat::Png:
    case ConversionFormat::Jpeg:
    case ConversionFormat::Tiff:
    case ConversionFormat::WebP:
    case ConversionFormat::Bmp:
        return verifyImage(filePath, errorMessage);

    case ConversionFormat::Docx:
        return verifyDocx(filePath, errorMessage);

    case ConversionFormat::Xlsx:
        return verifyXlsx(filePath, errorMessage);

    case ConversionFormat::Pptx:
        return verifyPptx(filePath, errorMessage);

    case ConversionFormat::Xfdf:
        return verifyXfdf(filePath, errorMessage);

    case ConversionFormat::Fdf:
        return verifyFdf(filePath, errorMessage);

    default:
        return true;
    }
}

bool OutputVerification::verifyPdf(const QString& filePath, int expectedPages, QString* errorMessage)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        if (errorMessage) *errorMessage = QStringLiteral("Cannot open PDF file '%1' for verification.").arg(filePath);
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    if (!data.startsWith("%PDF-"))
    {
        if (errorMessage) *errorMessage = QStringLiteral("File '%1' does not start with %%PDF- magic header.").arg(filePath);
        return false;
    }

    pdf::PDFDocument document;
    pdf::PDFDocumentReader reader(&document);
    auto res = reader.read(data);
    if (!res.isSuccessful())
    {
        if (errorMessage) *errorMessage = QStringLiteral("PDF reader failed to parse document: %1").arg(res.getMessage());
        return false;
    }

    if (!document.getCatalog() || document.getCatalog()->getPageCount() <= 0)
    {
        if (errorMessage) *errorMessage = QStringLiteral("PDF document has 0 pages.");
        return false;
    }

    if (expectedPages > 0 && document.getCatalog()->getPageCount() != expectedPages)
    {
        if (errorMessage) *errorMessage = QStringLiteral("PDF page count mismatch: expected %1, got %2.")
                                              .arg(expectedPages).arg(document.getCatalog()->getPageCount());
        return false;
    }

    return true;
}

bool OutputVerification::verifyImage(const QString& filePath, QString* errorMessage)
{
    QImageReader reader(filePath);
    if (!reader.canRead())
    {
        if (errorMessage) *errorMessage = QStringLiteral("QImageReader cannot decode image: %1").arg(reader.errorString());
        return false;
    }

    QSize size = reader.size();
    if (size.width() <= 0 || size.height() <= 0)
    {
        if (errorMessage) *errorMessage = QStringLiteral("Image has invalid dimensions: %1x%2").arg(size.width()).arg(size.height());
        return false;
    }

    return true;
}

bool OutputVerification::verifyDocx(const QString& filePath, QString* errorMessage)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        if (errorMessage) *errorMessage = QStringLiteral("Cannot open DOCX file.");
        return false;
    }

bool OutputVerification::verifyDocx(const QString& filePath, QString* errorMessage)
{
    OoxmlValidationResult res = OoxmlPackageValidator::validatePackage(filePath, ConversionFormat::Docx);
    if (!res.isValid)
    {
        if (errorMessage) *errorMessage = res.errorMessage;
        return false;
    }
    return true;
}

bool OutputVerification::verifyXlsx(const QString& filePath, QString* errorMessage)
{
    OoxmlValidationResult res = OoxmlPackageValidator::validatePackage(filePath, ConversionFormat::Xlsx);
    if (!res.isValid)
    {
        if (errorMessage) *errorMessage = res.errorMessage;
        return false;
    }
    return true;
}

bool OutputVerification::verifyPptx(const QString& filePath, QString* errorMessage)
{
    OoxmlValidationResult res = OoxmlPackageValidator::validatePackage(filePath, ConversionFormat::Pptx);
    if (!res.isValid)
    {
        if (errorMessage) *errorMessage = res.errorMessage;
        return false;
    }
    return true;
}

bool OutputVerification::verifyXfdf(const QString& filePath, QString* errorMessage)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        if (errorMessage) *errorMessage = QStringLiteral("Cannot open XFDF file.");
        return false;
    }

    QXmlStreamReader xml(&file);
    bool foundXfdfRoot = false;

    while (!xml.atEnd())
    {
        xml.readNext();
        if (xml.isStartElement())
        {
            if (xml.name().toString().compare(QLatin1String("xfdf"), Qt::CaseInsensitive) == 0)
            {
                foundXfdfRoot = true;
                break;
            }
        }
    }

    if (xml.hasError())
    {
        if (errorMessage) *errorMessage = QStringLiteral("XFDF XML parse error: %1").arg(xml.errorString());
        return false;
    }

    if (!foundXfdfRoot)
    {
        if (errorMessage) *errorMessage = QStringLiteral("XFDF document missing root <xfdf> element.");
        return false;
    }

    return true;
}

bool OutputVerification::verifyFdf(const QString& filePath, QString* errorMessage)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        if (errorMessage) *errorMessage = QStringLiteral("Cannot open FDF file.");
        return false;
    }

    QByteArray content = file.readAll();
    if (!content.startsWith("%FDF-"))
    {
        if (errorMessage) *errorMessage = QStringLiteral("FDF document missing %%FDF- magic header.");
        return false;
    }

    if (!content.contains("%%EOF"))
    {
        if (errorMessage) *errorMessage = QStringLiteral("FDF document missing %%%%EOF trailer.");
        return false;
    }

    return true;
}

} // namespace vectorpdf::conversion
