// MIT License
// Copyright (c) 2026 VectorPDF Contributors
#ifndef VECTORPDF_FINDREPLACETRANSACTION_H
#define VECTORPDF_FINDREPLACETRANSACTION_H

#include "../documenttoolsglobal.h"
#include "../documenttooltransaction.h"
#include "findreplacematch.h"
#include <QList>

namespace vectorpdf::tools::content
{

class VECTORPDF_DOCUMENTTOOLS_EXPORT FindReplaceTransaction : public DocumentToolTransaction
{
public:
    explicit FindReplaceTransaction(const QList<FindReplaceMatch>& matchesToApply);

    DocumentToolResult execute(pdf::PDFDocument* targetDocument) override;
    DocumentToolResult rollback(pdf::PDFDocument* targetDocument) override;

private:
    QList<FindReplaceMatch> m_matches;
};

} // namespace vectorpdf::tools::content

#endif // VECTORPDF_FINDREPLACETRANSACTION_H
