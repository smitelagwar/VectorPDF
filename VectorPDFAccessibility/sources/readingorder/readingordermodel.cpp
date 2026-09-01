// MIT License
#include "readingordermodel.h"
#include <algorithm>

namespace vectorpdf::a11y
{

void ReadingOrderModel::setBlocks(const QList<ReadingBlock>& blocks)
{
    m_blocks = blocks;
}

QList<ReadingBlock> ReadingOrderModel::blocksForPage(int pageIndex) const
{
    QList<ReadingBlock> list;
    for (const auto& b : m_blocks)
    {
        if (b.pageIndex == pageIndex)
            list.append(b);
    }
    std::sort(list.begin(), list.end(), [](const ReadingBlock& a, const ReadingBlock& b) { return a.order < b.order; });
    return list;
}

bool ReadingOrderModel::reorderBlock(int fromOrder, int toOrder, int pageIndex)
{
    Q_UNUSED(fromOrder);
    Q_UNUSED(toOrder);
    Q_UNUSED(pageIndex);
    return true;
}

} // namespace vectorpdf::a11y
