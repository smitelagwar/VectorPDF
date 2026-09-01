// MIT License
#ifndef VECTORPDF_FLATTENENGINE_H
#define VECTORPDF_FLATTENENGINE_H

#include "../documenttoolsglobal.h"
#include "flattenpolicy.h"
#include "flattenplan.h"
#include "flattenreport.h"

namespace pdf
{
class PDFDocument;
}

namespace vectorpdf::tools::flatten
{

class VECTORPDF_DOCUMENTTOOLS_EXPORT FlattenEngine
{
public:
    static FlattenPlan analyze(const pdf::PDFDocument* document, const FlattenPolicy& policy);
    static FlattenReport execute(pdf::PDFDocument* document, const FlattenPolicy& policy);
};

} // namespace vectorpdf::tools::flatten

#endif // VECTORPDF_FLATTENENGINE_H
