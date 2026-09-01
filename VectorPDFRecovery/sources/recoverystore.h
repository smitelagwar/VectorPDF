// MIT License
#ifndef VECTORPDF_RECOVERYSTORE_H
#define VECTORPDF_RECOVERYSTORE_H

#include "recoveryglobal.h"
#include <QString>
#include <QStringList>

namespace vectorpdf::recovery
{

class VECTORPDF_RECOVERY_EXPORT RecoveryStore
{
public:
    static QString defaultRecoveryDirectory();
    static QString sessionDirectory(const QString& sessionId);
    static bool initializeStore();
    static QStringList listAllManifestFiles();
    static bool purgeSession(const QString& sessionId);
    static void cleanupOldSessions(int maxAgeDays = 7);
};

} // namespace vectorpdf::recovery

#endif // VECTORPDF_RECOVERYSTORE_H
