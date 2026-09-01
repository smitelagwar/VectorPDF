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

#include "docxpackagewriter.h"
#include <QBuffer>
#include <QXmlStreamWriter>
#include <QDateTime>

namespace vectorpdf::conversion
{

namespace
{
QString xmlEscape(const QString& str)
{
    QString escaped = str;
    escaped.replace(QLatin1Char('&'), QStringLiteral("&amp;"));
    escaped.replace(QLatin1Char('<'), QStringLiteral("&lt;"));
    escaped.replace(QLatin1Char('>'), QStringLiteral("&gt;"));
    escaped.replace(QLatin1Char('"'), QStringLiteral("&quot;"));
    escaped.replace(QLatin1Char('\''), QStringLiteral("&apos;"));
    return escaped;
}
} // namespace

DocxPackageWriter::DocxPackageWriter()
{
}

void DocxPackageWriter::addParagraph(const DocxParagraph& paragraph)
{
    m_bodyXml.append(QStringLiteral("<w:p>"));

    if (paragraph.headingLevel > 0 || paragraph.alignment != 0)
    {
        m_bodyXml.append(QStringLiteral("<w:pPr>"));
        if (paragraph.headingLevel > 0)
        {
            m_bodyXml.append(QStringLiteral("<w:pStyle w:val=\"Heading%1\"/>").arg(paragraph.headingLevel));
        }
        if (paragraph.alignment == 1)
            m_bodyXml.append(QStringLiteral("<w:jc w:val=\"center\"/>"));
        else if (paragraph.alignment == 2)
            m_bodyXml.append(QStringLiteral("<w:jc w:val=\"right\"/>"));
        else if (paragraph.alignment == 3)
            m_bodyXml.append(QStringLiteral("<w:jc w:val=\"both\"/>"));

        m_bodyXml.append(QStringLiteral("</w:pPr>"));
    }

    for (const DocxTextRun& run : paragraph.runs)
    {
        m_bodyXml.append(QStringLiteral("<w:r>"));

        const bool hasProps = run.bold || run.italic || run.underline || run.fontSizePt != 11 ||
                              run.color != Qt::black || !run.fontFamily.isEmpty();

        if (hasProps)
        {
            m_bodyXml.append(QStringLiteral("<w:rPr>"));
            if (run.bold) m_bodyXml.append(QStringLiteral("<w:b/>"));
            if (run.italic) m_bodyXml.append(QStringLiteral("<w:i/>"));
            if (run.underline) m_bodyXml.append(QStringLiteral("<w:u w:val=\"single\"/>"));
            if (run.fontSizePt > 0)
            {
                m_bodyXml.append(QStringLiteral("<w:sz w:val=\"%1\"/>").arg(run.fontSizePt * 2));
            }
            if (run.color.isValid() && run.color != Qt::black)
            {
                m_bodyXml.append(QStringLiteral("<w:color w:val=\"%1\"/>").arg(run.color.name(QColor::HexRgb).mid(1).toUpper()));
            }
            if (!run.fontFamily.isEmpty())
            {
                m_bodyXml.append(QStringLiteral("<w:rFonts w:ascii=\"%1\" w:hAnsi=\"%1\"/>").arg(xmlEscape(run.fontFamily)));
            }
            m_bodyXml.append(QStringLiteral("</w:rPr>"));
        }

        m_bodyXml.append(QStringLiteral("<w:t xml:space=\"preserve\">%1</w:t>").arg(xmlEscape(run.text)));
        m_bodyXml.append(QStringLiteral("</w:r>"));
    }

    m_bodyXml.append(QStringLiteral("</w:p>"));
}

void DocxPackageWriter::addSimpleParagraph(const QString& text, bool bold, int headingLevel)
{
    DocxParagraph p;
    p.headingLevel = headingLevel;
    DocxTextRun r;
    r.text = text;
    r.bold = bold;
    p.runs.append(r);
    addParagraph(p);
}

void DocxPackageWriter::addPageBreak()
{
    m_bodyXml.append(QStringLiteral("<w:p><w:r><w:br w:type=\"page\"/></w:r></w:p>"));
}

void DocxPackageWriter::addTable(const DocxTable& table)
{
    if (table.rows.isEmpty()) return;

    m_bodyXml.append(QStringLiteral("<w:tbl>"));
    m_bodyXml.append(QStringLiteral("<w:tblPr><w:tblW w:w=\"0\" w:type=\"auto\"/><w:tblBorders>"
                                    "<w:top w:val=\"single\" w:sz=\"4\" w:space=\"0\" w:color=\"auto\"/>"
                                    "<w:left w:val=\"single\" w:sz=\"4\" w:space=\"0\" w:color=\"auto\"/>"
                                    "<w:bottom w:val=\"single\" w:sz=\"4\" w:space=\"0\" w:color=\"auto\"/>"
                                    "<w:right w:val=\"single\" w:sz=\"4\" w:space=\"0\" w:color=\"auto\"/>"
                                    "<w:insideH w:val=\"single\" w:sz=\"4\" w:space=\"0\" w:color=\"auto\"/>"
                                    "<w:insideV w:val=\"single\" w:sz=\"4\" w:space=\"0\" w:color=\"auto\"/>"
                                    "</w:tblBorders></w:tblPr>"));

    for (const DocxTableRow& row : table.rows)
    {
        m_bodyXml.append(QStringLiteral("<w:tr>"));
        for (const DocxTableCell& cell : row.cells)
        {
            m_bodyXml.append(QStringLiteral("<w:tc><w:tcPr>"));
            if (cell.colSpan > 1)
            {
                m_bodyXml.append(QStringLiteral("<w:gridSpan w:val=\"%1\"/>").arg(cell.colSpan));
            }
            if (cell.backgroundColor.isValid())
            {
                m_bodyXml.append(QStringLiteral("<w:shd w:val=\"clear\" w:color=\"auto\" w:fill=\"%1\"/>")
                                     .arg(cell.backgroundColor.name(QColor::HexRgb).mid(1).toUpper()));
            }
            m_bodyXml.append(QStringLiteral("</w:tcPr>"));
            m_bodyXml.append(QStringLiteral("<w:p><w:r>"));
            if (cell.bold) m_bodyXml.append(QStringLiteral("<w:rPr><w:b/></w:rPr>"));
            m_bodyXml.append(QStringLiteral("<w:t>%1</w:t></w:r></w:p></w:tc>").arg(xmlEscape(cell.text)));
        }
        m_bodyXml.append(QStringLiteral("</w:tr>"));
    }

    m_bodyXml.append(QStringLiteral("</w:tbl>"));
}

void DocxPackageWriter::addFullPageImage(const QImage& pageImage, const QSizeF& pageSizePoints)
{
    if (pageImage.isNull()) return;

    ++m_imageCounter;
    const QString rId = QStringLiteral("rIdImg%1").arg(m_imageCounter);
    const QString fileName = QStringLiteral("image%1.png").arg(m_imageCounter);

    QByteArray pngData;
    QBuffer buffer(&pngData);
    buffer.open(QIODevice::WriteOnly);
    pageImage.save(&buffer, "PNG");
    buffer.close();

    // 1 point = 12700 EMUs (English Metric Units)
    qreal ptW = pageSizePoints.width() > 0 ? pageSizePoints.width() : 595.0; // A4 default
    qreal ptH = pageSizePoints.height() > 0 ? pageSizePoints.height() : 842.0;

    // Word margin is typically 1 inch (72 pt) each side, usable area:
    qreal usablePtW = ptW - 144.0;
    qreal usablePtH = ptH - 144.0;
    if (usablePtW <= 100) usablePtW = ptW;
    if (usablePtH <= 100) usablePtH = ptH;

    qint64 cx = static_cast<qint64>(usablePtW * 12700.0);
    qint64 cy = static_cast<qint64>(usablePtH * 12700.0);

    ImageRef ref;
    ref.rId = rId;
    ref.fileName = fileName;
    ref.data = pngData;
    ref.cx = cx;
    ref.cy = cy;
    m_images.append(ref);

    // Embed DrawingML picture
    m_bodyXml.append(QStringLiteral(
        "<w:p><w:r><w:drawing>"
        "<wp:inline distT=\"0\" distB=\"0\" distL=\"0\" distR=\"0\">"
        "<wp:extent cx=\"%1\" cy=\"%2\"/>"
        "<wp:docPr id=\"%3\" name=\"Picture %3\"/>"
        "<a:graphic xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\">"
        "<a:graphicData uri=\"http://schemas.openxmlformats.org/drawingml/2006/picture\">"
        "<pic:pic xmlns:pic=\"http://schemas.openxmlformats.org/drawingml/2006/picture\">"
        "<pic:nvPicPr><pic:cNvPr id=\"%3\" name=\"Image %3\"/><pic:cNvPicPr/></pic:nvPicPr>"
        "<pic:blipFill><a:blip xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" r:embed=\"%4\"/>"
        "<a:stretch><a:fillRect/></a:stretch></pic:blipFill>"
        "<pic:spPr><a:xfrm><a:off x=\"0\" y=\"0\"/><a:ext cx=\"%1\" cy=\"%2\"/></a:xfrm>"
        "<a:prstGeom prst=\"rect\"><a:avLst/></a:prstGeom></pic:spPr>"
        "</pic:pic></a:graphicData></a:graphic></wp:inline></w:drawing></w:r></w:p>"
    ).arg(cx).arg(cy).arg(m_imageCounter).arg(rId));
}

void DocxPackageWriter::generateRels()
{
    // Root _rels/.rels
    QString rootRels = QStringLiteral(
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">\n"
        "  <Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument\" Target=\"word/document.xml\"/>\n"
        "  <Relationship Id=\"rId2\" Type=\"http://schemas.openxmlformats.org/package/2006/relationships/metadata/core-properties\" Target=\"docProps/core.xml\"/>\n"
        "  <Relationship Id=\"rId3\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/extended-properties\" Target=\"docProps/app.xml\"/>\n"
        "</Relationships>");
    m_pkg.addTextEntry(QStringLiteral("_rels/.rels"), rootRels);

    // word/_rels/document.xml.rels
    QString docRels = QStringLiteral(
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">\n"
        "  <Relationship Id=\"rIdStyles\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/styles\" Target=\"styles.xml\"/>\n");

    for (const ImageRef& img : m_images)
    {
        docRels.append(QStringLiteral("  <Relationship Id=\"%1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/image\" Target=\"media/%2\"/>\n")
                           .arg(img.rId).arg(img.fileName));
        m_pkg.addEntry(QStringLiteral("word/media/%1").arg(img.fileName), img.data, false);
    }

    docRels.append(QStringLiteral("</Relationships>"));
    m_pkg.addTextEntry(QStringLiteral("word/_rels/document.xml.rels"), docRels);
}

void DocxPackageWriter::generateContentTypes()
{
    QString ct = QStringLiteral(
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "<Types xmlns=\"http://schemas.openxmlformats.org/package/2006/content-types\">\n"
        "  <Default Extension=\"rels\" ContentType=\"application/vnd.openxmlformats-package.relationships+xml\"/>\n"
        "  <Default Extension=\"xml\" ContentType=\"application/xml\"/>\n"
        "  <Default Extension=\"png\" ContentType=\"image/png\"/>\n"
        "  <Default Extension=\"jpeg\" ContentType=\"image/jpeg\"/>\n"
        "  <Default Extension=\"jpg\" ContentType=\"image/jpeg\"/>\n"
        "  <Override PartName=\"/word/document.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.document.main+xml\"/>\n"
        "  <Override PartName=\"/word/styles.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.wordprocessingml.styles+xml\"/>\n"
        "  <Override PartName=\"/docProps/core.xml\" ContentType=\"application/vnd.openxmlformats-package.core-properties+xml\"/>\n"
        "  <Override PartName=\"/docProps/app.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.extended-properties+xml\"/>\n"
        "</Types>");
    m_pkg.addTextEntry(QStringLiteral("[Content_Types].xml"), ct);
}

void DocxPackageWriter::generateStyles()
{
    QString styles = QStringLiteral(
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "<w:styles xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\">\n"
        "  <w:docDefaults>\n"
        "    <w:rPrDefault>\n"
        "      <w:rPr>\n"
        "        <w:rFonts w:ascii=\"Segoe UI\" w:hAnsi=\"Segoe UI\" w:cs=\"Segoe UI\"/>\n"
        "        <w:sz w:val=\"22\"/>\n"
        "      </w:rPr>\n"
        "    </w:rPrDefault>\n"
        "  </w:docDefaults>\n"
        "  <w:style w:type=\"paragraph\" w:styleId=\"Normal\" w:default=\"1\">\n"
        "    <w:name w:val=\"Normal\"/>\n"
        "  </w:style>\n"
        "  <w:style w:type=\"paragraph\" w:styleId=\"Heading1\">\n"
        "    <w:name w:val=\"heading 1\"/>\n"
        "    <w:rPr>\n"
        "      <w:b/>\n"
        "      <w:sz w:val=\"40\"/>\n"
        "      <w:color w:val=\"1F497D\"/>\n"
        "    </w:rPr>\n"
        "  </w:style>\n"
        "  <w:style w:type=\"paragraph\" w:styleId=\"Heading2\">\n"
        "    <w:name w:val=\"heading 2\"/>\n"
        "    <w:rPr>\n"
        "      <w:b/>\n"
        "      <w:sz w:val=\"30\"/>\n"
        "      <w:color w:val=\"595959\"/>\n"
        "    </w:rPr>\n"
        "  </w:style>\n"
        "  <w:style w:type=\"paragraph\" w:styleId=\"Heading3\">\n"
        "    <w:name w:val=\"heading 3\"/>\n"
        "    <w:rPr>\n"
        "      <w:b/>\n"
        "      <w:sz w:val=\"26\"/>\n"
        "      <w:color w:val=\"595959\"/>\n"
        "    </w:rPr>\n"
        "  </w:style>\n"
        "</w:styles>");
    m_pkg.addTextEntry(QStringLiteral("word/styles.xml"), styles);
}

void DocxPackageWriter::generateCoreProps()
{
    const QString nowIso = QDateTime::currentDateTimeUtc().toString(Qt::ISODate);
    QString core = QStringLiteral(
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "<cp:coreProperties xmlns:cp=\"http://schemas.openxmlformats.org/package/2006/metadata/core-properties\" "
        "xmlns:dc=\"http://purl.org/dc/elements/1.1/\" xmlns:dcterms=\"http://purl.org/dc/terms/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">\n"
        "  <dc:title>Converted Document</dc:title>\n"
        "  <dc:creator>VectorPDF</dc:creator>\n"
        "  <cp:lastModifiedBy>VectorPDF</cp:lastModifiedBy>\n"
        "  <dcterms:created xsi:type=\"dcterms:W3CDTF\">%1</dcterms:created>\n"
        "  <dcterms:modified xsi:type=\"dcterms:W3CDTF\">%1</dcterms:modified>\n"
        "</cp:coreProperties>").arg(nowIso);
    m_pkg.addTextEntry(QStringLiteral("docProps/core.xml"), core);

    QString app = QStringLiteral(
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "<Properties xmlns=\"http://schemas.openxmlformats.org/officeDocument/2006/extended-properties\">\n"
        "  <Application>VectorPDF</Application>\n"
        "</Properties>");
    m_pkg.addTextEntry(QStringLiteral("docProps/app.xml"), app);
}

void DocxPackageWriter::generateDocumentXml()
{
    QString doc = QStringLiteral(
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "<w:document xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" "
        "xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" "
        "xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" "
        "xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" "
        "xmlns:pic=\"http://schemas.openxmlformats.org/drawingml/2006/picture\">\n"
        "<w:body>\n");

    doc.append(m_bodyXml);

    // Section properties (A4 with standard margins)
    doc.append(QStringLiteral(
        "<w:sectPr>\n"
        "  <w:pgSz w:w=\"11906\" w:h=\"16838\"/>\n" // A4 in 1/20th pt (dxa)
        "  <w:pgMar w:top=\"1440\" w:right=\"1440\" w:bottom=\"1440\" w:left=\"1440\"/>\n"
        "</w:sectPr>\n"
        "</w:body>\n"
        "</w:document>"));

    m_pkg.addTextEntry(QStringLiteral("word/document.xml"), doc);
}

bool DocxPackageWriter::save(const QString& targetFilePath, QString* errorMessage)
{
    generateContentTypes();
    generateRels();
    generateStyles();
    generateCoreProps();
    generateDocumentXml();

    return m_pkg.save(targetFilePath, errorMessage);
}

} // namespace vectorpdf::conversion
