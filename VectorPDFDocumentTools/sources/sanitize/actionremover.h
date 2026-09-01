// MIT License
#ifndef VECTORPDF_ACTIONREMOVER_H
#define VECTORPDF_ACTIONREMOVER_H

#include "../documenttoolsglobal.h"

namespace pdf { class PDFDocument; }

namespace vectorpdf::tools::sanitize
{

class VECTORPDF_DOCUMENTTOOLS_EXPORT ActionRemover
{
public:
    static int removeExternalActions(pdf::PDFDocument* document);
};

} // namespace vectorpdf::tools::sanitize

#endif // VECTORPDF_ACTIONREMOVER_H
