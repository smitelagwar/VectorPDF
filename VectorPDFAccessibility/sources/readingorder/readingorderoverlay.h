// MIT License
#ifndef VECTORPDF_READINGORDEROVERLAY_H
#define VECTORPDF_READINGORDEROVERLAY_H

#include "../accessibilityglobal.h"
#include "readingordermodel.h"
#include <QPainter>
#include <QRectF>

namespace vectorpdf::a11y
{

class VECTORPDF_ACCESSIBILITY_EXPORT ReadingOrderOverlay
{
public:
    static void renderOverlay(QPainter* painter, const QList<ReadingBlock>& blocks, const QRectF& pageBounds);
};

} // namespace vectorpdf::a11y

#endif // VECTORPDF_READINGORDEROVERLAY_H
