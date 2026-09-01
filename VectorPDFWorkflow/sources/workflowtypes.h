// MIT License
#ifndef VECTORPDF_WORKFLOWTYPES_H
#define VECTORPDF_WORKFLOWTYPES_H

#include "workflowglobal.h"
#include <QString>
#include <QDateTime>
#include <QUuid>

namespace vectorpdf::workflow
{

enum class WorkflowType
{
    SignatureRequest,
    SharedReview,
    ApprovalChain
};

enum class WorkflowStatus
{
    Draft,
    Active,
    Pending,
    Completed,
    Rejected,
    Expired,
    Cancelled
};

enum class ParticipantRole
{
    Signer,
    Reviewer,
    Approver,
    Observer
};

struct VECTORPDF_WORKFLOW_EXPORT WorkflowMetadata
{
    QUuid workflowId = QUuid::createUuid();
    WorkflowType type = WorkflowType::SignatureRequest;
    QString title;
    QString initiatorEmail;
    QString initiatorName;
    QDateTime createdAt = QDateTime::currentDateTime();
    QDateTime dueDate;
    WorkflowStatus status = WorkflowStatus::Draft;
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_WORKFLOWTYPES_H
