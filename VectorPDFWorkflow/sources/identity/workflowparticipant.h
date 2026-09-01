// MIT License
#ifndef VECTORPDF_WORKFLOWPARTICIPANT_H
#define VECTORPDF_WORKFLOWPARTICIPANT_H

#include "../workflowglobal.h"
#include "../workflowtypes.h"
#include <QString>
#include <QUuid>

namespace vectorpdf::workflow
{

struct VECTORPDF_WORKFLOW_EXPORT WorkflowParticipant
{
    QUuid participantId = QUuid::createUuid();
    QString email;
    QString name;
    ParticipantRole role = ParticipantRole::Signer;
    int sequenceOrder = 1;
    bool hasCompleted = false;
    QDateTime completedAt;
    QString signatureId;
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_WORKFLOWPARTICIPANT_H
