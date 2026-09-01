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

#include "imageprocessing.h"
#include <vector>
#include <cmath>

namespace vectorpdf::conversion
{

QImage ImageProcessing::toGrayscale(const QImage& source)
{
    if (source.isNull()) return QImage();
    return source.convertToFormat(QImage::Format_Grayscale8);
}

int ImageProcessing::calculateOtsuThreshold(const QImage& grayscaleImage)
{
    if (grayscaleImage.isNull()) return 128;

    QImage gray = grayscaleImage.format() == QImage::Format_Grayscale8
                      ? grayscaleImage
                      : grayscaleImage.convertToFormat(QImage::Format_Grayscale8);

    const int width = gray.width();
    const int height = gray.height();
    const int totalPixels = width * height;
    if (totalPixels <= 0) return 128;

    std::vector<int> histogram(256, 0);
    for (int y = 0; y < height; ++y)
    {
        const uchar* scanLine = gray.constScanLine(y);
        for (int x = 0; x < width; ++x)
        {
            histogram[scanLine[x]]++;
        }
    }

    double sum = 0;
    for (int t = 0; t < 256; ++t)
    {
        sum += t * histogram[t];
    }

    double sumB = 0;
    int wB = 0;
    int wF = 0;

    double varMax = 0;
    int threshold = 128;

    for (int t = 0; t < 256; ++t)
    {
        wB += histogram[t];
        if (wB == 0) continue;

        wF = totalPixels - wB;
        if (wF == 0) break;

        sumB += static_cast<double>(t * histogram[t]);

        double mB = sumB / wB;
        double mF = (sum - sumB) / wF;

        double varBetween = static_cast<double>(wB) * static_cast<double>(wF) * (mB - mF) * (mB - mF);

        if (varBetween > varMax)
        {
            varMax = varBetween;
            threshold = t;
        }
    }

    return threshold;
}

QImage ImageProcessing::toMonochrome(const QImage& source, MonochromeThresholdMethod method, int fixedThreshold)
{
    if (source.isNull()) return QImage();

    QImage gray = toGrayscale(source);
    int threshold = fixedThreshold;

    if (method == MonochromeThresholdMethod::Otsu)
    {
        threshold = calculateOtsuThreshold(gray);
    }
    else if (method == MonochromeThresholdMethod::Adaptive)
    {
        threshold = calculateOtsuThreshold(gray);
    }

    const int width = gray.width();
    const int height = gray.height();
    QImage mono(width, height, QImage::Format_Mono);
    mono.setColorCount(2);
    mono.setColor(0, qRgb(0, 0, 0));       // 0 = black
    mono.setColor(1, qRgb(255, 255, 255)); // 1 = white
    mono.fill(1);

    for (int y = 0; y < height; ++y)
    {
        const uchar* srcLine = gray.constScanLine(y);
        for (int x = 0; x < width; ++x)
        {
            if (srcLine[x] < threshold)
            {
                mono.setPixel(x, y, 0); // black
            }
            else
            {
                mono.setPixel(x, y, 1); // white
            }
        }
    }

    // Preserve DPI
    mono.setDotsPerMeterX(source.dotsPerMeterX());
    mono.setDotsPerMeterY(source.dotsPerMeterY());

    return mono;
}

QImage ImageProcessing::despeckle(const QImage& monoImage)
{
    if (monoImage.isNull() || monoImage.format() != QImage::Format_Mono)
        return monoImage;

    const int width = monoImage.width();
    const int height = monoImage.height();
    QImage result = monoImage;

    for (int y = 1; y < height - 1; ++y)
    {
        for (int x = 1; x < width - 1; ++x)
        {
            int current = monoImage.pixelIndex(x, y);
            int blackNeighbors = 0;

            for (int dy = -1; dy <= 1; ++dy)
            {
                for (int dx = -1; dx <= 1; ++dx)
                {
                    if (dx == 0 && dy == 0) continue;
                    if (monoImage.pixelIndex(x + dx, y + dy) == 0)
                        blackNeighbors++;
                }
            }

            // If an isolated black pixel has 0 or 1 black neighbors, flip to white
            if (current == 0 && blackNeighbors <= 1)
            {
                result.setPixel(x, y, 1);
            }
            // If an isolated white hole has 7 or 8 black neighbors, flip to black
            else if (current == 1 && blackNeighbors >= 7)
            {
                result.setPixel(x, y, 0);
            }
        }
    }

    return result;
}

QImage ImageProcessing::setDpi(const QImage& source, int dpi)
{
    if (source.isNull() || dpi <= 0) return source;

    QImage copy = source;
    const int dpm = static_cast<int>(std::round(dpi / 0.0254));
    copy.setDotsPerMeterX(dpm);
    copy.setDotsPerMeterY(dpm);
    return copy;
}

} // namespace vectorpdf::conversion
