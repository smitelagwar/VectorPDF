// MIT License
//
// Copyright (c) 2026 VectorPDF Contributors
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "fillsignappearancebuilder.h"
#include <QFont>
#include <QFontMetricsF>

namespace vectorpdf::tools::fillsign
{

QImage FillSignAppearanceBuilder::renderToImage(const SignatureAsset& asset, const QSizeF& targetSize, qreal dpr)
{
    int w = qMax(1, static_cast<int>(targetSize.width() * dpr));
    int h = qMax(1, static_cast<int>(targetSize.height() * dpr));

    QImage img(w, h, QImage::Format_ARGB32_Premultiplied);
    img.fill(Qt::transparent);

    QPainter painter(&img);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    painter.scale(dpr, dpr);

    paintAsset(&painter, asset, QRectF(0, 0, targetSize.width(), targetSize.height()));
    painter.end();

    return img;
}

void FillSignAppearanceBuilder::paintAsset(QPainter* painter, const SignatureAsset& asset, const QRectF& targetRect)
{
    if (!painter || targetRect.isEmpty())
        return;

    painter->save();

    if (asset.type == SignatureAssetType::Drawn)
    {
        QRectF pathBounds = asset.vectorPath.boundingRect();
        if (!pathBounds.isEmpty())
        {
            qreal scaleX = targetRect.width() / pathBounds.width();
            qreal scaleY = targetRect.height() / pathBounds.height();
            qreal scale = qMin(scaleX, scaleY);

            painter->translate(targetRect.center());
            painter->scale(scale, scale);
            painter->translate(-pathBounds.center());

            QPen pen(asset.inkColor, asset.strokeWidth / scale, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
            painter->setPen(pen);
            painter->setBrush(Qt::NoBrush);
            painter->drawPath(asset.vectorPath);
        }
    }
    else if (asset.type == SignatureAssetType::Typed)
    {
        QFont font("Segoe Script", 24, QFont::Normal, true);
        if (asset.fontPresetName == QStringLiteral("Formal Cursive"))
        {
            font = QFont("Brush Script MT", 26, QFont::Normal, true);
        }
        else if (asset.fontPresetName == QStringLiteral("Casual Hand"))
        {
            font = QFont("Segoe Print", 22, QFont::Bold, false);
        }

        // Fit font to targetRect
        QFontMetricsF fm(font);
        qreal textWidth = fm.horizontalAdvance(asset.typedText);
        qreal textHeight = fm.height();
        if (textWidth > 0 && textHeight > 0)
        {
            qreal scaleX = (targetRect.width() * 0.9) / textWidth;
            qreal scaleY = (targetRect.height() * 0.9) / textHeight;
            qreal scale = qMin(scaleX, scaleY);

            painter->translate(targetRect.center());
            painter->scale(scale, scale);
            painter->translate(-textWidth / 2.0, fm.ascent() - textHeight / 2.0);

            painter->setFont(font);
            painter->setPen(asset.inkColor.isValid() ? asset.inkColor : QColor(10, 30, 80));
            painter->drawText(QPointF(0, 0), asset.typedText);
        }
    }
    else if (asset.type == SignatureAssetType::Image)
    {
        if (!asset.image.isNull())
        {
            painter->drawImage(targetRect, asset.image);
        }
    }

    painter->restore();
}

QPainterPath FillSignAppearanceBuilder::smoothPath(const QPainterPath& inputPath)
{
    // Return simplified path
    return inputPath.simplified();
}

QImage FillSignAppearanceBuilder::cropWhiteBackground(const QImage& sourceImage, int threshold)
{
    if (sourceImage.isNull())
        return sourceImage;

    QImage img = sourceImage.convertToFormat(QImage::Format_ARGB32);
    int top = img.height();
    int bottom = -1;
    int left = img.width();
    int right = -1;

    for (int y = 0; y < img.height(); ++y)
    {
        const QRgb* scanLine = reinterpret_cast<const QRgb*>(img.constScanLine(y));
        for (int x = 0; x < img.width(); ++x)
        {
            QRgb pixel = scanLine[x];
            int r = qRed(pixel);
            int g = qGreen(pixel);
            int b = qBlue(pixel);
            int a = qAlpha(pixel);

            bool isWhiteOrTransparent = (a < 20) || (r >= threshold && g >= threshold && b >= threshold);
            if (!isWhiteOrTransparent)
            {
                if (x < left) left = x;
                if (x > right) right = x;
                if (y < top) top = y;
                if (y > bottom) bottom = y;
            }
        }
    }

    if (left <= right && top <= bottom)
    {
        QRect cropRect(left, top, right - left + 1, bottom - top + 1);
        QImage cropped = img.copy(cropRect);

        // Turn near white pixels into transparent
        for (int y = 0; y < cropped.height(); ++y)
        {
            QRgb* scanLine = reinterpret_cast<QRgb*>(cropped.scanLine(y));
            for (int x = 0; x < cropped.width(); ++x)
            {
                QRgb pixel = scanLine[x];
                int r = qRed(pixel);
                int g = qGreen(pixel);
                int b = qBlue(pixel);
                if (r >= threshold && g >= threshold && b >= threshold)
                {
                    scanLine[x] = qRgba(r, g, b, 0);
                }
            }
        }
        return cropped;
    }

    return sourceImage;
}

} // namespace vectorpdf::tools::fillsign
