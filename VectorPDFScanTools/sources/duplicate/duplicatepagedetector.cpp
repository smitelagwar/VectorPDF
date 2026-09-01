// MIT License
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
