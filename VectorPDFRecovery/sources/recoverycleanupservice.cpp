// MIT License
#include "recoverycleanupservice.h"
#include "recoverystore.h"
#include "recoverymanifest.h"

namespace vectorpdf::recovery
{

void RecoveryCleanupService::cleanupTerminatedSession(const QString& sessionId)
{
    RecoveryStore::purgeSession(sessionId);
}

void RecoveryCleanupService::markCleanClose(const QString& sessionId)
{
    QString manifestPath = RecoveryStore::sessionDirectory(sessionId) + QStringLiteral("/manifest.json");
    RecoveryMetadata meta;
    if (RecoveryManifest::readFromFile(manifestPath, meta))
    {
        meta.state = RecoverySessionState::CleanlyClosed;
        RecoveryManifest::writeToFile(manifestPath, meta);
    }
    RecoveryStore::purgeSession(sessionId);
}

} // namespace vectorpdf::recovery
