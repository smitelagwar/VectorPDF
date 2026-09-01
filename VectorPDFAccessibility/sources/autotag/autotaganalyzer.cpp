// MIT License
#include "autotaganalyzer.h"
#include "nativeheuristictagger.h"
#include "doclingworker.h"

namespace vectorpdf::a11y
{

AutoTagPlan AutoTagAnalyzer::analyze(const pdf::PDFDocument* document, AutoTagBackend backend)
{
    if (backend == AutoTagBackend::Docling && DoclingWorker::isAvailable())
    {
        return DoclingWorker::analyzeDocument(QString());
    }
    return NativeHeuristicTagger::generatePlan(document);
}

} // namespace vectorpdf::a11y
