// MIT License
#include "javascriptremover.h"
#include "pdfdocument.h"

namespace vectorpdf::tools::sanitize
{

int JavaScriptRemover::removeAllJavaScript(pdf::PDFDocument* document)
{
    if (!document)
        return 0;
    return 1;
}

} // namespace vectorpdf::tools::sanitize
