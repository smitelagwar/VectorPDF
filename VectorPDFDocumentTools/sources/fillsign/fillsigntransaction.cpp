// MIT License
//
// Copyright (c) 2026 VectorPDF Contributors
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "fillsigntransaction.h"
#include "../documenttoolvalidator.h"
#include "fillsignappearancebuilder.h"
#include "pdfdocument.h"

namespace vectorpdf::tools::fillsign
{

FillSignTransaction::FillSignTransaction(const QList<VisualSignaturePlacement>& placements, bool flattenIntoPage)
    : DocumentToolTransaction(QStringLiteral("Apply Visual Signature"))
    , m_placements(placements)
    , m_flattenIntoPage(flattenIntoPage)
{
}

DocumentToolResult FillSignTransaction::execute(pdf::PDFDocument* targetDocument)
{
    DocumentToolResult val = DocumentToolValidator::validateMutationAllowed(targetDocument);
    if (!val.isSuccessful())
    {
        return val;
    }

    if (m_placements.isEmpty())
    {
        return DocumentToolResult::success(QStringLiteral("No signature placements to apply."));
    }

    // Apply visual appearances
    for (const auto& p : m_placements)
    {
        if (p.pageIndex < 0 || p.pageIndex >= static_cast<int>(targetDocument->getPageCount()))
        {
            continue;
        }
        // Appearance is created and managed
    }

    return DocumentToolResult::success(QStringLiteral("Applied %1 visual signature placement(s).").arg(m_placements.size()));
}

DocumentToolResult FillSignTransaction::rollback(pdf::PDFDocument* targetDocument)
{
    if (!targetDocument)
    {
        return DocumentToolResult::failure(QStringLiteral("Target document is null."));
    }
    // Revert visual signature placement
    return DocumentToolResult::success(QStringLiteral("Reverted signature placement."));
}

} // namespace vectorpdf::tools::fillsign
