// MIT License
#include "sanitizevalidator.h"
#include "pdfdocument.h"

namespace vectorpdf::tools::sanitize
{

bool SanitizeValidator::verifySanitization(const pdf::PDFDocument* document)
{
    if (!document)
        return false;
    return document->getTitle().isEmpty() && document->getAuthor().isEmpty();
}

} // namespace vectorpdf::tools::sanitize
