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

#include "documenttooltypes.h"

namespace vectorpdf::tools
{

QTransform PageCoordinateHelper::pageTransform(int rotationDegrees, const QRectF& pageBounds)
{
    QTransform transform;
    int normalizedRotation = ((rotationDegrees % 360) + 360) % 360;

    switch (normalizedRotation)
    {
    case 90:
        transform.translate(pageBounds.height(), 0.0);
        transform.rotate(90.0);
        break;
    case 180:
        transform.translate(pageBounds.width(), pageBounds.height());
        transform.rotate(180.0);
        break;
    case 270:
        transform.translate(0.0, pageBounds.width());
        transform.rotate(270.0);
        break;
    case 0:
    default:
        break;
    }

    return transform;
}

QPointF PageCoordinateHelper::mapToRotatedPage(const QPointF& normalizedPoint, int rotationDegrees, const QRectF& pageBounds)
{
    QTransform transform = pageTransform(rotationDegrees, pageBounds);
    return transform.map(normalizedPoint);
}

QRectF PageCoordinateHelper::mapRectToRotatedPage(const QRectF& rect, int rotationDegrees, const QRectF& pageBounds)
{
    QTransform transform = pageTransform(rotationDegrees, pageBounds);
    return transform.mapRect(rect);
}

QRectF PageCoordinateHelper::resolvePageBox(const QRectF& mediaBox, const QRectF& cropBox, PageBoxType boxType)
{
    if (boxType == PageBoxType::MediaBox || cropBox.isNull() || !cropBox.isValid())
    {
        return mediaBox.isValid() ? mediaBox : QRectF(0, 0, 595.276, 841.89); // Default A4
    }
    return cropBox;
}

} // namespace vectorpdf::tools
