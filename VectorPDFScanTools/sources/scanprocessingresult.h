// MIT License
#ifndef VECTORPDF_SCANPROCESSINGRESULT_H
#define VECTORPDF_SCANPROCESSINGRESULT_H

#include "scantoolsglobal.h"
#include "scanimage.h"

namespace vectorpdf::scan
{

struct VECTORPDF_SCANTOOLS_EXPORT ScanProcessingResult
{
    ScanImage processedImage;
    double detectedSkewAngle = 0.0;
    int detectedOrientationRotation = 0; // 0, 90, 180, 270
    int punchHolesRemoved = 0;
    bool success = true;
};

} // namespace vectorpdf::scan

#endif // VECTORPDF_SCANPROCESSINGRESULT_H
