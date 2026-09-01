// MIT License
#ifndef VECTORPDF_REVIEWSIGNALSERVICE_H
#define VECTORPDF_REVIEWSIGNALSERVICE_H

#include "../workflowglobal.h"
#include "reviewdefinition.h"
#include "reviewstate.h"

namespace vectorpdf::workflow
{

class VECTORPDF_WORKFLOW_EXPORT ReviewService
{
public:
    static ReviewState calculateState(const ReviewDefinition& def);
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_REVIEWSIGNALSERVICE_H
