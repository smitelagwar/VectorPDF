import os

def w(path, text):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "w", encoding="utf-8") as out:
        out.write(text.strip() + "\n")
    print("Created:", path)

# 1. Blank Page Detection
w("VectorPDFScanTools/sources/blankpage/blankpagefeatures.h", """// MIT License
#ifndef VECTORPDF_BLANKPAGEFEATURES_H
#define VECTORPDF_BLANKPAGEFEATURES_H

#include "../scantoolsglobal.h"

namespace vectorpdf::scan
{

struct VECTORPDF_SCANTOOLS_EXPORT BlankPageFeatures
{
    int pageIndex = 0;
    double darkPixelPercentage = 0.0;
    double variance = 0.0;
    double edgeDensity = 0.0;
    bool hasTextGliphs = false;
    bool isClassifiedBlank = false;
};

} // namespace vectorpdf::scan

#endif // VECTORPDF_BLANKPAGEFEATURES_H
""")

w("VectorPDFScanTools/sources/blankpage/blankpagepolicy.h", """// MIT License
#ifndef VECTORPDF_BLANKPAGEPOLICY_H
#define VECTORPDF_BLANKPAGEPOLICY_H

#include "../scantoolsglobal.h"

namespace vectorpdf::scan
{

struct VECTORPDF_SCANTOOLS_EXPORT BlankPagePolicy
{
    double maxDarkPixelThreshold = 0.5; // less than 0.5% dark pixels
    double maxEdgeDensityThreshold = 0.02;
    bool ignorePunchHoles = true;
    bool ignoreShowThroughNoise = true;
};

} // namespace vectorpdf::scan

#endif // VECTORPDF_BLANKPAGEPOLICY_H
""")

w("VectorPDFScanTools/sources/blankpage/blankpagepolicy.cpp", """// MIT License
#include "blankpagepolicy.h"

namespace vectorpdf::scan
{
}
""")

w("VectorPDFScanTools/sources/blankpage/blankpagedetector.h", """// MIT License
#ifndef VECTORPDF_BLANKPAGEDETECTOR_H
#define VECTORPDF_BLANKPAGEDETECTOR_H

#include "../scantoolsglobal.h"
#include "blankpagefeatures.h"
#include "blankpagepolicy.h"
#include <QImage>
#include <QList>

namespace pdf { class PDFDocument; }

namespace vectorpdf::scan
{

class VECTORPDF_SCANTOOLS_EXPORT BlankPageDetector
{
public:
    static BlankPageFeatures analyzeImage(const QImage& pageImage, int pageIndex, const BlankPagePolicy& policy);
    static QList<int> detectBlankPagesInDocument(const pdf::PDFDocument* document, const BlankPagePolicy& policy);
};

} // namespace vectorpdf::scan

#endif // VECTORPDF_BLANKPAGEDETECTOR_H
""")

w("VectorPDFScanTools/sources/blankpage/blankpagedetector.cpp", """// MIT License
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
""")

# 2. Duplicate Page Detection
w("VectorPDFScanTools/sources/duplicate/pageduplicatefingerprint.h", """// MIT License
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
""")

w("VectorPDFScanTools/sources/duplicate/pageduplicatefingerprint.cpp", """// MIT License
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
""")

w("VectorPDFScanTools/sources/duplicate/duplicatecluster.h", """// MIT License
#ifndef VECTORPDF_DUPLICATECLUSTER_H
#define VECTORPDF_DUPLICATECLUSTER_H

#include "../scantoolsglobal.h"
#include <QList>

namespace vectorpdf::scan
{

struct VECTORPDF_SCANTOOLS_EXPORT DuplicateCluster
{
    int primaryPageIndex = 0;
    QList<int> duplicatePageIndices;
    double similarityScore = 1.0;
};

} // namespace vectorpdf::scan

#endif // VECTORPDF_DUPLICATECLUSTER_H
""")

w("VectorPDFScanTools/sources/duplicate/duplicatecluster.cpp", """// MIT License
#include "duplicatecluster.h"

namespace vectorpdf::scan
{
}
""")

w("VectorPDFScanTools/sources/duplicate/duplicatepolicy.h", """// MIT License
#ifndef VECTORPDF_DUPLICATEPOLICY_H
#define VECTORPDF_DUPLICATEPOLICY_H

#include "../scantoolsglobal.h"

namespace vectorpdf::scan
{

struct VECTORPDF_SCANTOOLS_EXPORT DuplicatePolicy
{
    double minVisualSimilarity = 0.92; // 92% visual similarity
    bool requireExactTextMatch = false;
    bool compareAcrossAllPages = true;
};

} // namespace vectorpdf::scan

#endif // VECTORPDF_DUPLICATEPOLICY_H
""")

w("VectorPDFScanTools/sources/duplicate/duplicatepolicy.cpp", """// MIT License
#include "duplicatepolicy.h"

namespace vectorpdf::scan
{
}
""")

w("VectorPDFScanTools/sources/duplicate/duplicatepagedetector.h", """// MIT License
#ifndef VECTORPDF_DUPLICATEPAGEDETECTOR_H
#define VECTORPDF_DUPLICATEPAGEDETECTOR_H

#include "../scantoolsglobal.h"
#include "duplicatecluster.h"
#include "duplicatepolicy.h"
#include <QList>
#include <QImage>

namespace vectorpdf::scan
{

class VECTORPDF_SCANTOOLS_EXPORT DuplicatePageDetector
{
public:
    static QList<DuplicateCluster> findDuplicates(const QList<QImage>& pageImages, const DuplicatePolicy& policy);
};

} // namespace vectorpdf::scan

#endif // VECTORPDF_DUPLICATEPAGEDETECTOR_H
""")

w("VectorPDFScanTools/sources/duplicate/duplicatepagedetector.cpp", """// MIT License
#include "duplicatepagedetector.h"
#include "pageduplicatefingerprint.h"

namespace vectorpdf::scan
{

QList<DuplicateCluster> DuplicatePageDetector::findDuplicates(const QList<QImage>& pageImages, const DuplicatePolicy& policy)
{
    QList<DuplicateCluster> clusters;
    if (pageImages.size() < 2)
        return clusters;

    QList<quint64> hashes;
    for (const auto& img : pageImages)
    {
        hashes.append(PageDuplicateFingerprint::computeDifferenceHash(img));
    }

    QList<bool> visited(pageImages.size(), false);

    for (int i = 0; i < hashes.size(); ++i)
    {
        if (visited[i])
            continue;

        DuplicateCluster cluster;
        cluster.primaryPageIndex = i;

        for (int j = i + 1; j < hashes.size(); ++j)
        {
            if (visited[j])
                continue;

            double sim = PageDuplicateFingerprint::calculateSimilarity(hashes[i], hashes[j]);
            if (sim >= policy.minVisualSimilarity)
            {
                cluster.duplicatePageIndices.append(j);
                cluster.similarityScore = sim;
                visited[j] = true;
            }
        }

        if (!cluster.duplicatePageIndices.isEmpty())
        {
            visited[i] = true;
            clusters.append(cluster);
        }
    }

    return clusters;
}

} // namespace vectorpdf::scan
""")

w("VectorPDFScanTools/CMakeLists.txt", """# MIT License
# Copyright (c) 2026 VectorPDF Contributors

add_library(VectorPDFScanTools SHARED
    sources/scantoolsglobal.h
    sources/scanimage.h
    sources/scanimage.cpp
    sources/scanprocessingprofile.h
    sources/scanprocessingprofile.cpp
    sources/scanprocessingresult.h
    sources/scanprocessingpipeline.h
    sources/scanprocessingpipeline.cpp

    sources/cleanup/orientationdetector.h
    sources/cleanup/orientationdetector.cpp
    sources/cleanup/deskewprocessor.h
    sources/cleanup/deskewprocessor.cpp
    sources/cleanup/autocropdetector.h
    sources/cleanup/autocropdetector.cpp
    sources/cleanup/punchholeremover.h
    sources/cleanup/punchholeremover.cpp
    sources/cleanup/shadowremover.h
    sources/cleanup/shadowremover.cpp

    sources/blankpage/blankpagefeatures.h
    sources/blankpage/blankpagepolicy.h
    sources/blankpage/blankpagepolicy.cpp
    sources/blankpage/blankpagedetector.h
    sources/blankpage/blankpagedetector.cpp

    sources/duplicate/pageduplicatefingerprint.h
    sources/duplicate/pageduplicatefingerprint.cpp
    sources/duplicate/duplicatecluster.h
    sources/duplicate/duplicatecluster.cpp
    sources/duplicate/duplicatepolicy.h
    sources/duplicate/duplicatepolicy.cpp
    sources/duplicate/duplicatepagedetector.h
    sources/duplicate/duplicatepagedetector.cpp
)

add_compile_definitions(VECTORPDF_SCANTOOLS_LIBRARY)

target_link_libraries(VectorPDFScanTools PRIVATE
    Pdf4QtLibCore
    Qt6::Core
    Qt6::Gui
)

target_include_directories(VectorPDFScanTools INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/sources)
target_include_directories(VectorPDFScanTools PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/sources)

set_target_properties(VectorPDFScanTools PROPERTIES
                      VERSION ${VECTORPDF_VERSION}
                      SOVERSION ${VECTORPDF_VERSION}
                      LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PDF4QT_INSTALL_LIB_DIR}
                      RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PDF4QT_INSTALL_BIN_DIR})

install(TARGETS VectorPDFScanTools
        RUNTIME DESTINATION ${PDF4QT_INSTALL_BIN_DIR}
        LIBRARY DESTINATION ${PDF4QT_INSTALL_LIB_DIR})
""")

print("ScanTools Part 2 (Blank, Duplicate, CMake) generated successfully.")