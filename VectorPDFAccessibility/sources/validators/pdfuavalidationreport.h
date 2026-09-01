// MIT License
#ifndef VECTORPDF_PDFUAVALIDATIONREPORT_H
#define VECTORPDF_PDFUAVALIDATIONREPORT_H

#include "../accessibilityglobal.h"
#include <QString>
#include <QStringList>

namespace vectorpdf::a11y
{

enum class PdfUaValidationState
{
    ValidatedPass,
    ValidatedFail,
    ValidatorUnavailable,
    ValidatorError
};

struct VECTORPDF_ACCESSIBILITY_EXPORT PdfUaValidationReport
{
    PdfUaValidationState state = PdfUaValidationState::ValidatorUnavailable;
    QString validatorEngineName = QStringLiteral("veraPDF PDF/UA-1 Validator");
    int passedRules = 0;
    int failedRules = 0;
    QStringList failureDetails;
};

} // namespace vectorpdf::a11y

#endif // VECTORPDF_PDFUAVALIDATIONREPORT_H
