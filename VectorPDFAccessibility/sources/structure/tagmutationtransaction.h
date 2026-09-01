// MIT License
#ifndef VECTORPDF_TAGMUTATIONTRANSACTION_H
#define VECTORPDF_TAGMUTATIONTRANSACTION_H

#include "../accessibilityglobal.h"
#include <QString>

namespace pdf { class PDFDocument; }

namespace vectorpdf::a11y
{

class VECTORPDF_ACCESSIBILITY_EXPORT TagMutationTransaction
{
public:
    explicit TagMutationTransaction(const QString& actionName);
    bool execute(pdf::PDFDocument* document);
    bool rollback(pdf::PDFDocument* document);

private:
    QString m_actionName;
};

} // namespace vectorpdf::a11y

#endif // VECTORPDF_TAGMUTATIONTRANSACTION_H
