// MIT License
#ifndef VECTORPDF_METADATAREMOVER_H
#define VECTORPDF_METADATAREMOVER_H

#include "../documenttoolsglobal.h"

namespace pdf { class PDFDocument; }

namespace vectorpdf::tools::sanitize
{

class VECTORPDF_DOCUMENTTOOLS_EXPORT MetadataRemover
{
public:
    static int removeAllMetadata(pdf::PDFDocument* document);
};

} // namespace vectorpdf::tools::sanitize

#endif // VECTORPDF_METADATAREMOVER_H
