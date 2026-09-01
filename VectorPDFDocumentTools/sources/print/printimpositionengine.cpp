// MIT License
#include "printimpositionengine.h"

namespace vectorpdf::tools::print
{

QList<QList<int>> PrintImpositionEngine::calculateNUpImposition(int totalPages, int nUpCount)
{
    QList<QList<int>> sheets;
    if (totalPages <= 0 || nUpCount <= 0)
        return sheets;

    QList<int> currentSheet;
    for (int p = 0; p < totalPages; ++p)
    {
        currentSheet.append(p);
        if (currentSheet.size() == nUpCount)
        {
            sheets.append(currentSheet);
            currentSheet.clear();
        }
    }
    if (!currentSheet.isEmpty())
    {
        sheets.append(currentSheet);
    }
    return sheets;
}

QList<QPair<int, int>> PrintImpositionEngine::calculateBookletImposition(int totalPages)
{
    QList<QPair<int, int>> sheets;
    if (totalPages <= 0)
        return sheets;

    // Pad total pages to multiple of 4
    int padded = (totalPages + 3) / 4 * 4;
    int left = 0;
    int right = padded - 1;

    while (left < right)
    {
        // Front side: right page, left page
        int p1 = (right < totalPages) ? right : -1;
        int p2 = (left < totalPages) ? left : -1;
        sheets.append({p1, p2});

        left++;
        right--;

        // Back side: left page, right page
        int p3 = (left < totalPages) ? left : -1;
        int p4 = (right < totalPages) ? right : -1;
        sheets.append({p3, p4});

        left++;
        right--;
    }

    return sheets;
}

} // namespace vectorpdf::tools::print
