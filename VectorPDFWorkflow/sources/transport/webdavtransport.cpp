// MIT License
#include "webdavtransport.h"

namespace vectorpdf::workflow
{

bool WebDavTransport::sendPackage(const QString& packagePath, const QString& destinationTarget)
{
    Q_UNUSED(packagePath);
    Q_UNUSED(destinationTarget);
    return true;
}

bool WebDavTransport::checkUpdates(const QString& sourceTarget, QString& updatePathOut)
{
    Q_UNUSED(sourceTarget);
    Q_UNUSED(updatePathOut);
    return false;
}

} // namespace vectorpdf::workflow
