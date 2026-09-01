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

#include "xlsxpackagewriter.h"
#include <QMap>
#include <algorithm>

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

XlsxPackageWriter::XlsxPackageWriter()
{
    addSheet(QStringLiteral("Sheet1"));
}

void XlsxPackageWriter::addSheet(const QString& sheetName)
{
    XlsxWorksheet ws;
    ws.name = sheetName.isEmpty() ? QStringLiteral("Sheet%1").arg(m_sheets.size() + 1) : sheetName;
    m_sheets.append(ws);
}

void XlsxPackageWriter::setCell(int sheetIndex, int row1Based, int col1Based, const QVariant& value, bool bold)
{
    if (sheetIndex < 0 || sheetIndex >= m_sheets.size())
    {
        if (m_sheets.isEmpty()) addSheet(QStringLiteral("Sheet1"));
        sheetIndex = 0;
    }

    XlsxCell cell;
    cell.row = row1Based;
    cell.col = col1Based;
    cell.value = value;
    cell.isBold = bold;

    m_sheets[sheetIndex].cells.append(cell);
}

void XlsxPackageWriter::addDetectedTable(int sheetIndex, const DetectedTable& table, int startRow, int startCol)
{
    for (const DetectedCell& cell : table.cells)
    {
        const int r = startRow + cell.row;
        const int c = startCol + cell.col;

        if (cell.isNumeric)
        {
            setCell(sheetIndex, r, c, cell.numericValue, cell.isHeader);
        }
        else
        {
            setCell(sheetIndex, r, c, cell.text, cell.isHeader);
        }
    }
}

QString XlsxPackageWriter::cellCoordinate(int col1Based, int row1Based)
{
    QString colStr;
    int c = col1Based;
    while (c > 0)
    {
        int rem = (c - 1) % 26;
        colStr.prepend(QChar('A' + rem));
        c = (c - 1) / 26;
    }
    return QStringLiteral("%1%2").arg(colStr).arg(row1Based);
}

int XlsxPackageWriter::getOrAddSharedString(const QString& str)
{
    auto it = m_stringIndexMap.find(str);
    if (it != m_stringIndexMap.end())
    {
        return it.value();
    }

    int idx = m_sharedStrings.size();
    m_sharedStrings.append(str);
    m_stringIndexMap[str] = idx;
    return idx;
}

void XlsxPackageWriter::generateRels()
{
    // Root _rels/.rels
    QString rootRels = QStringLiteral(
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">\n"
        "  <Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument\" Target=\"xl/workbook.xml\"/>\n"
        "</Relationships>");
    m_pkg.addTextEntry(QStringLiteral("_rels/.rels"), rootRels);

    // xl/_rels/workbook.xml.rels
    QString wbRels = QStringLiteral(
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">\n"
        "  <Relationship Id=\"rIdStyles\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/styles\" Target=\"styles.xml\"/>\n"
        "  <Relationship Id=\"rIdSharedStrings\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/sharedStrings\" Target=\"sharedStrings.xml\"/>\n");

    for (int i = 0; i < m_sheets.size(); ++i)
    {
        wbRels.append(QStringLiteral("  <Relationship Id=\"rIdSheet%1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/worksheet\" Target=\"worksheets/sheet%1.xml\"/>\n")
                          .arg(i + 1));
    }

    wbRels.append(QStringLiteral("</Relationships>"));
    m_pkg.addTextEntry(QStringLiteral("xl/_rels/workbook.xml.rels"), wbRels);
}

void XlsxPackageWriter::generateContentTypes()
{
    QString ct = QStringLiteral(
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "<Types xmlns=\"http://schemas.openxmlformats.org/package/2006/content-types\">\n"
        "  <Default Extension=\"rels\" ContentType=\"application/vnd.openxmlformats-package.relationships+xml\"/>\n"
        "  <Default Extension=\"xml\" ContentType=\"application/xml\"/>\n"
        "  <Override PartName=\"/xl/workbook.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.spreadsheetml.sheet.main+xml\"/>\n"
        "  <Override PartName=\"/xl/styles.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.spreadsheetml.styles+xml\"/>\n"
        "  <Override PartName=\"/xl/sharedStrings.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.spreadsheetml.sharedStrings+xml\"/>\n");

    for (int i = 0; i < m_sheets.size(); ++i)
    {
        ct.append(QStringLiteral("  <Override PartName=\"/xl/worksheets/sheet%1.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.spreadsheetml.worksheet+xml\"/>\n")
                      .arg(i + 1));
    }

    ct.append(QStringLiteral("</Types>"));
    m_pkg.addTextEntry(QStringLiteral("[Content_Types].xml"), ct);
}

void XlsxPackageWriter::generateStyles()
{
    QString styles = QStringLiteral(
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "<styleSheet xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\">\n"
        "  <fonts count=\"2\">\n"
        "    <font><sz val=\"11\"/><name val=\"Calibri\"/></font>\n"
        "    <font><b/><sz val=\"11\"/><name val=\"Calibri\"/></font>\n"
        "  </fonts>\n"
        "  <fills count=\"2\">\n"
        "    <fill><patternFill patternType=\"none\"/></fill>\n"
        "    <fill><patternFill patternType=\"gray125\"/></fill>\n"
        "  </fills>\n"
        "  <borders count=\"1\">\n"
        "    <border><left/><right/><top/><bottom/><diagonal/></border>\n"
        "  </borders>\n"
        "  <cellStyleXfs count=\"1\">\n"
        "    <xf numFmtId=\"0\" fontId=\"0\" fillId=\"0\" borderId=\"0\"/>\n"
        "  </cellStyleXfs>\n"
        "  <cellXfs count=\"2\">\n"
        "    <xf numFmtId=\"0\" fontId=\"0\" fillId=\"0\" borderId=\"0\" xfId=\"0\"/>\n"
        "    <xf numFmtId=\"0\" fontId=\"1\" fillId=\"0\" borderId=\"0\" xfId=\"0\" applyFont=\"1\"/>\n"
        "  </cellXfs>\n"
        "</styleSheet>");
    m_pkg.addTextEntry(QStringLiteral("xl/styles.xml"), styles);
}

void XlsxPackageWriter::generateWorkbook()
{
    QString wb = QStringLiteral(
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "<workbook xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\" "
        "xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\">\n"
        "  <sheets>\n");

    for (int i = 0; i < m_sheets.size(); ++i)
    {
        wb.append(QStringLiteral("    <sheet name=\"%1\" sheetId=\"%2\" r:id=\"rIdSheet%2\"/>\n")
                      .arg(xmlEscape(m_sheets[i].name)).arg(i + 1));
    }

    wb.append(QStringLiteral("  </sheets>\n</workbook>"));
    m_pkg.addTextEntry(QStringLiteral("xl/workbook.xml"), wb);
}

void XlsxPackageWriter::generateWorksheets()
{
    for (int s = 0; s < m_sheets.size(); ++s)
    {
        const XlsxWorksheet& ws = m_sheets[s];

        // Group cells by row
        QMap<int, QList<XlsxCell>> rowMap;
        for (const XlsxCell& c : ws.cells)
        {
            rowMap[c.row].append(c);
        }

        QString sheetXml = QStringLiteral(
            "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
            "<worksheet xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\">\n"
            "  <sheetData>\n");

        for (auto it = rowMap.constBegin(); it != rowMap.constEnd(); ++it)
        {
            const int rowNum = it.key();
            QList<XlsxCell> cells = it.value();
            std::sort(cells.begin(), cells.end(), [](const XlsxCell& a, const XlsxCell& b) { return a.col < b.col; });

            sheetXml.append(QStringLiteral("    <row r=\"%1\">\n").arg(rowNum));

            for (const XlsxCell& cell : cells)
            {
                const QString coord = cellCoordinate(cell.col, cell.row);
                const int styleIdx = cell.isBold ? 1 : 0;

                if (cell.value.typeId() == QMetaType::Double || cell.value.typeId() == QMetaType::Int ||
                    cell.value.typeId() == QMetaType::LongLong)
                {
                    sheetXml.append(QStringLiteral("      <c r=\"%1\" s=\"%2\"><v>%3</v></c>\n")
                                        .arg(coord).arg(styleIdx).arg(cell.value.toString()));
                }
                else
                {
                    const QString text = cell.value.toString();
                    const int strIdx = getOrAddSharedString(text);
                    sheetXml.append(QStringLiteral("      <c r=\"%1\" t=\"s\" s=\"%2\"><v>%3</v></c>\n")
                                        .arg(coord).arg(styleIdx).arg(strIdx));
                }
            }

            sheetXml.append(QStringLiteral("    </row>\n"));
        }

        sheetXml.append(QStringLiteral("  </sheetData>\n</worksheet>"));
        m_pkg.addTextEntry(QStringLiteral("xl/worksheets/sheet%1.xml").arg(s + 1), sheetXml);
    }
}

void XlsxPackageWriter::generateSharedStrings()
{
    QString sst = QStringLiteral(
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "<sst xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\" count=\"%1\" uniqueCount=\"%1\">\n")
                      .arg(m_sharedStrings.size());

    for (const QString& str : m_sharedStrings)
    {
        sst.append(QStringLiteral("  <si><t xml:space=\"preserve\">%1</t></si>\n").arg(xmlEscape(str)));
    }

    sst.append(QStringLiteral("</sst>"));
    m_pkg.addTextEntry(QStringLiteral("xl/sharedStrings.xml"), sst);
}

bool XlsxPackageWriter::save(const QString& targetFilePath, QString* errorMessage)
{
    // Generate worksheets first to populate shared strings
    generateWorksheets();
    generateSharedStrings();
    generateWorkbook();
    generateStyles();
    generateContentTypes();
    generateRels();

    return m_pkg.save(targetFilePath, errorMessage);
}

} // namespace vectorpdf::conversion
