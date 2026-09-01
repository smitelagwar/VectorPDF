// MIT License
// Copyright (c) 2026 VectorPDF Contributors
#include "pagedecorationrenderer.h"
#include <QFontMetricsF>

namespace vectorpdf::tools::decorations
{

void PageDecorationRenderer::renderHeaderFooter(QPainter* painter, const HeaderFooterDefinition& def, const QRectF& pageBounds, const TemplateContext& context)
{
    if (!painter || pageBounds.isEmpty())
        return;

    painter->save();
    painter->setOpacity(def.opacity());

    QRectF printableRect = pageBounds.adjusted(def.margins.left(), def.margins.top(), -def.margins.right(), -def.margins.bottom());

    auto drawSlot = [&](const HeaderFooterSlotContent& slot, Qt::Alignment align, const QRectF& rect)
    {
        if (!slot.enabled || slot.templateText.isEmpty())
            return;

        QString evaluated = DecorationTemplateEngine::evaluate(slot.templateText, context);
        painter->setFont(slot.font);
        painter->setPen(slot.textColor);
        painter->drawText(rect, align | Qt::AlignVCenter, evaluated);
    };

    qreal headerHeight = 30.0;
    qreal footerHeight = 30.0;

    QRectF topRect(printableRect.left(), pageBounds.top() + def.margins.top() - headerHeight, printableRect.width(), headerHeight);
    QRectF bottomRect(printableRect.left(), pageBounds.bottom() - def.margins.bottom(), printableRect.width(), footerHeight);

    drawSlot(def.topLeft, Qt::AlignLeft, topRect);
    drawSlot(def.topCenter, Qt::AlignHCenter, topRect);
    drawSlot(def.topRight, Qt::AlignRight, topRect);

    drawSlot(def.bottomLeft, Qt::AlignLeft, bottomRect);
    drawSlot(def.bottomCenter, Qt::AlignHCenter, bottomRect);
    drawSlot(def.bottomRight, Qt::AlignRight, bottomRect);

    painter->restore();
}

void PageDecorationRenderer::renderWatermark(QPainter* painter, const WatermarkDefinition& def, const QRectF& pageBounds, const TemplateContext& context)
{
    if (!painter || pageBounds.isEmpty())
        return;

    painter->save();
    painter->setOpacity(def.opacity());

    QPointF center = pageBounds.center() + def.offset;
    painter->translate(center);
    painter->rotate(def.rotationAngle);
    painter->scale(def.scale, def.scale);

    if (def.kind == WatermarkKind::Text)
    {
        QString evalText = DecorationTemplateEngine::evaluate(def.text, context);
        painter->setFont(def.font);
        painter->setPen(def.textColor);
        QFontMetricsF fm(def.font);
        QRectF textRect = fm.boundingRect(evalText);
        painter->drawText(-textRect.width() / 2.0, fm.ascent() - textRect.height() / 2.0, evalText);
    }
    else if (def.kind == WatermarkKind::Image && !def.image.isNull())
    {
        QRectF imgRect(-def.image.width() / 2.0, -def.image.height() / 2.0, def.image.width(), def.image.height());
        painter->drawImage(imgRect, def.image);
    }

    painter->restore();
}

void PageDecorationRenderer::renderBackground(QPainter* painter, const BackgroundDefinition& def, const QRectF& pageBounds)
{
    if (!painter || pageBounds.isEmpty())
        return;

    painter->save();
    painter->setOpacity(def.opacity());

    if (def.kind == BackgroundKind::SolidColor)
    {
        painter->fillRect(pageBounds, def.solidColor);
    }
    else if (def.kind == BackgroundKind::Image && !def.image.isNull())
    {
        if (def.placement == BackgroundPlacement::Fill || def.placement == BackgroundPlacement::Stretch)
        {
            painter->drawImage(pageBounds, def.image);
        }
        else if (def.placement == BackgroundPlacement::Fit)
        {
            QSizeF scaledSize = def.image.size().scaled(pageBounds.size().toSize(), Qt::KeepAspectRatio);
            QRectF destRect((pageBounds.width() - scaledSize.width()) / 2.0, (pageBounds.height() - scaledSize.height()) / 2.0, scaledSize.width(), scaledSize.height());
            painter->drawImage(destRect, def.image);
        }
        else if (def.placement == BackgroundPlacement::Center)
        {
            QRectF destRect((pageBounds.width() - def.image.width()) / 2.0, (pageBounds.height() - def.image.height()) / 2.0, def.image.width(), def.image.height());
            painter->drawImage(destRect, def.image);
        }
        else if (def.placement == BackgroundPlacement::Tile)
        {
            painter->drawTiledPixmap(pageBounds, QPixmap::fromImage(def.image));
        }
    }

    painter->restore();
}

} // namespace vectorpdf::tools::decorations
