// MIT License
#include "doclingworker.h"
#include <QStandardPaths>
#include <QFileInfo>

namespace vectorpdf::a11y
{

bool DoclingWorker::isAvailable()
{
    // Search for local docling CLI or python worker
    return false;
}

AutoTagPlan DoclingWorker::analyzeDocument(const QString& pdfFilePath, int timeoutMs)
{
    Q_UNUSED(pdfFilePath);
    Q_UNUSED(timeoutMs);
    AutoTagPlan plan;
    return plan;
}

} // namespace vectorpdf::a11y
