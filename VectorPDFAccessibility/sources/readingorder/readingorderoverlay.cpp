// MIT License
#include "readingorderoverlay.h"

namespace vectorpdf::a11y
{

void ReadingOrderOverlay::renderOverlay(QPainter* painter, const QList<ReadingBlock>& blocks, const QRectF& pageBounds)
{
    if (!painter || pageBounds.isEmpty())
        return;

    painter->save();
    for (const auto& b : blocks)
    {
        painter->setPen(QPen(QColor(0, 120, 215, 200), 2));
        painter->setBrush(QColor(0, 120, 215, 40));
        painter->drawRoundedRect(b.bounds, 4, 4);

        // Draw order badge
        QRectF badge(b.bounds.left(), b.bounds.top(), 24, 24);
        painter->setBrush(QColor(0, 120, 215, 240));
        painter->setPen(Qt::NoPen);
        painter->drawEllipse(badge);

        painter->setPen(Qt::white);
        painter->setFont(QFont("Segoe UI", 9, QFont::Bold));
        painter->drawText(badge, Qt::AlignCenter, QString::number(b.order));
    }
    painter->restore();
}

} // namespace vectorpdf::a11y
