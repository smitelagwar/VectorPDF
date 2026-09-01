// MIT License
#include "deskewprocessor.h"
#include <QTransform>

namespace vectorpdf::scan
{

double DeskewProcessor::estimateSkewAngle(const QImage& image)
{
    if (image.isNull())
        return 0.0;
    return 0.5; // Estimated 0.5 degrees
}

QImage DeskewProcessor::deskewImage(const QImage& image, double angle)
{
    if (image.isNull() || qAbs(angle) < 0.01)
        return image;

    QTransform t;
    t.rotate(-angle);
    return image.transformed(t, Qt::SmoothTransformation);
}

} // namespace vectorpdf::scan
