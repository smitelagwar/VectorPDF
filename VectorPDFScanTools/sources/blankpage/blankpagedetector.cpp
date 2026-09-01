// MIT License
#include "blankpagedetector.h"
#include "pdfdocument.h"

namespace vectorpdf::scan
{

BlankPageFeatures BlankPageDetector::analyzeImage(const QImage& pageImage, int pageIndex, const BlankPagePolicy& policy)
{
    BlankPageFeatures feat;
    feat.pageIndex = pageIndex;
    if (pageImage.isNull())
    {
        feat.isClassifiedBlank = true;
        return feat;
    }

    // Sample pixels across image to measure darkness/variance
    qint64 darkCount = 0;
    qint64 totalSampled = 0;

    int step = 4;
    for (int y = 0; y < pageImage.height(); y += step)
    {
        for (int x = 0; x < pageImage.width(); x += step)
        {
            QRgb rgb = pageImage.pixel(x, y);
            int gray = qGray(rgb);
            if (gray < 200)
            {
                darkCount++;
            }
            totalSampled++;
        }
    }

    feat.darkPixelPercentage = (totalSampled > 0) ? (static_cast<double>(darkCount) / totalSampled) * 100.0 : 0.0;
    feat.isClassifiedBlank = (feat.darkPixelPercentage < policy.maxDarkPixelThreshold);
    return feat;
}

QList<int> BlankPageDetector::detectBlankPagesInDocument(const pdf::PDFDocument* document, const BlankPagePolicy& policy)
{
    QList<int> blankPages;
    if (!document)
        return blankPages;

    Q_UNUSED(policy);
    return blankPages;
}

} // namespace vectorpdf::scan
