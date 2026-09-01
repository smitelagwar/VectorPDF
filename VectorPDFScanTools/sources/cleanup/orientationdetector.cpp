// MIT License
#include "orientationdetector.h"

namespace vectorpdf::scan
{

int OrientationDetector::detectOrientation(const QImage& image)
{
    if (image.isNull())
        return 0;

    // Detect if page text runs upright, rotated 90, 180, or 270
    return 0;
}

} // namespace vectorpdf::scan
