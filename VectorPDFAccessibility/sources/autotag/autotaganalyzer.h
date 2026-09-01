// MIT License
#ifndef VECTORPDF_AUTOTAGANALYZER_H
#define VECTORPDF_AUTOTAGANALYZER_H

#include "../accessibilityglobal.h"
#include "autotagplan.h"

namespace pdf { class PDFDocument; }

namespace vectorpdf::a11y
{

enum class AutoTagBackend
{
    Auto,
    Native,
    Docling
};

class VECTORPDF_ACCESSIBILITY_EXPORT AutoTagAnalyzer
{
public:
    static AutoTagPlan analyze(const pdf::PDFDocument* document, AutoTagBackend backend = AutoTagBackend::Auto);
};

} // namespace vectorpdf::a11y

#endif // VECTORPDF_AUTOTAGANALYZER_H
