// MIT License
#ifndef VECTORPDF_REVIEWDEFINITION_H
#define VECTORPDF_REVIEWDEFINITION_H

#include "../workflowglobal.h"
#include "../workflowtypes.h"
#include "../identity/workflowparticipant.h"
#include "reviewthread.h"
#include <QList>

namespace vectorpdf::workflow
{

struct VECTORPDF_WORKFLOW_EXPORT ReviewDefinition
{
    WorkflowMetadata metadata;
    QList<WorkflowParticipant> reviewers;
    QList<ReviewThread> threads;
    QString sharedStorageLocation;
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_REVIEWDEFINITION_H
