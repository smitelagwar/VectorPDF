// MIT License
#ifndef VECTORPDF_PRINTSERVICE_H
#define VECTORPDF_PRINTSERVICE_H

#include "../documenttoolsglobal.h"
#include "printjobdefinition.h"

namespace pdf { class PDFDocument; }

namespace vectorpdf::tools::print
{

class VECTORPDF_DOCUMENTTOOLS_EXPORT PrintService
{
public:
    static bool executePrintJob(const pdf::PDFDocument* document, const PrintJobDefinition& job);
};

} // namespace vectorpdf::tools::print

#endif // VECTORPDF_PRINTSERVICE_H
