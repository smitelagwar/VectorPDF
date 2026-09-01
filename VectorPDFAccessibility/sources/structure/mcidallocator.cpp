// MIT License
#include "mcidallocator.h"

namespace vectorpdf::a11y
{

int McidAllocator::allocateNextMcid(int pageIndex)
{
    int cur = m_nextMcidByPage.value(pageIndex, 0);
    m_nextMcidByPage[pageIndex] = cur + 1;
    return cur;
}

void McidAllocator::resetPage(int pageIndex)
{
    m_nextMcidByPage.remove(pageIndex);
}

void McidAllocator::resetAll()
{
    m_nextMcidByPage.clear();
}

} // namespace vectorpdf::a11y
