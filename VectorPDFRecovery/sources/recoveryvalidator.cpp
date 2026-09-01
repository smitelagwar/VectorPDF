// MIT License
#include "recoveryvalidator.h"
#include <QFileInfo>
#include <QFile>

namespace vectorpdf::recovery
{

bool RecoveryValidator::validateSnapshotIntegrity(const QString& snapshotFilePath)
{
    if (snapshotFilePath.isEmpty())
        return false;

    QFileInfo info(snapshotFilePath);
    if (!info.exists() || info.size() < 10)
        return false;

    QFile f(snapshotFilePath);
    if (!f.open(QIODevice::ReadOnly))
        return false;

    QByteArray header = f.read(5);
    return header.startsWith("%PDF-");
}

} // namespace vectorpdf::recovery
