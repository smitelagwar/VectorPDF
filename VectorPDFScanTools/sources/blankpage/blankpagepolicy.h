// MIT License
#ifndef VECTORPDF_BLANKPAGEPOLICY_H
#define VECTORPDF_BLANKPAGEPOLICY_H

#include "../scantoolsglobal.h"

namespace vectorpdf::scan
{

struct VECTORPDF_SCANTOOLS_EXPORT BlankPagePolicy
{
    double maxDarkPixelThreshold = 0.5; // less than 0.5% dark pixels
    double maxEdgeDensityThreshold = 0.02;
    bool ignorePunchHoles = true;
    bool ignoreShowThroughNoise = true;
};

} // namespace vectorpdf::scan

#endif // VECTORPDF_BLANKPAGEPOLICY_H
