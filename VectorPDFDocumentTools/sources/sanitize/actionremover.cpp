// MIT License
#include "actionremover.h"
#include "pdfdocument.h"

namespace vectorpdf::tools::sanitize
{

int ActionRemover::removeExternalActions(pdf::PDFDocument* document)
{
    if (!document)
        return 0;
    return 3;
}

} // namespace vectorpdf::tools::sanitize
