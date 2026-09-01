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

#include "pagerangeparser.h"
#include <QSet>
#include <algorithm>

namespace vectorpdf::conversion
{

QList<int> PageRangeParser::parse(const QString& rangeStr, int totalPages, QString* errorMessage)
{
    const QString trimmed = rangeStr.trimmed();
    if (trimmed.isEmpty())
    {
        QList<int> allPages;
        if (totalPages > 0)
        {
            allPages.reserve(totalPages);
            for (int i = 0; i < totalPages; ++i)
            {
                allPages.append(i);
            }
        }
        return allPages;
    }

    QSet<int> pageSet;
    const QStringList parts = trimmed.split(QLatin1Char(','), Qt::SkipEmptyParts);

    for (const QString& part : parts)
    {
        const QString token = part.trimmed();
        if (token.isEmpty()) continue;

        if (token.contains(QLatin1Char('-')))
        {
            const QStringList bounds = token.split(QLatin1Char('-'));
            if (bounds.size() != 2)
            {
                if (errorMessage) *errorMessage = QStringLiteral("Invalid range syntax: '%1'").arg(token);
                return {};
            }

            bool okStart = false;
            bool okEnd = false;
            int start = bounds[0].trimmed().toInt(&okStart);
            int end = bounds[1].trimmed().toInt(&okEnd);

            if (!okStart || !okEnd || start < 1 || end < start)
            {
                if (errorMessage) *errorMessage = QStringLiteral("Invalid page numbers in range: '%1'").arg(token);
                return {};
            }

            if (totalPages > 0 && end > totalPages)
            {
                end = totalPages;
            }

            for (int p = start; p <= end; ++p)
            {
                pageSet.insert(p - 1); // convert to 0-based
            }
        }
        else
        {
            bool ok = false;
            int page = token.toInt(&ok);
            if (!ok || page < 1)
            {
                if (errorMessage) *errorMessage = QStringLiteral("Invalid page number: '%1'").arg(token);
                return {};
            }

            if (totalPages <= 0 || page <= totalPages)
            {
                pageSet.insert(page - 1);
            }
        }
    }

    QList<int> result = pageSet.values();
    std::sort(result.begin(), result.end());
    return result;
}

QString PageRangeParser::format(const QList<int>& pages)
{
    if (pages.isEmpty()) return QString();

    QList<int> sorted = pages;
    std::sort(sorted.begin(), sorted.end());
    sorted.erase(std::unique(sorted.begin(), sorted.end()), sorted.end());

    QStringList chunks;
    int rangeStart = -1;
    int prev = -1;

    for (int p : sorted)
    {
        int page1Based = p + 1;
        if (rangeStart == -1)
        {
            rangeStart = page1Based;
            prev = page1Based;
        }
        else if (page1Based == prev + 1)
        {
            prev = page1Based;
        }
        else
        {
            if (rangeStart == prev)
                chunks.append(QString::number(rangeStart));
            else
                chunks.append(QStringLiteral("%1-%2").arg(rangeStart).arg(prev));

            rangeStart = page1Based;
            prev = page1Based;
        }
    }

    if (rangeStart != -1)
    {
        if (rangeStart == prev)
            chunks.append(QString::number(rangeStart));
        else
            chunks.append(QStringLiteral("%1-%2").arg(rangeStart).arg(prev));
    }

    return chunks.join(QStringLiteral(", "));
}

} // namespace vectorpdf::conversion
