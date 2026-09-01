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

#ifndef VECTORPDF_DOCUMENTTOOLTYPES_H
#define VECTORPDF_DOCUMENTTOOLTYPES_H

#include "documenttoolsglobal.h"
#include <QString>
#include <QMetaType>
#include <QRectF>
#include <QSizeF>
#include <QPointF>
#include <QTransform>

namespace vectorpdf::tools
{

enum class ToolExecutionStatus
{
    Success,
    SuccessWithWarnings,
    Cancelled,
    Unsupported,
    ValidationFailed,
    Failed
};

enum class PageBoxType
{
    MediaBox,
    CropBox,
    BleedBox,
    TrimBox,
    ArtBox
};

enum class AlignmentHorizontal
{
    Left,
    Center,
    Right
};

enum class AlignmentVertical
{
    Top,
    Center,
    Bottom
};

class VECTORPDF_DOCUMENTTOOLS_EXPORT PageCoordinateHelper
{
public:
    static QTransform pageTransform(int rotationDegrees, const QRectF& pageBounds);
    static QPointF mapToRotatedPage(const QPointF& normalizedPoint, int rotationDegrees, const QRectF& pageBounds);
    static QRectF mapRectToRotatedPage(const QRectF& rect, int rotationDegrees, const QRectF& pageBounds);
    static QRectF resolvePageBox(const QRectF& mediaBox, const QRectF& cropBox, PageBoxType boxType = PageBoxType::CropBox);
};

} // namespace vectorpdf::tools

#endif // VECTORPDF_DOCUMENTTOOLTYPES_H
