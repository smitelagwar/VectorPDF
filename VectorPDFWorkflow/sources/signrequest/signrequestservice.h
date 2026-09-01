// MIT License
#ifndef VECTORPDF_SIGNREQUESTSERVICE_H
#define VECTORPDF_SIGNREQUESTSERVICE_H

#include "../workflowglobal.h"
#include "signrequestdefinition.h"
#include "signrequeststate.h"

namespace vectorpdf::workflow
{

class VECTORPDF_WORKFLOW_EXPORT SignRequestService
{
public:
    static bool initiateSignRequest(const SignRequestDefinition& def, const QString& destinationPackagePath);
    static SignRequestState evaluateState(const SignRequestDefinition& def);
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_SIGNREQUESTSERVICE_H
