// MIT License
#ifndef VECTORPDF_MCIDALLOCATOR_H
#define VECTORPDF_MCIDALLOCATOR_H

#include "../accessibilityglobal.h"
#include <QMap>

namespace vectorpdf::a11y
{

class VECTORPDF_ACCESSIBILITY_EXPORT McidAllocator
{
public:
    int allocateNextMcid(int pageIndex);
    void resetPage(int pageIndex);
    void resetAll();

private:
    QMap<int, int> m_nextMcidByPage;
};

} // namespace vectorpdf::a11y

#endif // VECTORPDF_MCIDALLOCATOR_H
