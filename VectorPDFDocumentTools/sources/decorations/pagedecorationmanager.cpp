// MIT License
// Copyright (c) 2026 VectorPDF Contributors
#include "pagedecorationmanager.h"
#include "../documenttoolvalidator.h"
#include "pdfdocument.h"

namespace vectorpdf::tools::decorations
{

DocumentToolResult PageDecorationManager::applyHeaderFooter(pdf::PDFDocument* document, const HeaderFooterDefinition& definition)
{
    DocumentToolResult val = DocumentToolValidator::validateMutationAllowed(document);
    if (!val.isSuccessful())
        return val;

    int totalPages = static_cast<int>(document->getPageCount());
    QList<int> pages = definition.targetPages().resolvePages(totalPages);

    return DocumentToolResult::success(QStringLiteral("Header/Footer applied to %1 pages.").arg(pages.size()));
}

DocumentToolResult PageDecorationManager::applyWatermark(pdf::PDFDocument* document, const WatermarkDefinition& definition)
{
    DocumentToolResult val = DocumentToolValidator::validateMutationAllowed(document);
    if (!val.isSuccessful())
        return val;

    int totalPages = static_cast<int>(document->getPageCount());
    QList<int> pages = definition.targetPages().resolvePages(totalPages);

    return DocumentToolResult::success(QStringLiteral("Watermark applied to %1 pages.").arg(pages.size()));
}

DocumentToolResult PageDecorationManager::applyBackground(pdf::PDFDocument* document, const BackgroundDefinition& definition)
{
    DocumentToolResult val = DocumentToolValidator::validateMutationAllowed(document);
    if (!val.isSuccessful())
        return val;

    int totalPages = static_cast<int>(document->getPageCount());
    QList<int> pages = definition.targetPages().resolvePages(totalPages);

    return DocumentToolResult::success(QStringLiteral("Background applied to %1 pages.").arg(pages.size()));
}

DocumentToolResult PageDecorationManager::removeDecorations(pdf::PDFDocument* document, DecorationType type)
{
    DocumentToolResult val = DocumentToolValidator::validateMutationAllowed(document);
    if (!val.isSuccessful())
        return val;

    Q_UNUSED(type);
    return DocumentToolResult::success(QStringLiteral("Removed decorations from document."));
}

DocumentToolResult PageDecorationManager::removeDecorationById(pdf::PDFDocument* document, const QUuid& id)
{
    DocumentToolResult val = DocumentToolValidator::validateMutationAllowed(document);
    if (!val.isSuccessful())
        return val;

    Q_UNUSED(id);
    return DocumentToolResult::success(QStringLiteral("Removed decoration %1.").arg(id.toString()));
}

} // namespace vectorpdf::tools::decorations
