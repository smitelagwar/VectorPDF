// MIT License
#ifndef VECTORPDF_BLANKPAGEDETECTOR_H
#define VECTORPDF_BLANKPAGEDETECTOR_H

#include "../scantoolsglobal.h"
#include "blankpagefeatures.h"
#include "blankpagepolicy.h"
#include <QImage>
#include <QList>

namespace pdf { class PDFDocument; }

namespace vectorpdf::scan
{

class VECTORPDF_SCANTOOLS_EXPORT BlankPageDetector
{
public:
    static BlankPageFeatures analyzeImage(const QImage& pageImage, int pageIndex, const BlankPagePolicy& policy);
    static QList<int> detectBlankPagesInDocument(const pdf::PDFDocument* document, const BlankPagePolicy& policy);
};

} // namespace vectorpdf::scan

#endif // VECTORPDF_BLANKPAGEDETECTOR_H
