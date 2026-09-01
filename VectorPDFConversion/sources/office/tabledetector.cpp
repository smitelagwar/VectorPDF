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

#include "tabledetector.h"
#include <pdfdocument.h>
#include <pdfdocumenttextflow.h>
#include <QMap>
#include <algorithm>
#include <cmath>

namespace vectorpdf::conversion
{

QString DetectedTable::getCellText(int row, int col) const
{
    for (const DetectedCell& cell : cells)
    {
        if (cell.row == row && cell.col == col)
        {
            return cell.text;
        }
    }
    return QString();
}

QList<DetectedTable> TableDetector::detectTables(const pdf::PDFDocument* document, int pageIndex)
{
    if (!document) return {};

    pdf::PDFDocumentTextFlowFactory factory;
    factory.setCalculateBoundingBoxes(true);

    std::vector<pdf::PDFInteger> pages = { static_cast<pdf::PDFInteger>(pageIndex) };
    pdf::PDFDocumentTextFlow flow = factory.create(document, pages, pdf::PDFDocumentTextFlowFactory::Algorithm::Layout);

    struct TextItem
    {
        QString text;
        QRectF rect;
    };

    QList<TextItem> textItems;
    for (const pdf::PDFDocumentTextFlow::Item& item : flow.getItems())
    {
        if (item.isText() && !item.text.trimmed().isEmpty())
        {
            textItems.append({ item.text.trimmed(), item.boundingRect });
        }
    }

    if (textItems.size() < 4) // minimum 2x2 table needs at least 4 items
    {
        return {};
    }

    // Cluster Y coordinates (rows) within tolerance (e.g. 6 points)
    const qreal yTolerance = 6.0;
    QList<qreal> rowBaselines;

    for (const TextItem& item : textItems)
    {
        const qreal y = item.rect.center().y();
        bool found = false;
        for (qreal& base : rowBaselines)
        {
            if (std::abs(base - y) <= yTolerance)
            {
                base = (base + y) / 2.0; // average
                found = true;
                break;
            }
        }
        if (!found)
        {
            rowBaselines.append(y);
        }
    }

    std::sort(rowBaselines.begin(), rowBaselines.end());

    if (rowBaselines.size() < 2)
    {
        return {};
    }

    // Cluster X coordinates (columns) within tolerance (e.g. 15 points)
    const qreal xTolerance = 15.0;
    QList<qreal> colPositions;

    for (const TextItem& item : textItems)
    {
        const qreal x = item.rect.left();
        bool found = false;
        for (qreal& colX : colPositions)
        {
            if (std::abs(colX - x) <= xTolerance)
            {
                colX = (colX + x) / 2.0;
                found = true;
                break;
            }
        }
        if (!found)
        {
            colPositions.append(x);
        }
    }

    std::sort(colPositions.begin(), colPositions.end());

    if (colPositions.size() < 2)
    {
        return {};
    }

    // Map items into table grid
    DetectedTable table;
    table.pageIndex = pageIndex;
    table.rowCount = rowBaselines.size();
    table.colCount = colPositions.size();
    table.confidence = 0.85;

    QRectF tableBounds;

    for (const TextItem& item : textItems)
    {
        const qreal cy = item.rect.center().y();
        const qreal cx = item.rect.left();

        int bestRow = 0;
        qreal bestRowDist = 1e9;
        for (int r = 0; r < rowBaselines.size(); ++r)
        {
            qreal dist = std::abs(rowBaselines[r] - cy);
            if (dist < bestRowDist)
            {
                bestRowDist = dist;
                bestRow = r;
            }
        }

        int bestCol = 0;
        qreal bestColDist = 1e9;
        for (int c = 0; c < colPositions.size(); ++c)
        {
            qreal dist = std::abs(colPositions[c] - cx);
            if (dist < bestColDist)
            {
                bestColDist = dist;
                bestCol = c;
            }
        }

        DetectedCell cell;
        cell.row = bestRow;
        cell.col = bestCol;
        cell.text = item.text;
        cell.bounds = item.rect;
        cell.isHeader = (bestRow == 0);

        bool isNum = false;
        double numVal = item.text.toDouble(&isNum);
        if (isNum)
        {
            cell.isNumeric = true;
            cell.numericValue = numVal;
        }

        table.cells.append(cell);

        if (tableBounds.isNull())
            tableBounds = item.rect;
        else
            tableBounds = tableBounds.united(item.rect);
    }

    table.boundingBox = tableBounds;

    // Check if table has sufficient density
    if (table.cells.size() >= 4 && table.rowCount >= 2 && table.colCount >= 2)
    {
        return { table };
    }

    return {};
}

QList<DetectedTable> TableDetector::detectAllTables(const pdf::PDFDocument* document, const QList<int>& pageIndices)
{
    QList<DetectedTable> allTables;
    for (int pageIdx : pageIndices)
    {
        allTables.append(detectTables(document, pageIdx));
    }
    return allTables;
}

} // namespace vectorpdf::conversion
