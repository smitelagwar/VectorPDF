// MIT License
#ifndef VECTORPDF_ATTACHMENTREMOVER_H
#define VECTORPDF_ATTACHMENTREMOVER_H

#include "../documenttoolsglobal.h"

namespace pdf { class PDFDocument; }

namespace vectorpdf::tools::sanitize
{

class VECTORPDF_DOCUMENTTOOLS_EXPORT AttachmentRemover
{
public:
    static int removeAllAttachments(pdf::PDFDocument* document);
};

} // namespace vectorpdf::tools::sanitize

#endif // VECTORPDF_ATTACHMENTREMOVER_H
