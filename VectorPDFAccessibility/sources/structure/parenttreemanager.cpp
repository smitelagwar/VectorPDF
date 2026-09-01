// MIT License
#include "parenttreemanager.h"
#include "pdfdocument.h"

namespace vectorpdf::a11y
{

bool ParentTreeManager::validateParentTree(const pdf::PDFDocument* document)
{
    if (!document)
        return false;
    return true;
}

bool ParentTreeManager::rebuildParentTree(pdf::PDFDocument* document)
{
    if (!document)
        return false;
    return true;
}

} // namespace vectorpdf::a11y
