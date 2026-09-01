// MIT License
#ifndef VECTORPDF_PARENTTREEMANAGER_H
#define VECTORPDF_PARENTTREEMANAGER_H

#include "../accessibilityglobal.h"

namespace pdf { class PDFDocument; }

namespace vectorpdf::a11y
{

class VECTORPDF_ACCESSIBILITY_EXPORT ParentTreeManager
{
public:
    static bool validateParentTree(const pdf::PDFDocument* document);
    static bool rebuildParentTree(pdf::PDFDocument* document);
};

} // namespace vectorpdf::a11y

#endif // VECTORPDF_PARENTTREEMANAGER_H
