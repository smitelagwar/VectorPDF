// MIT License
#ifndef VECTORPDF_APPROVALDEFINITION_H
#define VECTORPDF_APPROVALDEFINITION_H

#include "../workflowglobal.h"
#include "../workflowtypes.h"
#include "../identity/workflowparticipant.h"
#include "approvalstep.h"
#include <QList>

namespace vectorpdf::workflow
{

struct VECTORPDF_WORKFLOW_EXPORT ApprovalDefinition
{
    WorkflowMetadata metadata;
    QList<WorkflowParticipant> participants;
    QList<ApprovalStep> steps;
    bool requireAllApprovals = true;
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_APPROVALDEFINITION_H
