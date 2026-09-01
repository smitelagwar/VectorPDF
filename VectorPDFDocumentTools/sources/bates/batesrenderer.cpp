// MIT License
#include "batesrenderer.h"

namespace vectorpdf::tools::bates
{

void BatesRenderer::renderBatesStamp(QPainter* painter, const QString& batesText, const BatesDefinition& def, const QRectF& pageBounds)
{
    if (!painter || pageBounds.isEmpty() || batesText.isEmpty())
        return;

    painter->save();
    painter->setFont(def.font);
    painter->setPen(def.textColor);

    QRectF targetRect = pageBounds.adjusted(def.marginPt, def.marginPt, -def.marginPt, -def.marginPt);
    Qt::Alignment align = Qt::AlignRight | Qt::AlignBottom;

    switch (def.position)
    {
    case BatesPosition::TopLeft: align = Qt::AlignLeft | Qt::AlignTop; break;
    case BatesPosition::TopCenter: align = Qt::AlignHCenter | Qt::AlignTop; break;
    case BatesPosition::TopRight: align = Qt::AlignRight | Qt::AlignTop; break;
    case BatesPosition::BottomLeft: align = Qt::AlignLeft | Qt::AlignBottom; break;
    case BatesPosition::BottomCenter: align = Qt::AlignHCenter | Qt::AlignBottom; break;
    case BatesPosition::BottomRight: align = Qt::AlignRight | Qt::AlignBottom; break;
    }

    painter->drawText(targetRect, align, batesText);
    painter->restore();
}

} // namespace vectorpdf::tools::bates
