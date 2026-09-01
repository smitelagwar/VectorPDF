// MIT License
#ifndef VECTORPDF_PAGEDUPLICATEFINGERPRINT_H
#define VECTORPDF_PAGEDUPLICATEFINGERPRINT_H

#include "../scantoolsglobal.h"
#include <QString>
#include <QImage>

namespace vectorpdf::scan
{

class VECTORPDF_SCANTOOLS_EXPORT PageDuplicateFingerprint
{
public:
    static quint64 computeDifferenceHash(const QImage& image);
    static QString computeTextFingerprint(const QString& pageText);
    static int hammingDistance(quint64 hash1, quint64 hash2);
    static double calculateSimilarity(quint64 hash1, quint64 hash2);
};

} // namespace vectorpdf::scan

#endif // VECTORPDF_PAGEDUPLICATEFINGERPRINT_H
