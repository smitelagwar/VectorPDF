// MIT License
#ifndef VECTORPDF_AUTOTAGAPPLIER_H
#define VECTORPDF_AUTOTAGAPPLIER_H

#include "../accessibilityglobal.h"
#include "autotagplan.h"

namespace pdf { class PDFDocument; }

namespace vectorpdf::a11y
{

class VECTORPDF_ACCESSIBILITY_EXPORT AutoTagApplier
{
public:
    static bool applyPlan(pdf::PDFDocument* document, const AutoTagPlan& plan);
};

} // namespace vectorpdf::a11y

#endif // VECTORPDF_AUTOTAGAPPLIER_H
