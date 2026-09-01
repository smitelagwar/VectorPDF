// MIT License
#ifndef VECTORPDF_SIGNREQUESTDEFINITION_H
#define VECTORPDF_SIGNREQUESTDEFINITION_H

#include "../workflowglobal.h"
#include "../workflowtypes.h"
#include "../identity/workflowparticipant.h"
#include "signrequestfield.h"
#include <QList>

namespace vectorpdf::workflow
{

struct VECTORPDF_WORKFLOW_EXPORT SignRequestDefinition
{
    WorkflowMetadata metadata;
    QList<WorkflowParticipant> participants;
    QList<SignRequestField> fields;
    QString messageForRecipients;
    bool sequentialSigning = true;
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_SIGNREQUESTDEFINITION_H
