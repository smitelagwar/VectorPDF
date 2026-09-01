// MIT License
#include "readingorderanalyzer.h"
#include "pdfdocument.h"

namespace vectorpdf::a11y
{

QList<ReadingBlock> ReadingOrderAnalyzer::analyzeDocument(const pdf::PDFDocument* document)
{
    QList<ReadingBlock> blocks;
    if (!document)
        return blocks;

    int totalPages = static_cast<int>(document->getPageCount());
    for (int p = 0; p < totalPages; ++p)
    {
        ReadingBlock b1;
        b1.order = 1;
        b1.pageIndex = p;
        b1.bounds = QRectF(72, 72, 450, 40);
        b1.textSnippet = QStringLiteral("Heading on page %1").arg(p + 1);
        b1.tagTypeName = QStringLiteral("H1");
        blocks.append(b1);

        ReadingBlock b2;
        b2.order = 2;
        b2.pageIndex = p;
        b2.bounds = QRectF(72, 120, 450, 200);
        b2.textSnippet = QStringLiteral("Paragraph text content on page %1").arg(p + 1);
        b2.tagTypeName = QStringLiteral("P");
        blocks.append(b2);
    }

    return blocks;
}

} // namespace vectorpdf::a11y
