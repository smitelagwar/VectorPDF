// MIT License
#ifndef VECTORPDF_PARTICIPANTRESOLVER_H
#define VECTORPDF_PARTICIPANTRESOLVER_H

#include "../workflowglobal.h"
#include "workflowparticipant.h"
#include <QList>

namespace vectorpdf::workflow
{

class VECTORPDF_WORKFLOW_EXPORT ParticipantResolver
{
public:
    static WorkflowParticipant currentLocalUser();
    static bool validateEmail(const QString& email);
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_PARTICIPANTRESOLVER_H
