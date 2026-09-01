// MIT License
#include "documentrules.h"
#include "pdfdocument.h"

namespace vectorpdf::a11y
{

QList<AccessibilityIssue> DocumentRules::evaluate(const pdf::PDFDocument* document)
{
    QList<AccessibilityIssue> issues;
    if (!document)
        return issues;

    // Title check
    if (document->getTitle().trimmed().isEmpty())
    {
        AccessibilityIssue issue;
        issue.ruleId = QStringLiteral("rule.doc.title");
        issue.status = AccessibilityIssueStatus::Fail;
        issue.category = QStringLiteral("Document");
        issue.title = QStringLiteral("Document Title Missing");
        issue.message = QStringLiteral("The document does not specify a title in its metadata catalog.");
        issue.canAutoFix = true;
        issue.fixActionId = QStringLiteral("fix.doc.title");
        issues.append(issue);
    }

    // Language check
    AccessibilityIssue langIssue;
    langIssue.ruleId = QStringLiteral("rule.doc.lang");
    langIssue.status = AccessibilityIssueStatus::Warning;
    langIssue.category = QStringLiteral("Document");
    langIssue.title = QStringLiteral("Primary Language Not Set");
    langIssue.message = QStringLiteral("Document natural language /Lang is not specified.");
    langIssue.canAutoFix = true;
    langIssue.fixActionId = QStringLiteral("fix.doc.lang");
    issues.append(langIssue);

    return issues;
}

} // namespace vectorpdf::a11y
