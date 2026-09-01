// MIT License
#ifndef VECTORPDF_BATESBATCHPLAN_H
#define VECTORPDF_BATESBATCHPLAN_H

#include "../documenttoolsglobal.h"
#include "batesdefinition.h"
#include <QStringList>
#include <QMap>

namespace vectorpdf::tools::bates
{

struct VECTORPDF_DOCUMENTTOOLS_EXPORT BatesBatchPlan
{
    BatesDefinition definition;
    QStringList targetFilePaths;
    QMap<QString, QString> startEndNumbersByFile;
    int totalPages = 0;

    void calculatePlan(const QList<int>& pageCounts);
};

} // namespace vectorpdf::tools::bates

#endif // VECTORPDF_BATESBATCHPLAN_H
