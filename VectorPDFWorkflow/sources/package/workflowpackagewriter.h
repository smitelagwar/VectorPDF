// MIT License
#ifndef VECTORPDF_WORKFLOWPACKAGEWRITER_H
#define VECTORPDF_WORKFLOWPACKAGEWRITER_H

#include "../workflowglobal.h"
#include "../workflowtypes.h"
#include "../identity/workflowparticipant.h"
#include <QString>
#include <QList>

namespace vectorpdf::workflow
{

class VECTORPDF_WORKFLOW_EXPORT WorkflowPackageWriter
{
public:
    static bool writePackage(const QString& destinationPath, const QString& sourcePdfPath, const WorkflowMetadata& meta, const QList<WorkflowParticipant>& participants);
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_WORKFLOWPACKAGEWRITER_H
