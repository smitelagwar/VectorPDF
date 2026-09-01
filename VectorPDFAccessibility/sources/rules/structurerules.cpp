// MIT License
#include "structurerules.h"
#include "pdfdocument.h"

namespace vectorpdf::a11y
{

QList<AccessibilityIssue> StructureRules::evaluate(const pdf::PDFDocument* document)
{
    QList<AccessibilityIssue> issues;
    if (!document)
        return issues;

    // Tagged PDF structure tree check
    if (!document->getStructureTree())
    {
        AccessibilityIssue issue;
        issue.ruleId = QStringLiteral("rule.structure.tagged");
        issue.status = AccessibilityIssueStatus::Fail;
        issue.category = QStringLiteral("Structure / Tags");
        issue.title = QStringLiteral("Untagged PDF Document");
        issue.message = QStringLiteral("Document has no logical structure tree (/StructTreeRoot). Screen readers cannot interpret content flow.");
        issue.canAutoFix = true;
        issue.fixActionId = QStringLiteral("fix.autotag");
        issues.append(issue);
    }

    return issues;
}

} // namespace vectorpdf::a11y
