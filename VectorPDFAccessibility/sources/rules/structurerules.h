// MIT License
#ifndef VECTORPDF_STRUCTURERULES_H
#define VECTORPDF_STRUCTURERULES_H

#include "../accessibilityglobal.h"
#include "../accessibilityissue.h"
#include <QList>

namespace pdf { class PDFDocument; }

namespace vectorpdf::a11y
{

class VECTORPDF_ACCESSIBILITY_EXPORT StructureRules
{
public:
    static QList<AccessibilityIssue> evaluate(const pdf::PDFDocument* document);
};

} // namespace vectorpdf::a11y

#endif // VECTORPDF_STRUCTURERULES_H
