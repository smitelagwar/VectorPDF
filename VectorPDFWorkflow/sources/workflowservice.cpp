// MIT License
#include "workflowservice.h"
#include "package/workflowpackagevalidator.h"

namespace vectorpdf::workflow
{

bool WorkflowService::isWorkflowPackage(const QString& filePath)
{
    return WorkflowPackageValidator::validatePackage(filePath);
}

} // namespace vectorpdf::workflow
