// MIT License
#ifndef VECTORPDF_ACCESSIBILITYREPORT_H
#define VECTORPDF_ACCESSIBILITYREPORT_H

#include "accessibilityglobal.h"
#include "accessibilityissue.h"
#include <QList>

namespace vectorpdf::a11y
{

struct VECTORPDF_ACCESSIBILITY_EXPORT AccessibilityReport
{
    QList<AccessibilityIssue> issues;
    int passedRulesCount = 0;
    int failureCount = 0;
    int warningCount = 0;
    int manualCheckCount = 0;

    bool isCompliant() const
    {
        return failureCount == 0;
    }

    void recalculateSummary();
};

} // namespace vectorpdf::a11y

#endif // VECTORPDF_ACCESSIBILITYREPORT_H
