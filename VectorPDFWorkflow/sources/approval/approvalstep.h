// MIT License
#ifndef VECTORPDF_APPROVALSTEP_H
#define VECTORPDF_APPROVALSTEP_H

#include "../workflowglobal.h"
#include <QString>
#include <QDateTime>
#include <QUuid>

namespace vectorpdf::workflow
{

enum class ApprovalDecision
{
    Pending,
    Approved,
    Rejected,
    ConditionallyApproved
};

struct VECTORPDF_WORKFLOW_EXPORT ApprovalStep
{
    QUuid stepId = QUuid::createUuid();
    QUuid approverId;
    QString approverName;
    int order = 1;
    ApprovalDecision decision = ApprovalDecision::Pending;
    QString comment;
    QDateTime decisionTimestamp;
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_APPROVALSTEP_H
