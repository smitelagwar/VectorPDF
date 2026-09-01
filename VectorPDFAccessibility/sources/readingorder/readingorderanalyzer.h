// MIT License
#ifndef VECTORPDF_READINGORDERANALYZER_H
#define VECTORPDF_READINGORDERANALYZER_H

#include "../accessibilityglobal.h"
#include "readingordermodel.h"

namespace pdf { class PDFDocument; }

namespace vectorpdf::a11y
{

class VECTORPDF_ACCESSIBILITY_EXPORT ReadingOrderAnalyzer
{
public:
    static QList<ReadingBlock> analyzeDocument(const pdf::PDFDocument* document);
};

} // namespace vectorpdf::a11y

#endif // VECTORPDF_READINGORDERANALYZER_H
