// MIT License
#ifndef VECTORPDF_VERAPDFUAWORKER_H
#define VECTORPDF_VERAPDFUAWORKER_H

#include "../accessibilityglobal.h"
#include "pdfuavalidationreport.h"
#include <QString>

namespace vectorpdf::a11y
{

class VECTORPDF_ACCESSIBILITY_EXPORT VeraPdfUaWorker
{
public:
    static bool isAvailable();
    static PdfUaValidationReport validatePdfUa(const QString& pdfFilePath, int timeoutMs = 30000);
};

} // namespace vectorpdf::a11y

#endif // VECTORPDF_VERAPDFUAWORKER_H
