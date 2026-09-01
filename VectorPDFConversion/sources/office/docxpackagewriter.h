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

#ifndef VECTORPDF_DOCXPACKAGEWRITER_H
#define VECTORPDF_DOCXPACKAGEWRITER_H

#include "../conversionglobal.h"
#include "ooxmlpackagewriter.h"
#include <QString>
#include <QList>
#include <QImage>
#include <QColor>
#include <QSizeF>

namespace vectorpdf::conversion
{

struct DocxTextRun
{
    QString text;
    bool bold = false;
    bool italic = false;
    bool underline = false;
    int fontSizePt = 11;
    QString fontFamily;
    QColor color = Qt::black;
};

struct DocxParagraph
{
    QList<DocxTextRun> runs;
    int headingLevel = 0; // 0 = normal paragraph, 1 = H1, 2 = H2, 3 = H3
    bool isBullet = false;
    bool isNumbered = false;
    int alignment = 0; // 0 = left, 1 = center, 2 = right, 3 = justify
};

struct DocxTableCell
{
    QString text;
    bool bold = false;
    int colSpan = 1;
    int rowSpan = 1;
    QColor backgroundColor;
};

struct DocxTableRow
{
    QList<DocxTableCell> cells;
    bool isHeader = false;
};

struct DocxTable
{
    QList<DocxTableRow> rows;
};

class VECTORPDF_CONVERSION_EXPORT DocxPackageWriter
{
public:
    DocxPackageWriter();
    ~DocxPackageWriter() = default;

    void addParagraph(const DocxParagraph& paragraph);
    void addSimpleParagraph(const QString& text, bool bold = false, int headingLevel = 0);
    void addPageBreak();
    void addTable(const DocxTable& table);

    /// Adds a high-resolution page image for Visual Fidelity mode
    void addFullPageImage(const QImage& pageImage, const QSizeF& pageSizePoints);

    /// Generates and saves the .docx archive
    bool save(const QString& targetFilePath, QString* errorMessage = nullptr);

private:
    void generateRels();
    void generateContentTypes();
    void generateStyles();
    void generateCoreProps();
    void generateDocumentXml();

    OoxmlPackageWriter m_pkg;
    QString m_bodyXml;
    int m_imageCounter = 0;

    struct ImageRef
    {
        QString rId;
        QString fileName;
        QByteArray data;
        qint64 cx = 0; // EMUs
        qint64 cy = 0; // EMUs
    };
    QList<ImageRef> m_images;
};

} // namespace vectorpdf::conversion

#endif // VECTORPDF_DOCXPACKAGEWRITER_H
