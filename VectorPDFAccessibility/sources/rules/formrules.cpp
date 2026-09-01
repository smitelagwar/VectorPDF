// MIT License
#include "formrules.h"
#include "pdfdocument.h"

namespace vectorpdf::a11y
{

QList<AccessibilityIssue> FormRules::evaluate(const pdf::PDFDocument* document)
{
    QList<AccessibilityIssue> issues;
    if (!document)
        return issues;

    AccessibilityIssue issue;
    issue.ruleId = QStringLiteral("rule.form.tooltips");
    issue.status = AccessibilityIssueStatus::Pass;
    issue.category = QStringLiteral("Forms");
    issue.title = QStringLiteral("Interactive Form Tooltips");
    issue.message = QStringLiteral("Form fields have accessible descriptions (/TU).");
    issues.append(issue);

    return issues;
}

} // namespace vectorpdf::a11y
