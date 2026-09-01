// MIT License
#ifndef VECTORPDF_SCANPROCESSINGPIPELINE_H
#define VECTORPDF_SCANPROCESSINGPIPELINE_H

#include "scantoolsglobal.h"
#include "scanimage.h"
#include "scanprocessingprofile.h"
#include "scanprocessingresult.h"

namespace vectorpdf::scan
{

class VECTORPDF_SCANTOOLS_EXPORT ScanProcessingPipeline
{
public:
    static ScanProcessingResult process(const ScanImage& inputImage, const ScanProcessingProfile& profile);
};

} // namespace vectorpdf::scan

#endif // VECTORPDF_SCANPROCESSINGPIPELINE_H
