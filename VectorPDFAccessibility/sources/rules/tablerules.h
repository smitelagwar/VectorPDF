// MIT License
#ifndef VECTORPDF_TABLERULES_H
#define VECTORPDF_TABLERULES_H

#include "../accessibilityglobal.h"
#include "../accessibilityissue.h"
#include <QList>

namespace pdf { class PDFDocument; }

namespace vectorpdf::a11y
{

class VECTORPDF_ACCESSIBILITY_EXPORT TableRules
{
public:
    static QList<AccessibilityIssue> evaluate(const pdf::PDFDocument* document);
};

} // namespace vectorpdf::a11y

#endif // VECTORPDF_TABLERULES_H
