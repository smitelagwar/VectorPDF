// MIT License
#include "signresponseimporter.h"

namespace vectorpdf::workflow
{

bool SignResponseImporter::importResponse(SignRequestDefinition& def, const QUuid& participantId, const QString& signatureId)
{
    for (auto& p : def.participants)
    {
        if (p.participantId == participantId)
        {
            p.hasCompleted = true;
            p.completedAt = QDateTime::currentDateTime();
            p.signatureId = signatureId;
            return true;
        }
    }
    return false;
}

} // namespace vectorpdf::workflow
