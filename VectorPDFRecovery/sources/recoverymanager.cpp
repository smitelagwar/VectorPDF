// MIT License
#include "recoverymanager.h"
#include "recoverylaunchscanner.h"
#include "recoverystore.h"
#include <QFile>

namespace vectorpdf::recovery
{

RecoveryManager& RecoveryManager::instance()
{
    static RecoveryManager mgr;
    return mgr;
}

RecoveryManager::RecoveryManager(QObject* parent)
    : QObject(parent)
{
    RecoveryStore::initializeStore();
}

std::shared_ptr<RecoverySession> RecoveryManager::startSession(const QString& filePath, pdf::PDFDocument* document)
{
    auto session = std::make_shared<RecoverySession>(filePath, document);
    m_activeSessions[session->sessionId()] = session;
    return session;
}

void RecoveryManager::endSession(const QUuid& sessionId)
{
    if (m_activeSessions.contains(sessionId))
    {
        m_activeSessions[sessionId]->closeCleanly();
        m_activeSessions.remove(sessionId);
    }
}

QList<RecoveryCandidate> RecoveryManager::checkRecoverableSessionsOnLaunch()
{
    return RecoveryLaunchScanner::scanForRecoverableSessions();
}

bool RecoveryManager::restoreCandidate(const RecoveryCandidate& candidate, const QString& restoreToPath)
{
    if (candidate.metadata.tempSnapshotFilePath.isEmpty() || restoreToPath.isEmpty())
        return false;

    if (QFile::exists(restoreToPath))
    {
        QFile::remove(restoreToPath);
    }

    bool copied = QFile::copy(candidate.metadata.tempSnapshotFilePath, restoreToPath);
    if (copied)
    {
        RecoveryStore::purgeSession(candidate.metadata.sessionId.toString(QUuid::WithoutBraces));
    }
    return copied;
}

bool RecoveryManager::discardCandidate(const RecoveryCandidate& candidate)
{
    return RecoveryStore::purgeSession(candidate.metadata.sessionId.toString(QUuid::WithoutBraces));
}

} // namespace vectorpdf::recovery
