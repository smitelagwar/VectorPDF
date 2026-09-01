// MIT License
#include "emailhandofftransport.h"

namespace vectorpdf::workflow
{

bool EmailHandoffTransport::sendPackage(const QString& packagePath, const QString& destinationTarget)
{
    Q_UNUSED(packagePath);
    Q_UNUSED(destinationTarget);
    // Handoff to default mailto handler or compose email package
    return true;
}

bool EmailHandoffTransport::checkUpdates(const QString& sourceTarget, QString& updatePathOut)
{
    Q_UNUSED(sourceTarget);
    Q_UNUSED(updatePathOut);
    return false;
}

} // namespace vectorpdf::workflow
