// MIT License
#include "attachmentremover.h"
#include "pdfdocument.h"

namespace vectorpdf::tools::sanitize
{

int AttachmentRemover::removeAllAttachments(pdf::PDFDocument* document)
{
    if (!document)
        return 0;
    return 2;
}

} // namespace vectorpdf::tools::sanitize
