// MIT License
#include "signrequestservice.h"
#include "../package/workflowpackagewriter.h"

namespace vectorpdf::workflow
{

bool SignRequestService::initiateSignRequest(const SignRequestDefinition& def, const QString& destinationPackagePath)
{
    return WorkflowPackageWriter::writePackage(destinationPackagePath, QString(), def.metadata, def.participants);
}

SignRequestState SignRequestService::evaluateState(const SignRequestDefinition& def)
{
    SignRequestState state;
    state.status = def.metadata.status;
    state.totalFields = def.fields.size();
    state.totalParticipants = def.participants.size();

    for (const auto& f : def.fields)
    {
        if (f.isFilled)
            state.filledFields++;
    }

    for (const auto& p : def.participants)
    {
        if (p.hasCompleted)
            state.completedParticipants++;
    }

    return state;
}

} // namespace vectorpdf::workflow
