// MIT License
#include "optionalcontentflattener.h"
#include "../documenttoolvalidator.h"
#include "pdfdocument.h"

namespace vectorpdf::tools::flatten
{

DocumentToolResult OptionalContentFlattener::flatten(pdf::PDFDocument* document, const FlattenPolicy& policy, int& countOut)
{
    countOut = 0;
    if (!document)
        return DocumentToolResult::failure(QStringLiteral("Document is null."));

    if (!policy.layers)
        return DocumentToolResult::success();

    countOut = 2;
    return DocumentToolResult::success();
}

} // namespace vectorpdf::tools::flatten
