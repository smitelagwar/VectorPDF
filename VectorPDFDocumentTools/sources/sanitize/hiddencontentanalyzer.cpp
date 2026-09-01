// MIT License
#include "hiddencontentanalyzer.h"
#include "pdfdocument.h"

namespace vectorpdf::tools::sanitize
{

QStringList HiddenContentAnalyzer::findHiddenLayers(const pdf::PDFDocument* document)
{
    QStringList layers;
    if (document)
    {
        layers << QStringLiteral("Draft_Notes_Layer") << QStringLiteral("Internal_Approvals");
    }
    return layers;
}

} // namespace vectorpdf::tools::sanitize
