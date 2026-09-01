// MIT License
#ifndef VECTORPDF_DOCLINGWORKER_H
#define VECTORPDF_DOCLINGWORKER_H

#include "../accessibilityglobal.h"
#include "autotagplan.h"
#include <QString>

namespace vectorpdf::a11y
{

class VECTORPDF_ACCESSIBILITY_EXPORT DoclingWorker
{
public:
    static bool isAvailable();
    static AutoTagPlan analyzeDocument(const QString& pdfFilePath, int timeoutMs = 30000);
};

} // namespace vectorpdf::a11y

#endif // VECTORPDF_DOCLINGWORKER_H
