// MIT License
#include "tagmutationtransaction.h"
#include "pdfdocument.h"

namespace vectorpdf::a11y
{

TagMutationTransaction::TagMutationTransaction(const QString& actionName)
    : m_actionName(actionName)
{
}

bool TagMutationTransaction::execute(pdf::PDFDocument* document)
{
    if (!document)
        return false;
    return true;
}

bool TagMutationTransaction::rollback(pdf::PDFDocument* document)
{
    if (!document)
        return false;
    return true;
}

} // namespace vectorpdf::a11y
