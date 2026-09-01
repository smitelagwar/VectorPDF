// MIT License
#ifndef VECTORPDF_APPROVALSERVICE_H
#define VECTORPDF_APPROVALSERVICE_H

#include "../workflowglobal.h"
#include "approvaldefinition.h"
#include "approvalresponse.h"

namespace vectorpdf::workflow
{

class VECTORPDF_WORKFLOW_EXPORT ApprovalService
{
public:
    static bool applyDecision(ApprovalDefinition& def, const ApprovalResponse& response);
    static bool isFullyApproved(const ApprovalDefinition& def);
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_APPROVALSERVICE_H
