// MIT License
#ifndef VECTORPDF_SIGNRESPONSEIMPORTER_H
#define VECTORPDF_SIGNRESPONSEIMPORTER_H

#include "../workflowglobal.h"
#include "signrequestdefinition.h"

namespace vectorpdf::workflow
{

class VECTORPDF_WORKFLOW_EXPORT SignResponseImporter
{
public:
    static bool importResponse(SignRequestDefinition& def, const QUuid& participantId, const QString& signatureId);
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_SIGNRESPONSEIMPORTER_H
