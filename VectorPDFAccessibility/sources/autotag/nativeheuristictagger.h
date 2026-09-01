// MIT License
#ifndef VECTORPDF_NATIVEHEURISTICTAGGER_H
#define VECTORPDF_NATIVEHEURISTICTAGGER_H

#include "../accessibilityglobal.h"
#include "autotagplan.h"

namespace pdf { class PDFDocument; }

namespace vectorpdf::a11y
{

class VECTORPDF_ACCESSIBILITY_EXPORT NativeHeuristicTagger
{
public:
    static AutoTagPlan generatePlan(const pdf::PDFDocument* document);
};

} // namespace vectorpdf::a11y

#endif // VECTORPDF_NATIVEHEURISTICTAGGER_H
