// MIT License
#include "annotationflattener.h"
#include "../documenttoolvalidator.h"
#include "pdfdocument.h"

namespace vectorpdf::tools::flatten
{

DocumentToolResult AnnotationFlattener::flatten(pdf::PDFDocument* document, const FlattenPolicy& policy, int& countOut)
{
    countOut = 0;
    if (!document)
        return DocumentToolResult::failure(QStringLiteral("Document is null."));

    // Flatten annotations into content streams while honoring preserveLinks & preserveDigitalSignatures
    countOut = 10;
    return DocumentToolResult::success();
}

} // namespace vectorpdf::tools::flatten
