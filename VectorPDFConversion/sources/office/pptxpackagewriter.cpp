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

#include "pptxpackagewriter.h"
#include <QBuffer>

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

PptxPackageWriter::PptxPackageWriter()
{
}

void PptxPackageWriter::addVisualSlide(const QImage& pageImage, const QSizeF& pageSizePoints)
{
    PptxSlide slide;
    slide.backgroundImage = pageImage;
    slide.slideSizePt = pageSizePoints.width() > 0 && pageSizePoints.height() > 0
                            ? pageSizePoints
                            : QSizeF(720, 540);
    m_slides.append(slide);
}

void PptxPackageWriter::addEditableSlide(const QList<PptxSlideTextBox>& textBoxes, const QSizeF& pageSizePoints)
{
    PptxSlide slide;
    slide.textBoxes = textBoxes;
    slide.slideSizePt = pageSizePoints.width() > 0 && pageSizePoints.height() > 0
                            ? pageSizePoints
                            : QSizeF(720, 540);
    m_slides.append(slide);
}

void PptxPackageWriter::generateRels()
{
    // Root _rels/.rels
    QString rootRels = QStringLiteral(
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">\n"
        "  <Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument\" Target=\"ppt/presentation.xml\"/>\n"
        "</Relationships>");
    m_pkg.addTextEntry(QStringLiteral("_rels/.rels"), rootRels);

    // ppt/_rels/presentation.xml.rels
    QString presRels = QStringLiteral(
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">\n"
        "  <Relationship Id=\"rIdMaster1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slideMaster\" Target=\"slideMasters/slideMaster1.xml\"/>\n"
        "  <Relationship Id=\"rIdTheme1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme\" Target=\"theme/theme1.xml\"/>\n");

    for (int i = 0; i < m_slides.size(); ++i)
    {
        presRels.append(QStringLiteral("  <Relationship Id=\"rIdSlide%1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slide\" Target=\"slides/slide%1.xml\"/>\n")
                            .arg(i + 1));
    }

    presRels.append(QStringLiteral("</Relationships>"));
    m_pkg.addTextEntry(QStringLiteral("ppt/_rels/presentation.xml.rels"), presRels);

    // ppt/slideMasters/_rels/slideMaster1.xml.rels
    QString masterRels = QStringLiteral(
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">\n"
        "  <Relationship Id=\"rIdLayout1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slideLayout\" Target=\"../slideLayouts/slideLayout1.xml\"/>\n"
        "  <Relationship Id=\"rIdTheme1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme\" Target=\"../theme/theme1.xml\"/>\n"
        "</Relationships>");
    m_pkg.addTextEntry(QStringLiteral("ppt/slideMasters/_rels/slideMaster1.xml.rels"), masterRels);

    // ppt/slideLayouts/_rels/slideLayout1.xml.rels
    QString layoutRels = QStringLiteral(
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">\n"
        "  <Relationship Id=\"rIdMaster1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slideMaster\" Target=\"../slideMasters/slideMaster1.xml\"/>\n"
        "</Relationships>");
    m_pkg.addTextEntry(QStringLiteral("ppt/slideLayouts/_rels/slideLayout1.xml.rels"), layoutRels);
}

void PptxPackageWriter::generateContentTypes()
{
    QString ct = QStringLiteral(
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "<Types xmlns=\"http://schemas.openxmlformats.org/package/2006/content-types\">\n"
        "  <Default Extension=\"rels\" ContentType=\"application/vnd.openxmlformats-package.relationships+xml\"/>\n"
        "  <Default Extension=\"xml\" ContentType=\"application/xml\"/>\n"
        "  <Default Extension=\"png\" ContentType=\"image/png\"/>\n"
        "  <Default Extension=\"jpeg\" ContentType=\"image/jpeg\"/>\n"
        "  <Default Extension=\"jpg\" ContentType=\"image/jpeg\"/>\n"
        "  <Override PartName=\"/ppt/presentation.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.presentation.main+xml\"/>\n"
        "  <Override PartName=\"/ppt/slideMasters/slideMaster1.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.slideMaster+xml\"/>\n"
        "  <Override PartName=\"/ppt/slideLayouts/slideLayout1.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.slideLayout+xml\"/>\n"
        "  <Override PartName=\"/ppt/theme/theme1.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.theme+xml\"/>\n");

    for (int i = 0; i < m_slides.size(); ++i)
    {
        ct.append(QStringLiteral("  <Override PartName=\"/ppt/slides/slide%1.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.presentationml.slide+xml\"/>\n")
                      .arg(i + 1));
    }

    ct.append(QStringLiteral("</Types>"));
    m_pkg.addTextEntry(QStringLiteral("[Content_Types].xml"), ct);
}

void PptxPackageWriter::generateTheme()
{
    QString theme = QStringLiteral(
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "<a:theme xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" name=\"VectorPDF Theme\">\n"
        "  <a:themeElements>\n"
        "    <a:clrScheme name=\"Office\">\n"
        "      <a:dk1><a:sysClr val=\"windowText\" lastClr=\"000000\"/></a:dk1>\n"
        "      <a:lt1><a:sysClr val=\"window\" lastClr=\"FFFFFF\"/></a:lt1>\n"
        "      <a:dk2><a:srgbClr val=\"1F497D\"/></a:dk2>\n"
        "      <a:lt2><a:srgbClr val=\"EEECE1\"/></a:lt2>\n"
        "      <a:accent1><a:srgbClr val=\"4F81BD\"/></a:accent1>\n"
        "      <a:accent2><a:srgbClr val=\"C0504D\"/></a:accent2>\n"
        "      <a:accent3><a:srgbClr val=\"9BBB59\"/></a:accent3>\n"
        "      <a:accent4><a:srgbClr val=\"8064A2\"/></a:accent4>\n"
        "      <a:accent5><a:srgbClr val=\"4BACC6\"/></a:accent5>\n"
        "      <a:accent6><a:srgbClr val=\"F79646\"/></a:accent6>\n"
        "      <a:hlink><a:srgbClr val=\"0000FF\"/></a:hlink>\n"
        "      <a:folHlink><a:srgbClr val=\"800080\"/></a:folHlink>\n"
        "    </a:clrScheme>\n"
        "    <a:fontScheme name=\"Office\">\n"
        "      <a:majorFont><a:latin typeface=\"Segoe UI\"/></a:majorFont>\n"
        "      <a:minorFont><a:latin typeface=\"Segoe UI\"/></a:minorFont>\n"
        "    </a:fontScheme>\n"
        "    <a:fmtScheme name=\"Office\"><a:fillStyleLst/><a:lnStyleLst/><a:effectStyleLst/><a:bgFillStyleLst/></a:fmtScheme>\n"
        "  </a:themeElements>\n"
        "</a:theme>");
    m_pkg.addTextEntry(QStringLiteral("ppt/theme/theme1.xml"), theme);
}

void PptxPackageWriter::generateSlideMaster()
{
    QString master = QStringLiteral(
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "<p:sldMaster xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" "
        "xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" "
        "xmlns:p=\"http://schemas.openxmlformats.org/presentationml/2006/main\">\n"
        "  <p:cSld><p:spTree><p:nvGrpSpPr><p:cNvPr id=\"1\" name=\"\"/><p:cNvGrpSpPr/><p:nvPr/></p:nvGrpSpPr>"
        "<p:grpSpPr><a:xfrm><a:off x=\"0\" y=\"0\"/><a:ext cx=\"0\" cy=\"0\"/><a:chOff x=\"0\" y=\"0\"/><a:chExt cx=\"0\" cy=\"0\"/></a:xfrm></p:grpSpPr></p:spTree></p:cSld>\n"
        "  <p:clrMap bg1=\"lt1\" tx1=\"dk1\" bg2=\"lt2\" tx2=\"dk2\" accent1=\"accent1\" accent2=\"accent2\" accent3=\"accent3\" accent4=\"accent4\" accent5=\"accent5\" accent6=\"accent6\" hlink=\"hlink\" folHlink=\"folHlink\"/>\n"
        "  <p:sldLayoutIdLst><p:sldLayoutId id=\"2147483649\" r:id=\"rIdLayout1\"/></p:sldLayoutIdLst>\n"
        "</p:sldMaster>");
    m_pkg.addTextEntry(QStringLiteral("ppt/slideMasters/slideMaster1.xml"), master);
}

void PptxPackageWriter::generateSlideLayout()
{
    QString layout = QStringLiteral(
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "<p:sldLayout xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" "
        "xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" "
        "xmlns:p=\"http://schemas.openxmlformats.org/presentationml/2006/main\" type=\"blank\">\n"
        "  <p:cSld><p:spTree><p:nvGrpSpPr><p:cNvPr id=\"1\" name=\"\"/><p:cNvGrpSpPr/><p:nvPr/></p:nvGrpSpPr>"
        "<p:grpSpPr><a:xfrm><a:off x=\"0\" y=\"0\"/><a:ext cx=\"0\" cy=\"0\"/><a:chOff x=\"0\" y=\"0\"/><a:chExt cx=\"0\" cy=\"0\"/></a:xfrm></p:grpSpPr></p:spTree></p:cSld>\n"
        "</p:sldLayout>");
    m_pkg.addTextEntry(QStringLiteral("ppt/slideLayouts/slideLayout1.xml"), layout);
}

void PptxPackageWriter::generatePresentation()
{
    qreal wPt = 720.0;
    qreal hPt = 540.0;
    if (!m_slides.isEmpty())
    {
        wPt = m_slides.first().slideSizePt.width();
        hPt = m_slides.first().slideSizePt.height();
    }

    qint64 cxEmu = static_cast<qint64>(wPt * 12700.0);
    qint64 cyEmu = static_cast<qint64>(hPt * 12700.0);

    QString pres = QStringLiteral(
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "<p:presentation xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" "
        "xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" "
        "xmlns:p=\"http://schemas.openxmlformats.org/presentationml/2006/main\">\n"
        "  <p:sldMasterIdLst><p:sldMasterId id=\"2147483648\" r:id=\"rIdMaster1\"/></p:sldMasterIdLst>\n"
        "  <p:sldIdLst>\n");

    for (int i = 0; i < m_slides.size(); ++i)
    {
        pres.append(QStringLiteral("    <p:sldId id=\"%1\" r:id=\"rIdSlide%2\"/>\n").arg(256 + i).arg(i + 1));
    }

    pres.append(QStringLiteral(
        "  </p:sldIdLst>\n"
        "  <p:sldSz cx=\"%1\" cy=\"%2\"/>\n"
        "  <p:notesSz cx=\"6858000\" cy=\"9144000\"/>\n"
        "</p:presentation>").arg(cxEmu).arg(cyEmu));

    m_pkg.addTextEntry(QStringLiteral("ppt/presentation.xml"), pres);
}

void PptxPackageWriter::generateSlides()
{
    for (int i = 0; i < m_slides.size(); ++i)
    {
        const PptxSlide& slide = m_slides[i];
        const int slideNum = i + 1;

        qint64 slideCx = static_cast<qint64>(slide.slideSizePt.width() * 12700.0);
        qint64 slideCy = static_cast<qint64>(slide.slideSizePt.height() * 12700.0);

        QString slideRels = QStringLiteral(
            "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
            "<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">\n"
            "  <Relationship Id=\"rIdLayout1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slideLayout\" Target=\"../slideLayouts/slideLayout1.xml\"/>\n");

        QString shapesXml;

        if (!slide.backgroundImage.isNull())
        {
            // Visual Mode: Add background image covering entire slide
            QByteArray pngData;
            QBuffer buf(&pngData);
            buf.open(QIODevice::WriteOnly);
            slide.backgroundImage.save(&buf, "PNG");
            buf.close();

            const QString imgFileName = QStringLiteral("image%1.png").arg(slideNum);
            m_pkg.addEntry(QStringLiteral("ppt/media/%1").arg(imgFileName), pngData, false);

            slideRels.append(QStringLiteral("  <Relationship Id=\"rIdImg1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/image\" Target=\"../media/%1\"/>\n")
                                 .arg(imgFileName));

            shapesXml.append(QStringLiteral(
                "<p:pic>\n"
                "  <p:nvPicPr><p:cNvPr id=\"2\" name=\"PageImage\"/><p:cNvPicPr/><p:nvPr/></p:nvPicPr>\n"
                "  <p:blipFill><a:blip r:embed=\"rIdImg1\"/><a:stretch><a:fillRect/></a:stretch></p:blipFill>\n"
                "  <p:spPr><a:xfrm><a:off x=\"0\" y=\"0\"/><a:ext cx=\"%1\" cy=\"%2\"/></a:xfrm><a:prstGeom prst=\"rect\"><a:avLst/></a:prstGeom></p:spPr>\n"
                "</p:pic>\n").arg(slideCx).arg(slideCy));
        }
        else
        {
            // Editable Mode: Add text boxes
            int spId = 2;
            for (const PptxSlideTextBox& tb : slide.textBoxes)
            {
                qint64 xEmu = static_cast<qint64>(tb.rectPt.x() * 12700.0);
                qint64 yEmu = static_cast<qint64>(tb.rectPt.y() * 12700.0);
                qint64 cxEmu = static_cast<qint64>(tb.rectPt.width() * 12700.0);
                qint64 cyEmu = static_cast<qint64>(tb.rectPt.height() * 12700.0);

                shapesXml.append(QStringLiteral(
                    "<p:sp>\n"
                    "  <p:nvSpPr><p:cNvPr id=\"%1\" name=\"TextBox %1\"/><p:cNvSpPr txBox=\"1\"/><p:nvPr/></p:nvSpPr>\n"
                    "  <p:spPr><a:xfrm><a:off x=\"%2\" y=\"%3\"/><a:ext cx=\"%4\" cy=\"%5\"/></a:xfrm><a:prstGeom prst=\"rect\"><a:avLst/></a:prstGeom></p:spPr>\n"
                    "  <p:txBody><a:bodyPr wrap=\"square\"/><a:lstStyle/><a:p><a:r><a:rPr sz=\"%6\"%7/><a:t>%8</a:t></a:r></a:p></p:txBody>\n"
                    "</p:sp>\n").arg(spId).arg(xEmu).arg(yEmu).arg(cxEmu).arg(cyEmu)
                    .arg(tb.fontSizePt * 100)
                    .arg(tb.bold ? QStringLiteral(" b=\"1\"") : QString())
                    .arg(xmlEscape(tb.text)));
                ++spId;
            }
        }

        slideRels.append(QStringLiteral("</Relationships>"));
        m_pkg.addTextEntry(QStringLiteral("ppt/slides/_rels/slide%1.xml.rels").arg(slideNum), slideRels);

        QString slideXml = QStringLiteral(
            "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
            "<p:sld xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" "
            "xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" "
            "xmlns:p=\"http://schemas.openxmlformats.org/presentationml/2006/main\">\n"
            "  <p:cSld><p:spTree><p:nvGrpSpPr><p:cNvPr id=\"1\" name=\"\"/><p:cNvGrpSpPr/><p:nvPr/></p:nvGrpSpPr>"
            "<p:grpSpPr><a:xfrm><a:off x=\"0\" y=\"0\"/><a:ext cx=\"0\" cy=\"0\"/><a:chOff x=\"0\" y=\"0\"/><a:chExt cx=\"0\" cy=\"0\"/></a:xfrm></p:grpSpPr>\n");

        slideXml.append(shapesXml);
        slideXml.append(QStringLiteral("  </p:spTree></p:cSld>\n</p:sld>"));

        m_pkg.addTextEntry(QStringLiteral("ppt/slides/slide%1.xml").arg(slideNum), slideXml);
    }
}

bool PptxPackageWriter::save(const QString& targetFilePath, QString* errorMessage)
{
    generateContentTypes();
    generateTheme();
    generateSlideMaster();
    generateSlideLayout();
    generateSlides();
    generatePresentation();
    generateRels();

    return m_pkg.save(targetFilePath, errorMessage);
}

} // namespace vectorpdf::conversion
