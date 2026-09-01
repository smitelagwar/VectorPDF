// MIT License
#ifndef VECTORPDF_AUTOTAGCANDIDATE_H
#define VECTORPDF_AUTOTAGCANDIDATE_H

#include "../accessibilityglobal.h"
#include "../accessibilitytypes.h"
#include <QString>
#include <QRectF>

namespace vectorpdf::a11y
{

struct VECTORPDF_ACCESSIBILITY_EXPORT AutoTagCandidate
{
    int order = 1;
    int pageIndex = 0;
    StructureTagType tagType = StructureTagType::P;
    QRectF boundingBox;
    QString text;
    double confidence = 0.90;
    bool isArtifactCandidate = false;
    bool isLowConfidence = false;
    bool selected = true;
};

} // namespace vectorpdf::a11y

#endif // VECTORPDF_AUTOTAGCANDIDATE_H
