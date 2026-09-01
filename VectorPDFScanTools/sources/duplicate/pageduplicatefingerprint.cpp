// MIT License
#include "pageduplicatefingerprint.h"
#include <QCryptographicHash>

namespace vectorpdf::scan
{

quint64 PageDuplicateFingerprint::computeDifferenceHash(const QImage& image)
{
    if (image.isNull())
        return 0;

    QImage scaled = image.scaled(9, 8, Qt::IgnoreAspectRatio, Qt::SmoothTransformation).convertToFormat(QImage::Format_Grayscale8);
    quint64 hash = 0;

    for (int y = 0; y < 8; ++y)
    {
        for (int x = 0; x < 8; ++x)
        {
            int left = qGray(scaled.pixel(x, y));
            int right = qGray(scaled.pixel(x + 1, y));
            if (left > right)
            {
                hash |= (1ULL << (y * 8 + x));
            }
        }
    }

    return hash;
}

QString PageDuplicateFingerprint::computeTextFingerprint(const QString& pageText)
{
    QString simplified = pageText.simplified().toLower();
    QByteArray hashBytes = QCryptographicHash::hash(simplified.toUtf8(), QCryptographicHash::Sha256);
    return QString::fromLatin1(hashBytes.toHex());
}

int PageDuplicateFingerprint::hammingDistance(quint64 hash1, quint64 hash2)
{
    quint64 diff = hash1 ^ hash2;
    int dist = 0;
    while (diff > 0)
    {
        dist += (diff & 1);
        diff >>= 1;
    }
    return dist;
}

double PageDuplicateFingerprint::calculateSimilarity(quint64 hash1, quint64 hash2)
{
    int dist = hammingDistance(hash1, hash2);
    return (64.0 - dist) / 64.0;
}

} // namespace vectorpdf::scan
