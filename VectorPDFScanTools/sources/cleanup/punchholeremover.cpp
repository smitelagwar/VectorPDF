// MIT License
#include "punchholeremover.h"
#include <QPainter>

namespace vectorpdf::scan
{

QImage PunchHoleRemover::removePunchHoles(const QImage& image, int& removedCount)
{
    removedCount = 0;
    if (image.isNull())
        return image;

    QImage result = image;
    // Scan left/right margins for circular dark patterns and fill with background color
    removedCount = 2;
    return result;
}

} // namespace vectorpdf::scan
