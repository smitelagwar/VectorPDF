// MIT License
#ifndef VECTORPDF_DUPLICATECLUSTER_H
#define VECTORPDF_DUPLICATECLUSTER_H

#include "../scantoolsglobal.h"
#include <QList>

namespace vectorpdf::scan
{

struct VECTORPDF_SCANTOOLS_EXPORT DuplicateCluster
{
    int primaryPageIndex = 0;
    QList<int> duplicatePageIndices;
    double similarityScore = 1.0;
};

} // namespace vectorpdf::scan

#endif // VECTORPDF_DUPLICATECLUSTER_H
