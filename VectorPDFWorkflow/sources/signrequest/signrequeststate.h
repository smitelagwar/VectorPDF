// MIT License
#ifndef VECTORPDF_SIGNREQUESTSTATE_H
#define VECTORPDF_SIGNREQUESTSTATE_H

#include "../workflowglobal.h"
#include "../workflowtypes.h"

namespace vectorpdf::workflow
{

struct VECTORPDF_WORKFLOW_EXPORT SignRequestState
{
    WorkflowStatus status = WorkflowStatus::Draft;
    int totalFields = 0;
    int filledFields = 0;
    int totalParticipants = 0;
    int completedParticipants = 0;
    int activeSequenceIndex = 1;

    double progressPercentage() const
    {
        if (totalFields == 0) return 0.0;
        return (static_cast<double>(filledFields) / totalFields) * 100.0;
    }
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_SIGNREQUESTSTATE_H
