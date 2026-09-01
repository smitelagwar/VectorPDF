// MIT License
#ifndef VECTORPDF_WORKFLOWPACKAGEREADER_H
#define VECTORPDF_WORKFLOWPACKAGEREADER_H

#include "../workflowglobal.h"
#include "../workflowtypes.h"
#include "../identity/workflowparticipant.h"
#include <QString>
#include <QList>

namespace vectorpdf::workflow
{

class VECTORPDF_WORKFLOW_EXPORT WorkflowPackageReader
{
public:
    static bool readPackage(const QString& packagePath, WorkflowMetadata& metaOut, QList<WorkflowParticipant>& participantsOut);
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_WORKFLOWPACKAGEREADER_H
