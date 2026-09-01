// MIT License
#ifndef VECTORPDF_ACCESSIBILITYISSUE_H
#define VECTORPDF_ACCESSIBILITYISSUE_H

#include "accessibilityglobal.h"
#include "accessibilitytypes.h"
#include <QString>
#include <QRectF>

namespace vectorpdf::a11y
{

struct VECTORPDF_ACCESSIBILITY_EXPORT AccessibilityIssue
{
    QString ruleId;
    AccessibilityIssueStatus status = AccessibilityIssueStatus::Fail;
    QString category;
    QString title;
    QString message;
    int pageIndex = 0;
    QRectF boundingBox;
    bool canAutoFix = false;
    QString fixActionId;
};

} // namespace vectorpdf::a11y

#endif // VECTORPDF_ACCESSIBILITYISSUE_H
