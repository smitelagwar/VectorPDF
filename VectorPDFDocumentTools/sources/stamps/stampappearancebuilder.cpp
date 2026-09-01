// MIT License
#include "stampappearancebuilder.h"
#include <QDateTime>

namespace vectorpdf::tools::stamps
{

void StampAppearanceBuilder::paintStamp(QPainter* painter, const StampDefinition& def, const QRectF& targetRect)
{
    if (!painter || targetRect.isEmpty())
        return;

    painter->save();
    painter->setOpacity(def.opacity);
    painter->translate(targetRect.center());
    painter->rotate(def.rotationAngle);

    QRectF box(-targetRect.width() / 2.0, -targetRect.height() / 2.0, targetRect.width(), targetRect.height());

    // Draw border
    QPen pen(def.borderColor, 3);
    painter->setPen(pen);
    painter->setBrush(QColor(def.primaryColor.red(), def.primaryColor.green(), def.primaryColor.blue(), 25));
    painter->drawRoundedRect(box, 8, 8);

    // Draw main text
    painter->setFont(def.font);
    painter->setPen(def.primaryColor);

    if (def.renderType == StampRenderType::DynamicTimestamped)
    {
        QRectF topBox(box.left(), box.top() + 6, box.width(), box.height() * 0.55);
        QRectF bottomBox(box.left(), box.top() + box.height() * 0.55, box.width(), box.height() * 0.40);
        painter->drawText(topBox, Qt::AlignCenter, def.mainText);

        QFont subFont("Segoe UI", 9);
        painter->setFont(subFont);
        QString dateStr = QDateTime::currentDateTime().toString(QStringLiteral("dd.MM.yyyy hh:mm"));
        painter->drawText(bottomBox, Qt::AlignCenter, dateStr);
    }
    else
    {
        painter->drawText(box, Qt::AlignCenter, def.mainText);
    }

    painter->restore();
}

QImage StampAppearanceBuilder::renderStampToImage(const StampDefinition& def, qreal dpr)
{
    QSize pxSize = def.preferredSize * dpr;
    QImage img(pxSize, QImage::Format_ARGB32_Premultiplied);
    img.fill(Qt::transparent);

    QPainter painter(&img);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.scale(dpr, dpr);
    paintStamp(&painter, def, QRectF(0, 0, def.preferredSize.width(), def.preferredSize.height()));
    painter.end();

    img.setDevicePixelRatio(dpr);
    return img;
}

} // namespace vectorpdf::tools::stamps
