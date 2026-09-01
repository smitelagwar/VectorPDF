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

#ifndef VECTORPDF_XLSXPACKAGEWRITER_H
#define VECTORPDF_XLSXPACKAGEWRITER_H

#include "../conversionglobal.h"
#include "ooxmlpackagewriter.h"
#include "tabledetector.h"
#include <QString>
#include <QList>
#include <QVariant>

namespace vectorpdf::conversion
{

struct XlsxCell
{
    int row = 1; // 1-based
    int col = 1; // 1-based
    QVariant value;
    bool isBold = false;
};

struct XlsxWorksheet
{
    QString name = QStringLiteral("Sheet1");
    QList<XlsxCell> cells;
};

class VECTORPDF_CONVERSION_EXPORT XlsxPackageWriter
{
public:
    XlsxPackageWriter();
    ~XlsxPackageWriter() = default;

    void addSheet(const QString& sheetName);
    void setCell(int sheetIndex, int row1Based, int col1Based, const QVariant& value, bool bold = false);
    void addDetectedTable(int sheetIndex, const DetectedTable& table, int startRow = 1, int startCol = 1);

    /// Generates and saves the .xlsx archive
    bool save(const QString& targetFilePath, QString* errorMessage = nullptr);

    /// Converts (col, row) e.g. (1, 1) -> "A1", (27, 2) -> "AA2"
    static QString cellCoordinate(int col1Based, int row1Based);

private:
    void generateRels();
    void generateContentTypes();
    void generateStyles();
    void generateWorkbook();
    void generateWorksheets();
    void generateSharedStrings();

    OoxmlPackageWriter m_pkg;
    QList<XlsxWorksheet> m_sheets;
    QStringList m_sharedStrings;
    QMap<QString, int> m_stringIndexMap;

    int getOrAddSharedString(const QString& str);
};

} // namespace vectorpdf::conversion

#endif // VECTORPDF_XLSXPACKAGEWRITER_H
