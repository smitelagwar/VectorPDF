// MIT License
#ifndef VECTORPDF_APPROVALRESPONSE_H
#define VECTORPDF_APPROVALRESPONSE_H

#include "../workflowglobal.h"
#include "approvalstep.h"

namespace vectorpdf::workflow
{

struct VECTORPDF_WORKFLOW_EXPORT ApprovalResponse
{
    QUuid stepId;
    ApprovalDecision decision = ApprovalDecision::Approved;
    QString comment;
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_APPROVALRESPONSE_H
