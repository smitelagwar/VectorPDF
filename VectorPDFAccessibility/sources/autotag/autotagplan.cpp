// MIT License
#include "autotagplan.h"

namespace vectorpdf::a11y
{

void AutoTagPlan::updateMetrics()
{
    headingCount = 0;
    paragraphCount = 0;
    listCount = 0;
    tableCount = 0;
    figureCount = 0;
    artifactCount = 0;
    requiresUserReview = false;

    for (const auto& c : candidates)
    {
        if (c.isLowConfidence)
            requiresUserReview = true;

        if (c.isArtifactCandidate)
        {
            artifactCount++;
            continue;
        }

        switch (c.tagType)
        {
        case StructureTagType::H1:
        case StructureTagType::H2:
        case StructureTagType::H3:
        case StructureTagType::H4:
        case StructureTagType::H5:
        case StructureTagType::H6:
            headingCount++;
            break;
        case StructureTagType::P:
            paragraphCount++;
            break;
        case StructureTagType::L:
        case StructureTagType::LI:
            listCount++;
            break;
        case StructureTagType::Table:
        case StructureTagType::TR:
        case StructureTagType::TH:
        case StructureTagType::TD:
            tableCount++;
            break;
        case StructureTagType::Figure:
            figureCount++;
            break;
        default:
            break;
        }
    }
}

} // namespace vectorpdf::a11y
