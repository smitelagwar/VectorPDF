// MIT License
#ifndef VECTORPDF_DUPLICATEPAGEDETECTOR_H
#define VECTORPDF_DUPLICATEPAGEDETECTOR_H

#include "../scantoolsglobal.h"
#include "duplicatecluster.h"
#include "duplicatepolicy.h"
#include <QList>
#include <QImage>

namespace vectorpdf::scan
{

class VECTORPDF_SCANTOOLS_EXPORT DuplicatePageDetector
{
public:
    static QList<DuplicateCluster> findDuplicates(const QList<QImage>& pageImages, const DuplicatePolicy& policy);
};

} // namespace vectorpdf::scan

#endif // VECTORPDF_DUPLICATEPAGEDETECTOR_H
