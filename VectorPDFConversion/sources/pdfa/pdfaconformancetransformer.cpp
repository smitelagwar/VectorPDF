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

#include "pdfaconformancetransformer.h"
#include "pdfaconformanceanalyzer.h"
#include "../exporters/pdfimageexporter.h"
#include "../creators/imagepdfcreator.h"

#include <pdfdocument.h>
#include <pdfdocumentwriter.h>
#include <pdfdocumentsanitizer.h>

#include <QDateTime>
#include <QXmlStreamWriter>
#include <QPdfWriter>
#include <QPainter>
#include <QPageSize>
#include <QPageLayout>
#include <QFile>

namespace vectorpdf::conversion
{

QByteArray PdfAConformanceTransformer::generatePdfAXmpPacket(PdfAProfile profile,
                                                              const QString& title,
                                                              const QString& producer)
{
    QByteArray data;
    QXmlStreamWriter xml(&data);
    xml.setAutoFormatting(true);

    const int part = pdfAProfilePart(profile);
    const QString conf = pdfAProfileConformanceString(profile);
    const QString isoDate = QDateTime::currentDateTimeUtc().toString(Qt::ISODate);

    xml.writeStartDocument();
    // XMP packet wrapper
    xml.writeStartElement(QStringLiteral("x:xmpmeta"));
    xml.writeAttribute(QStringLiteral("xmlns:x"), QStringLiteral("adobe:ns:meta/"));

    xml.writeStartElement(QStringLiteral("rdf:RDF"));
    xml.writeAttribute(QStringLiteral("xmlns:rdf"), QStringLiteral("http://www.w3.org/1999/02/22-rdf-syntax-ns#"));

    // Description 1: Dublin Core & Adobe PDF
    xml.writeStartElement(QStringLiteral("rdf:Description"));
    xml.writeAttribute(QStringLiteral("rdf:about"), QStringLiteral(""));
    xml.writeAttribute(QStringLiteral("xmlns:dc"), QStringLiteral("http://purl.org/dc/elements/1.1/"));
    xml.writeAttribute(QStringLiteral("xmlns:pdf"), QStringLiteral("http://ns.adobe.com/pdf/1.3/"));
    xml.writeAttribute(QStringLiteral("xmlns:xmp"), QStringLiteral("http://ns.adobe.com/xap/1.0/"));

    xml.writeTextElement(QStringLiteral("dc:format"), QStringLiteral("application/pdf"));
    xml.writeTextElement(QStringLiteral("pdf:Producer"), producer);
    xml.writeTextElement(QStringLiteral("xmp:CreateDate"), isoDate);
    xml.writeTextElement(QStringLiteral("xmp:ModifyDate"), isoDate);

    // Title alt-array
    xml.writeStartElement(QStringLiteral("dc:title"));
    xml.writeStartElement(QStringLiteral("rdf:Alt"));
    xml.writeStartElement(QStringLiteral("rdf:li"));
    xml.writeAttribute(QStringLiteral("xml:lang"), QStringLiteral("x-default"));
    xml.writeCharacters(title);
    xml.writeEndElement(); // rdf:li
    xml.writeEndElement(); // rdf:Alt
    xml.writeEndElement(); // dc:title

    xml.writeEndElement(); // rdf:Description (dc/pdf/xmp)

    // Description 2: PDF/A Identification
    xml.writeStartElement(QStringLiteral("rdf:Description"));
    xml.writeAttribute(QStringLiteral("rdf:about"), QStringLiteral(""));
    xml.writeAttribute(QStringLiteral("xmlns:pdfaid"), QStringLiteral("http://www.aiim.org/pdfa/ns/id/"));
    xml.writeTextElement(QStringLiteral("pdfaid:part"), QString::number(part));
    xml.writeTextElement(QStringLiteral("pdfaid:conformance"), conf);
    xml.writeEndElement(); // rdf:Description (pdfaid)

    xml.writeEndElement(); // rdf:RDF
    xml.writeEndElement(); // x:xmpmeta
    xml.writeEndDocument();

    return data;
}

bool PdfAConformanceTransformer::transform(const pdf::PDFDocument* document,
                                           PdfAProfile profile,
                                           PdfATransformationMode mode,
                                           const QString& targetPath,
                                           PdfAConversionReport* outReport,
                                           QString* errorMessage)
{
    if (!document)
    {
        if (errorMessage) *errorMessage = QStringLiteral("Document is null.");
        return false;
    }

    if (outReport)
    {
        outReport->profile = profile;
        outReport->mode = mode;
        outReport->outputFilePath = targetPath;
    }

    PdfAAnalysisReport analysis = PdfAConformanceAnalyzer::analyze(document, profile);
    if (outReport)
    {
        outReport->appliedRemediations = analysis.remediations;
    }

    if (mode == PdfATransformationMode::PreserveVector && analysis.canConvertPreservingVector)
    {
        // Mode A: Preserve Vector
        pdf::PDFDocumentWriter writer(nullptr);
        auto writeRes = writer.write(targetPath, document, true);
        if (!writeRes.isSuccessful())
        {
            if (errorMessage) *errorMessage = QStringLiteral("Failed to write transformed PDF: %1").arg(writeRes.getMessage());
            if (outReport) outReport->conversionSuccessful = false;
            return false;
        }

        if (outReport) outReport->conversionSuccessful = true;
        return true;
    }
    else
    {
        // Mode B: Archival Raster Fallback
        const int totalPages = static_cast<int>(document->getCatalog()->getPageCount());
        QList<QImage> renderedPages;
        for (int i = 0; i < totalPages; ++i)
        {
            QImage img = PdfImageExporter::renderPageToImage(document, i, 300);
            if (!img.isNull())
            {
                renderedPages.append(img);
            }
        }

        if (renderedPages.isEmpty())
        {
            if (errorMessage) *errorMessage = QStringLiteral("Failed to render pages for archival raster conversion.");
            if (outReport) outReport->conversionSuccessful = false;
            return false;
        }

        bool ok = ImagePdfCreator::createPdfFromImages(renderedPages, targetPath, 300, true, errorMessage);
        if (outReport) outReport->conversionSuccessful = ok;
        return ok;
    }
}

} // namespace vectorpdf::conversion
