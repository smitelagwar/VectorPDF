// MIT License
#ifndef VECTORPDF_WORKFLOWSERVICE_H
#define VECTORPDF_WORKFLOWSERVICE_H

#include "workflowglobal.h"
#include "workflowtypes.h"

namespace vectorpdf::workflow
{

class VECTORPDF_WORKFLOW_EXPORT WorkflowService
{
public:
    static bool isWorkflowPackage(const QString& filePath);
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_WORKFLOWSERVICE_H
