// MIT License
#ifndef VECTORPDF_SANITIZEVALIDATOR_H
#define VECTORPDF_SANITIZEVALIDATOR_H

#include "../documenttoolsglobal.h"

namespace pdf { class PDFDocument; }

namespace vectorpdf::tools::sanitize
{

class VECTORPDF_DOCUMENTTOOLS_EXPORT SanitizeValidator
{
public:
    static bool verifySanitization(const pdf::PDFDocument* document);
};

} // namespace vectorpdf::tools::sanitize

#endif // VECTORPDF_SANITIZEVALIDATOR_H
