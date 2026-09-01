// MIT License
#ifndef VECTORPDF_BLANKPAGEFEATURES_H
#define VECTORPDF_BLANKPAGEFEATURES_H

#include "../scantoolsglobal.h"

namespace vectorpdf::scan
{

struct VECTORPDF_SCANTOOLS_EXPORT BlankPageFeatures
{
    int pageIndex = 0;
    double darkPixelPercentage = 0.0;
    double variance = 0.0;
    double edgeDensity = 0.0;
    bool hasTextGliphs = false;
    bool isClassifiedBlank = false;
};

} // namespace vectorpdf::scan

#endif // VECTORPDF_BLANKPAGEFEATURES_H
