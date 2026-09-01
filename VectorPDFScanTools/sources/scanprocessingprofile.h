// MIT License
#ifndef VECTORPDF_SCANPROCESSINGPROFILE_H
#define VECTORPDF_SCANPROCESSINGPROFILE_H

#include "scantoolsglobal.h"

namespace vectorpdf::scan
{

struct VECTORPDF_SCANTOOLS_EXPORT ScanProcessingProfile
{
    bool autoRotate = true;
    bool autoDeskew = true;
    bool autoCrop = true;
    bool backgroundNormalization = true;
    bool denoise = true;
    bool removePunchHoles = true;
    bool removeShadows = true;
    double contrastBoost = 1.2;
    int binarizationThreshold = 180; // 0 for automatic Otsu
};

} // namespace vectorpdf::scan

#endif // VECTORPDF_SCANPROCESSINGPROFILE_H
