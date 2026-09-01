// MIT License
#ifndef VECTORPDF_REVIEWPACKAGESTORE_H
#define VECTORPDF_REVIEWPACKAGESTORE_H

#include "../workflowglobal.h"
#include "reviewdefinition.h"

namespace vectorpdf::workflow
{

class VECTORPDF_WORKFLOW_EXPORT ReviewPackageStore
{
public:
    static bool saveReview(const QString& packagePath, const ReviewDefinition& def);
    static bool loadReview(const QString& packagePath, ReviewDefinition& defOut);
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_REVIEWPACKAGESTORE_H
