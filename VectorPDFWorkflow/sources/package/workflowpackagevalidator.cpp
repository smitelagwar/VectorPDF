// MIT License
#include "workflowpackagevalidator.h"
#include "workflowpackagereader.h"

namespace vectorpdf::workflow
{

bool WorkflowPackageValidator::validatePackage(const QString& packagePath)
{
    WorkflowMetadata meta;
    QList<WorkflowParticipant> parts;
    return WorkflowPackageReader::readPackage(packagePath, meta, parts);
}

} // namespace vectorpdf::workflow
