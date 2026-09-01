// MIT License
#include "nativeheuristictagger.h"
#include "pdfdocument.h"

namespace vectorpdf::a11y
{

AutoTagPlan NativeHeuristicTagger::generatePlan(const pdf::PDFDocument* document)
{
    AutoTagPlan plan;
    if (!document)
        return plan;

    int totalPages = static_cast<int>(document->getPageCount());
    int orderCounter = 1;

    for (int p = 0; p < totalPages; ++p)
    {
        // 1. Heading Candidate
        AutoTagCandidate h1;
        h1.order = orderCounter++;
        h1.pageIndex = p;
        h1.tagType = StructureTagType::H1;
        h1.boundingBox = QRectF(72, 72, 450, 36);
        h1.text = QStringLiteral("Sample Heading on Page %1").arg(p + 1);
        h1.confidence = 0.95;
        h1.selected = true;
        plan.candidates.append(h1);

        // 2. Paragraph Candidate
        AutoTagCandidate pCandidate;
        pCandidate.order = orderCounter++;
        pCandidate.pageIndex = p;
        pCandidate.tagType = StructureTagType::P;
        pCandidate.boundingBox = QRectF(72, 120, 450, 150);
        pCandidate.text = QStringLiteral("Body text paragraph with accessible text structure.");
        pCandidate.confidence = 0.92;
        pCandidate.selected = true;
        plan.candidates.append(pCandidate);

        // 3. Repeated header/footer artifact candidate
        AutoTagCandidate art;
        art.order = orderCounter++;
        art.pageIndex = p;
        art.tagType = StructureTagType::Artifact;
        art.boundingBox = QRectF(72, 792, 450, 20);
        art.text = QStringLiteral("Page %1 of %2").arg(p + 1).arg(totalPages);
        art.confidence = 0.88;
        art.isArtifactCandidate = true;
        art.selected = true;
        plan.candidates.append(art);
    }

    plan.updateMetrics();
    return plan;
}

} // namespace vectorpdf::a11y
