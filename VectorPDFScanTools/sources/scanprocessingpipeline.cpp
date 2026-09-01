// MIT License
#include "scanprocessingpipeline.h"
#include "cleanup/orientationdetector.h"
#include "cleanup/deskewprocessor.h"
#include "cleanup/autocropdetector.h"
#include "cleanup/punchholeremover.h"
#include "cleanup/shadowremover.h"

namespace vectorpdf::scan
{

ScanProcessingResult ScanProcessingPipeline::process(const ScanImage& inputImage, const ScanProcessingProfile& profile)
{
    ScanProcessingResult result;
    if (inputImage.isNull())
    {
        result.success = false;
        return result;
    }

    QImage img = inputImage.image();

    if (profile.autoRotate)
    {
        result.detectedOrientationRotation = OrientationDetector::detectOrientation(img);
    }

    if (profile.autoDeskew)
    {
        result.detectedSkewAngle = DeskewProcessor::estimateSkewAngle(img);
        img = DeskewProcessor::deskewImage(img, result.detectedSkewAngle);
    }

    if (profile.removePunchHoles)
    {
        int count = 0;
        img = PunchHoleRemover::removePunchHoles(img, count);
        result.punchHolesRemoved = count;
    }

    if (profile.removeShadows)
    {
        img = ShadowRemover::removeBorderShadows(img);
    }

    result.processedImage.setImage(img);
    result.success = true;
    return result;
}

} // namespace vectorpdf::scan
