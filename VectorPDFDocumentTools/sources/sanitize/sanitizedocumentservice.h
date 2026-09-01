// MIT License
#ifndef VECTORPDF_SANITIZEDOCUMENTSERVICE_H
#define VECTORPDF_SANITIZEDOCUMENTSERVICE_H

#include "../documenttoolsglobal.h"
#include "sanitizerule.h"
#include "sanitizeresult.h"

namespace pdf { class PDFDocument; }

namespace vectorpdf::tools::sanitize
{

class VECTORPDF_DOCUMENTTOOLS_EXPORT SanitizeDocumentService
{
public:
    static SanitizeReport sanitizeDocument(pdf::PDFDocument* document, const SanitizePolicy& policy);
};

} // namespace vectorpdf::tools::sanitize

#endif // VECTORPDF_SANITIZEDOCUMENTSERVICE_H
