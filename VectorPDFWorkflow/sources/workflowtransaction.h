// MIT License
#ifndef VECTORPDF_WORKFLOWTRANSACTION_H
#define VECTORPDF_WORKFLOWTRANSACTION_H

#include "workflowglobal.h"
#include <QString>

namespace vectorpdf::workflow
{

class VECTORPDF_WORKFLOW_EXPORT WorkflowTransaction
{
public:
    explicit WorkflowTransaction(const QString& transactionName);
    bool commit();
    bool rollback();

private:
    QString m_name;
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_WORKFLOWTRANSACTION_H
