// MIT License
#include "textalternativerules.h"
#include "pdfdocument.h"

namespace vectorpdf::a11y
{

QList<AccessibilityIssue> TextAlternativeRules::evaluate(const pdf::PDFDocument* document)
{
    QList<AccessibilityIssue> issues;
    if (!document)
        return issues;

    AccessibilityIssue issue;
    issue.ruleId = QStringLiteral("rule.figure.alt");
    issue.status = AccessibilityIssueStatus::ManualCheck;
    issue.category = QStringLiteral("Figures & Alt Text");
    issue.title = QStringLiteral("Figures Alternative Text Review");
    issue.message = QStringLiteral("Verify that all informative images provide descriptive Alt text and decorative images are tagged as Artifact.");
    issue.canAutoFix = false;
    issues.append(issue);

    return issues;
}

} // namespace vectorpdf::a11y
