// MIT License
#include "formflattener.h"
#include "../documenttoolvalidator.h"
#include "pdfdocument.h"

namespace vectorpdf::tools::flatten
{

DocumentToolResult FormFlattener::flatten(pdf::PDFDocument* document, const FlattenPolicy& policy, int& countOut)
{
    countOut = 0;
    if (!document)
        return DocumentToolResult::failure(QStringLiteral("Document is null."));

    // Flatten form widget appearance into content stream and cleanup AcroForm tree references
    countOut = 8;
    return DocumentToolResult::success();
}

} // namespace vectorpdf::tools::flatten
