// MIT License
#ifndef VECTORPDF_ORIENTATIONDETECTOR_H
#define VECTORPDF_ORIENTATIONDETECTOR_H

#include "../scantoolsglobal.h"
#include <QImage>

namespace vectorpdf::scan
{

class VECTORPDF_SCANTOOLS_EXPORT OrientationDetector
{
public:
    static int detectOrientation(const QImage& image);
};

} // namespace vectorpdf::scan

#endif // VECTORPDF_ORIENTATIONDETECTOR_H
