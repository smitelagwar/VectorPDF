// MIT License
#ifndef VECTORPDF_WORKFLOWPACKAGEMANIFEST_H
#define VECTORPDF_WORKFLOWPACKAGEMANIFEST_H

#include "../workflowglobal.h"
#include "../workflowtypes.h"
#include "../identity/workflowparticipant.h"
#include <QJsonObject>
#include <QList>

namespace vectorpdf::workflow
{

class VECTORPDF_WORKFLOW_EXPORT WorkflowPackageManifest
{
public:
    static QJsonObject serialize(const WorkflowMetadata& meta, const QList<WorkflowParticipant>& participants);
    static bool deserialize(const QJsonObject& json, WorkflowMetadata& metaOut, QList<WorkflowParticipant>& participantsOut);
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_WORKFLOWPACKAGEMANIFEST_H
