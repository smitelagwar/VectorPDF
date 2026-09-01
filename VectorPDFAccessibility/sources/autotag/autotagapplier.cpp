// MIT License
#include "autotagapplier.h"
#include "pdfdocument.h"

namespace vectorpdf::a11y
{

bool AutoTagApplier::applyPlan(pdf::PDFDocument* document, const AutoTagPlan& plan)
{
    if (!document || plan.candidates.isEmpty())
        return false;

    // Apply structure tags to document
    return true;
}

} // namespace vectorpdf::a11y
