// MIT License
#ifndef VECTORPDF_FLATTENPLAN_H
#define VECTORPDF_FLATTENPLAN_H

#include "../documenttoolsglobal.h"
#include "flattenpolicy.h"
#include <QStringList>

namespace vectorpdf::tools::flatten
{

struct VECTORPDF_DOCUMENTTOOLS_EXPORT FlattenPlan
{
    FlattenPolicy policy;
    int totalFormWidgets = 0;
    int totalAnnotations = 0;
    int totalStamps = 0;
    int totalLayers = 0;
    int totalLinksProtected = 0;
    int totalDigitalSignaturesProtected = 0;
    QStringList targetPages;
};

} // namespace vectorpdf::tools::flatten

#endif // VECTORPDF_FLATTENPLAN_H
