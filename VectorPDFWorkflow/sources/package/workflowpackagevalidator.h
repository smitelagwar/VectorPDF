// MIT License
#ifndef VECTORPDF_WORKFLOWPACKAGEVALIDATOR_H
#define VECTORPDF_WORKFLOWPACKAGEVALIDATOR_H

#include "../workflowglobal.h"
#include <QString>

namespace vectorpdf::workflow
{

class VECTORPDF_WORKFLOW_EXPORT WorkflowPackageValidator
{
public:
    static bool validatePackage(const QString& packagePath);
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_WORKFLOWPACKAGEVALIDATOR_H
