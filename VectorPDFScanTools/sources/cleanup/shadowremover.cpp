// MIT License
#include "shadowremover.h"

namespace vectorpdf::scan
{

QImage ShadowRemover::removeBorderShadows(const QImage& image)
{
    if (image.isNull())
        return image;
    return image;
}

} // namespace vectorpdf::scan
