// MIT License
#ifndef VECTORPDF_HIDDENCONTENTANALYZER_H
#define VECTORPDF_HIDDENCONTENTANALYZER_H

#include "../documenttoolsglobal.h"
#include <QStringList>

namespace pdf { class PDFDocument; }

namespace vectorpdf::tools::sanitize
{

class VECTORPDF_DOCUMENTTOOLS_EXPORT HiddenContentAnalyzer
{
public:
    static QStringList findHiddenLayers(const pdf::PDFDocument* document);
};

} // namespace vectorpdf::tools::sanitize

#endif // VECTORPDF_HIDDENCONTENTANALYZER_H
