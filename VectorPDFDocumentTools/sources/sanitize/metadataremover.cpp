// MIT License
#include "metadataremover.h"
#include "pdfdocument.h"

namespace vectorpdf::tools::sanitize
{

int MetadataRemover::removeAllMetadata(pdf::PDFDocument* document)
{
    if (!document)
        return 0;

    document->setTitle(QString());
    document->setAuthor(QString());
    document->setSubject(QString());
    document->setKeywords(QString());
    document->setCreator(QString());
    document->setProducer(QString());
    return 6;
}

} // namespace vectorpdf::tools::sanitize
