// MIT License
#include "structuretreereader.h"
#include "pdfdocument.h"

namespace vectorpdf::a11y
{

bool StructureTreeReader::parseStructureTree(const pdf::PDFDocument* document, StructureNode* targetRoot)
{
    if (!document || !targetRoot)
        return false;

    targetRoot->type = StructureTagType::Document;
    targetRoot->title = document->getTitle().isEmpty() ? QStringLiteral("Document") : document->getTitle();
    return true;
}

} // namespace vectorpdf::a11y
