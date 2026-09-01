// MIT License
#ifndef VECTORPDF_AUTOTAGPLAN_H
#define VECTORPDF_AUTOTAGPLAN_H

#include "../accessibilityglobal.h"
#include "autotagcandidate.h"
#include <QList>

namespace vectorpdf::a11y
{

struct VECTORPDF_ACCESSIBILITY_EXPORT AutoTagPlan
{
    QList<AutoTagCandidate> candidates;
    int headingCount = 0;
    int paragraphCount = 0;
    int listCount = 0;
    int tableCount = 0;
    int figureCount = 0;
    int artifactCount = 0;
    bool requiresUserReview = false;

    void updateMetrics();
};

} // namespace vectorpdf::a11y

#endif // VECTORPDF_AUTOTAGPLAN_H
