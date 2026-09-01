// MIT License
#ifndef VECTORPDF_FLATTENREPORT_H
#define VECTORPDF_FLATTENREPORT_H

#include "../documenttoolsglobal.h"
#include "../documenttoolresult.h"
#include <QString>

namespace vectorpdf::tools::flatten
{

struct VECTORPDF_DOCUMENTTOOLS_EXPORT FlattenReport
{
    DocumentToolResult result;
    int flattenedFormsCount = 0;
    int flattenedAnnotationsCount = 0;
    int flattenedStampsCount = 0;
    int preservedLinksCount = 0;
    int preservedSignaturesCount = 0;
};

} // namespace vectorpdf::tools::flatten

#endif // VECTORPDF_FLATTENREPORT_H
