// MIT License
#include "recoverystore.h"
#include <QStandardPaths>
#include <QDir>
#include <QFileInfo>
#include <QDateTime>

namespace vectorpdf::recovery
{

QString RecoveryStore::defaultRecoveryDirectory()
{
    QString base = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (base.isEmpty())
    {
        base = QDir::homePath() + QStringLiteral("/.vectorpdf");
    }
    return base + QStringLiteral("/recovery");
}

QString RecoveryStore::sessionDirectory(const QString& sessionId)
{
    return defaultRecoveryDirectory() + QStringLiteral("/") + sessionId;
}

bool RecoveryStore::initializeStore()
{
    QDir dir(defaultRecoveryDirectory());
    if (!dir.exists())
    {
        return dir.mkpath(QStringLiteral("."));
    }
    return true;
}

QStringList RecoveryStore::listAllManifestFiles()
{
    QStringList results;
    QDir rootDir(defaultRecoveryDirectory());
    if (!rootDir.exists())
        return results;

    QStringList subdirs = rootDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString& sub : subdirs)
    {
        QString manifestPath = rootDir.filePath(sub + QStringLiteral("/manifest.json"));
        if (QFile::exists(manifestPath))
        {
            results.append(manifestPath);
        }
    }
    return results;
}

bool RecoveryStore::purgeSession(const QString& sessionId)
{
    QDir dir(sessionDirectory(sessionId));
    if (dir.exists())
    {
        return dir.removeRecursively();
    }
    return true;
}

void RecoveryStore::cleanupOldSessions(int maxAgeDays)
{
    QDir rootDir(defaultRecoveryDirectory());
    if (!rootDir.exists())
        return;

    QStringList subdirs = rootDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    QDateTime cutoff = QDateTime::currentDateTime().addDays(-maxAgeDays);

    for (const QString& sub : subdirs)
    {
        QFileInfo info(rootDir.filePath(sub));
        if (info.lastModified() < cutoff)
        {
            QDir(info.absoluteFilePath()).removeRecursively();
        }
    }
}

} // namespace vectorpdf::recovery
