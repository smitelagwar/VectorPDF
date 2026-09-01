// MIT License
#include "batesbatchplan.h"

namespace vectorpdf::tools::bates
{

void BatesBatchPlan::calculatePlan(const QList<int>& pageCounts)
{
    startEndNumbersByFile.clear();
    totalPages = 0;
    int currentNum = definition.sequence.currentRawNumber();

    for (int i = 0; i < targetFilePaths.size() && i < pageCounts.size(); ++i)
    {
        int pCount = pageCounts.at(i);
        BatesSequence tempSeq(QStringLiteral(""), currentNum, 6);
        QString startStr = tempSeq.currentNumber();
        for (int p = 0; p < pCount; ++p)
            tempSeq.nextNumber();
        BatesSequence endSeq(QStringLiteral(""), currentNum + pCount - 1, 6);
        QString endStr = endSeq.currentNumber();

        startEndNumbersByFile[targetFilePaths.at(i)] = startStr + QStringLiteral(" - ") + endStr;
        currentNum += pCount;
        totalPages += pCount;
    }
}

} // namespace vectorpdf::tools::bates
