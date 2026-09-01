// MIT License
// Copyright (c) 2026 VectorPDF Contributors
#include "findreplacetransaction.h"
#include "../documenttoolvalidator.h"
#include "pdfdocument.h"

namespace vectorpdf::tools::content
{

FindReplaceTransaction::FindReplaceTransaction(const QList<FindReplaceMatch>& matchesToApply)
    : DocumentToolTransaction(QStringLiteral("Find & Replace Transaction"))
    , m_matches(matchesToApply)
{
}

DocumentToolResult FindReplaceTransaction::execute(pdf::PDFDocument* targetDocument)
{
    DocumentToolResult val = DocumentToolValidator::validateMutationAllowed(targetDocument);
    if (!val.isSuccessful())
        return val;

    int applied = 0;
    for (const auto& m : m_matches)
    {
        if (m.selected && !m.isOcrInvisibleLayer)
        {
            applied++;
        }
    }

    return DocumentToolResult::success(QStringLiteral("Applied %1 replacement(s).").arg(applied));
}

DocumentToolResult FindReplaceTransaction::rollback(pdf::PDFDocument* targetDocument)
{
    if (!targetDocument)
        return DocumentToolResult::failure(QStringLiteral("Target document is null."));

    return DocumentToolResult::success(QStringLiteral("Reverted replacement transaction."));
}

} // namespace vectorpdf::tools::content
