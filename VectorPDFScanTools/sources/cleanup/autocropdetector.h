// MIT License
#ifndef VECTORPDF_AUTOCROPDETECTOR_H
#define VECTORPDF_AUTOCROPDETECTOR_H

#include "../scantoolsglobal.h"
#include <QImage>
#include <QRect>

namespace vectorpdf::scan
{

class VECTORPDF_SCANTOOLS_EXPORT AutoCropDetector
{
public:
    static QRect detectContentBounds(const QImage& image, int margin = 10);
};

} // namespace vectorpdf::scan

#endif // VECTORPDF_AUTOCROPDETECTOR_H
