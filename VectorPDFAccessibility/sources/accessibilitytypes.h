// MIT License
#ifndef VECTORPDF_ACCESSIBILITYTYPES_H
#define VECTORPDF_ACCESSIBILITYTYPES_H

#include "accessibilityglobal.h"
#include <QString>
#include <QMetaType>

namespace vectorpdf::a11y
{

enum class AccessibilityIssueStatus
{
    Pass,
    Fail,
    Warning,
    ManualCheck,
    NotApplicable
};

enum class StructureTagType
{
    Document,
    Part,
    Sect,
    H1,
    H2,
    H3,
    H4,
    H5,
    H6,
    P,
    L,
    LI,
    Lbl,
    LBody,
    Table,
    TR,
    TH,
    TD,
    Figure,
    Caption,
    Link,
    Form,
    Artifact
};

VECTORPDF_ACCESSIBILITY_EXPORT QString tagTypeToString(StructureTagType type);
VECTORPDF_ACCESSIBILITY_EXPORT StructureTagType stringToTagType(const QString& name);

} // namespace vectorpdf::a11y

#endif // VECTORPDF_ACCESSIBILITYTYPES_H
