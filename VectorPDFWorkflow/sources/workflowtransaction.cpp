// MIT License
#include "workflowtransaction.h"

namespace vectorpdf::workflow
{

WorkflowTransaction::WorkflowTransaction(const QString& transactionName)
    : m_name(transactionName)
{
}

bool WorkflowTransaction::commit()
{
    return true;
}

bool WorkflowTransaction::rollback()
{
    return true;
}

} // namespace vectorpdf::workflow
