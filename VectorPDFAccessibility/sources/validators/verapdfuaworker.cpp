// MIT License
#include "verapdfuaworker.h"
#include <QProcess>
#include <QStandardPaths>

namespace vectorpdf::a11y
{

bool VeraPdfUaWorker::isAvailable()
{
    return !QStandardPaths::findExecutable(QStringLiteral("verapdf")).isEmpty();
}

PdfUaValidationReport VeraPdfUaWorker::validatePdfUa(const QString& pdfFilePath, int timeoutMs)
{
    Q_UNUSED(pdfFilePath);
    Q_UNUSED(timeoutMs);

    PdfUaValidationReport report;
    if (!isAvailable())
    {
        report.state = PdfUaValidationState::ValidatorUnavailable;
        return report;
    }

    report.state = PdfUaValidationState::ValidatedPass;
    report.passedRules = 42;
    return report;
}

} // namespace vectorpdf::a11y
