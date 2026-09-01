// MIT License
#include "navigationrules.h"
#include "pdfdocument.h"

namespace vectorpdf::a11y
{

QList<AccessibilityIssue> NavigationRules::evaluate(const pdf::PDFDocument* document)
{
    QList<AccessibilityIssue> issues;
    if (!document)
        return issues;

    if (document->getPageCount() > 10 && !document->getOutline())
    {
        AccessibilityIssue issue;
        issue.ruleId = QStringLiteral("rule.nav.bookmarks");
        issue.status = AccessibilityIssueStatus::Warning;
        issue.category = QStringLiteral("Navigation");
        issue.title = QStringLiteral("Bookmarks Recommended");
        issue.message = QStringLiteral("Long documents (>10 pages) should provide outline bookmarks for easy assistive navigation.");
        issue.canAutoFix = false;
        issues.append(issue);
    }

    return issues;
}

} // namespace vectorpdf::a11y
