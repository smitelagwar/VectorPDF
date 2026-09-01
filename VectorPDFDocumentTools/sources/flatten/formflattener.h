// MIT License
#ifndef VECTORPDF_FORMFLATTENER_H
#define VECTORPDF_FORMFLATTENER_H

#include "../documenttoolsglobal.h"
#include "flattenpolicy.h"
#include "../documenttoolresult.h"

namespace pdf
{
class PDFDocument;
}

namespace vectorpdf::tools::flatten
{

class VECTORPDF_DOCUMENTTOOLS_EXPORT FormFlattener
{
public:
    static DocumentToolResult flatten(pdf::PDFDocument* document, const FlattenPolicy& policy, int& countOut);
};

} // namespace vectorpdf::tools::flatten

#endif // VECTORPDF_FORMFLATTENER_H
