// MIT License
#ifndef VECTORPDF_JAVASCRIPTREMOVER_H
#define VECTORPDF_JAVASCRIPTREMOVER_H

#include "../documenttoolsglobal.h"

namespace pdf { class PDFDocument; }

namespace vectorpdf::tools::sanitize
{

class VECTORPDF_DOCUMENTTOOLS_EXPORT JavaScriptRemover
{
public:
    static int removeAllJavaScript(pdf::PDFDocument* document);
};

} // namespace vectorpdf::tools::sanitize

#endif // VECTORPDF_JAVASCRIPTREMOVER_H
