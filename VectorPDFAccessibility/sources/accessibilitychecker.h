// MIT License
#ifndef VECTORPDF_ACCESSIBILITYCHECKER_H
#define VECTORPDF_ACCESSIBILITYCHECKER_H

#include "accessibilityglobal.h"
#include "accessibilityreport.h"

namespace pdf { class PDFDocument; }

namespace vectorpdf::a11y
{

class VECTORPDF_ACCESSIBILITY_EXPORT AccessibilityChecker
{
public:
    static AccessibilityReport checkDocument(const pdf::PDFDocument* document);
    static bool applyQuickFix(pdf::PDFDocument* document, const QString& fixActionId, const QVariantMap& params = QVariantMap());
};

} // namespace vectorpdf::a11y

#endif // VECTORPDF_ACCESSIBILITYCHECKER_H
