// MIT License
#include "autocropdetector.h"

namespace vectorpdf::scan
{

QRect AutoCropDetector::detectContentBounds(const QImage& image, int margin)
{
    if (image.isNull())
        return QRect();

    QRect bounds(margin, margin, image.width() - 2 * margin, image.height() - 2 * margin);
    return bounds;
}

} // namespace vectorpdf::scan
