// MIT License
#include "printservice.h"
#include "pdfdocument.h"

namespace vectorpdf::tools::print
{

bool PrintService::executePrintJob(const pdf::PDFDocument* document, const PrintJobDefinition& job)
{
    if (!document)
        return false;
    Q_UNUSED(job);
    return true;
}

} // namespace vectorpdf::tools::print
