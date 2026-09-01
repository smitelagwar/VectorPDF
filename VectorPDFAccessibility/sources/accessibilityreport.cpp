// MIT License
#include "accessibilityreport.h"

namespace vectorpdf::a11y
{

void AccessibilityReport::recalculateSummary()
{
    failureCount = 0;
    warningCount = 0;
    manualCheckCount = 0;

    for (const auto& issue : issues)
    {
        if (issue.status == AccessibilityIssueStatus::Fail)
            failureCount++;
        else if (issue.status == AccessibilityIssueStatus::Warning)
            warningCount++;
        else if (issue.status == AccessibilityIssueStatus::ManualCheck)
            manualCheckCount++;
    }
}

} // namespace vectorpdf::a11y
