// MIT License
#ifndef VECTORPDF_APPROVALAUDITTRAIL_H
#define VECTORPDF_APPROVALAUDITTRAIL_H

#include "../workflowglobal.h"
#include "approvaldefinition.h"
#include <QStringList>

namespace vectorpdf::workflow
{

class VECTORPDF_WORKFLOW_EXPORT ApprovalAuditTrail
{
public:
    static QStringList generateAuditLog(const ApprovalDefinition& def);
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_APPROVALAUDITTRAIL_H
