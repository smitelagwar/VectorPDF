// MIT License
// Copyright (c) 2026 VectorPDF Contributors
#include "pagedecorationdefinition.h"
#include <QStringList>
#include <algorithm>

namespace vectorpdf::tools::decorations
{

QList<int> PageDecorationTargetPages::resolvePages(int totalPages) const
{
    QList<int> res;
    if (totalPages <= 0)
        return res;

    if (rangeMode == PageRangeMode::AllPages)
    {
        for (int i = (excludeFirstPage ? 1 : 0); i < totalPages; ++i)
            res.append(i);
    }
    else if (rangeMode == PageRangeMode::OddPagesOnly)
    {
        for (int i = (excludeFirstPage ? 1 : 0); i < totalPages; ++i)
        {
            if ((i + 1) % 2 != 0)
                res.append(i);
        }
    }
    else if (rangeMode == PageRangeMode::EvenPagesOnly)
    {
        for (int i = (excludeFirstPage ? 1 : 0); i < totalPages; ++i)
        {
            if ((i + 1) % 2 == 0)
                res.append(i);
        }
    }
    else if (rangeMode == PageRangeMode::CustomRange)
    {
        const QStringList parts = customRangeString.split(',', Qt::SkipEmptyParts);
        for (const QString& part : parts)
        {
            QString trimmed = part.trimmed();
            if (trimmed.contains('-'))
            {
                QStringList sub = trimmed.split('-', Qt::SkipEmptyParts);
                if (sub.size() == 2)
                {
                    int start = sub[0].trimmed().toInt();
                    int end = sub[1].trimmed().toInt();
                    for (int p = qMax(1, start); p <= qMin(totalPages, end); ++p)
                    {
                        int idx = p - 1;
                        if (!res.contains(idx) && (!excludeFirstPage || idx > 0))
                            res.append(idx);
                    }
                }
            }
            else
            {
                int p = trimmed.toInt();
                if (p >= 1 && p <= totalPages)
                {
                    int idx = p - 1;
                    if (!res.contains(idx) && (!excludeFirstPage || idx > 0))
                        res.append(idx);
                }
            }
        }
    }

    std::sort(res.begin(), res.end());
    return res;
}

PageDecorationDefinition::PageDecorationDefinition(DecorationType type)
    : m_type(type)
    , m_id(QUuid::createUuid())
{
}

} // namespace vectorpdf::tools::decorations
