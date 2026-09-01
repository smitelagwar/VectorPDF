// MIT License
#include "approvalservice.h"

namespace vectorpdf::workflow
{

bool ApprovalService::applyDecision(ApprovalDefinition& def, const ApprovalResponse& response)
{
    for (auto& s : def.steps)
    {
        if (s.stepId == response.stepId)
        {
            s.decision = response.decision;
            s.comment = response.comment;
            s.decisionTimestamp = QDateTime::currentDateTime();
            return true;
        }
    }
    return false;
}

bool ApprovalService::isFullyApproved(const ApprovalDefinition& def)
{
    if (def.steps.isEmpty())
        return false;

    for (const auto& s : def.steps)
    {
        if (s.decision != ApprovalDecision::Approved)
            return false;
    }
    return true;
}

} // namespace vectorpdf::workflow
