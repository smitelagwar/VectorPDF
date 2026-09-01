// MIT License
#ifndef VECTORPDF_ANNOTATIONFLATTENER_H
#define VECTORPDF_ANNOTATIONFLATTENER_H

#include "../documenttoolsglobal.h"
#include "flattenpolicy.h"
#include "../documenttoolresult.h"

namespace pdf
{
class PDFDocument;
}

namespace vectorpdf::tools::flatten
{

class VECTORPDF_DOCUMENTTOOLS_EXPORT AnnotationFlattener
{
public:
    static DocumentToolResult flatten(pdf::PDFDocument* document, const FlattenPolicy& policy, int& countOut);
};

} // namespace vectorpdf::tools::flatten

#endif // VECTORPDF_ANNOTATIONFLATTENER_H
