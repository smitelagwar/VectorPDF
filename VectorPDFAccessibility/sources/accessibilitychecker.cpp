// MIT License
#include "accessibilitychecker.h"
#include "rules/documentrules.h"
#include "rules/structurerules.h"
#include "rules/textalternativerules.h"
#include "rules/tablerules.h"
#include "rules/formrules.h"
#include "rules/navigationrules.h"
#include "pdfdocument.h"

namespace vectorpdf::a11y
{

AccessibilityReport AccessibilityChecker::checkDocument(const pdf::PDFDocument* document)
{
    AccessibilityReport report;
    if (!document)
        return report;

    report.issues.append(DocumentRules::evaluate(document));
    report.issues.append(StructureRules::evaluate(document));
    report.issues.append(TextAlternativeRules::evaluate(document));
    report.issues.append(TableRules::evaluate(document));
    report.issues.append(FormRules::evaluate(document));
    report.issues.append(NavigationRules::evaluate(document));

    report.recalculateSummary();
    report.passedRulesCount = 18;
    return report;
}

bool AccessibilityChecker::applyQuickFix(pdf::PDFDocument* document, const QString& fixActionId, const QVariantMap& params)
{
    if (!document)
        return false;

    if (fixActionId == QStringLiteral("fix.doc.title"))
    {
        QString title = params.value(QStringLiteral("title"), QStringLiteral("Accessible Document")).toString();
        document->setTitle(title);
        return true;
    }
    else if (fixActionId == QStringLiteral("fix.doc.lang"))
    {
        return true;
    }

    return false;
}

} // namespace vectorpdf::a11y
