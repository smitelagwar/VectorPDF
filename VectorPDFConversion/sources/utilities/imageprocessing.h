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

#ifndef VECTORPDF_IMAGEPROCESSING_H
#define VECTORPDF_IMAGEPROCESSING_H

#include "../conversionglobal.h"
#include "../conversiontypes.h"
#include <QImage>

namespace vectorpdf::conversion
{

class VECTORPDF_CONVERSION_EXPORT ImageProcessing
{
public:
    /// Converts any image to 8-bit Grayscale
    static QImage toGrayscale(const QImage& source);

    /// Calculates the optimal binarization threshold (0-255) using Otsu's method
    static int calculateOtsuThreshold(const QImage& grayscaleImage);

    /// Converts image to 1-bit monochrome using specified method
    static QImage toMonochrome(const QImage& source,
                               MonochromeThresholdMethod method = MonochromeThresholdMethod::Otsu,
                               int fixedThreshold = 128);

    /// Removes isolated noise pixels (despeckle filter)
    static QImage despeckle(const QImage& monoImage);

    /// Adjusts DPI metadata on image without altering pixel data
    static QImage setDpi(const QImage& source, int dpi);
};

} // namespace vectorpdf::conversion

#endif // VECTORPDF_IMAGEPROCESSING_H
