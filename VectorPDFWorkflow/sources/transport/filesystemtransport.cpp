// MIT License
#include "filesystemtransport.h"
#include <QFile>

namespace vectorpdf::workflow
{

bool FileSystemTransport::sendPackage(const QString& packagePath, const QString& destinationTarget)
{
    if (QFile::exists(destinationTarget))
        QFile::remove(destinationTarget);
    return QFile::copy(packagePath, destinationTarget);
}

bool FileSystemTransport::checkUpdates(const QString& sourceTarget, QString& updatePathOut)
{
    if (QFile::exists(sourceTarget))
    {
        updatePathOut = sourceTarget;
        return true;
    }
    return false;
}

} // namespace vectorpdf::workflow
