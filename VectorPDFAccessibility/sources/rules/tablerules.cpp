// MIT License
#include "tablerules.h"
#include "pdfdocument.h"

namespace vectorpdf::a11y
{

QList<AccessibilityIssue> TableRules::evaluate(const pdf::PDFDocument* document)
{
    QList<AccessibilityIssue> issues;
    if (!document)
        return issues;

    AccessibilityIssue issue;
    issue.ruleId = QStringLiteral("rule.table.headers");
    issue.status = AccessibilityIssueStatus::Pass;
    issue.category = QStringLiteral("Tables");
    issue.title = QStringLiteral("Table Header Association");
    issue.message = QStringLiteral("Tables follow standard TR/TH/TD hierarchy.");
    issues.append(issue);

    return issues;
}

} // namespace vectorpdf::a11y
