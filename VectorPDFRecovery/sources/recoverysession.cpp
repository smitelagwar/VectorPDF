// MIT License
#include "recoverysession.h"
#include "recoverystore.h"
#include "recoverymanifest.h"
#include "recoverysnapshotwriter.h"
#include <QDir>
#include <QFileInfo>

namespace vectorpdf::recovery
{

RecoverySession::RecoverySession(const QString& originalFilePath, pdf::PDFDocument* document)
    : m_document(document)
{
    RecoveryStore::initializeStore();
    m_metadata.sessionId = QUuid::createUuid();
    m_metadata.originalFilePath = originalFilePath;
    m_metadata.sessionStartTime = QDateTime::currentDateTime();
    m_metadata.lastAutosaveTime = QDateTime::currentDateTime();
    m_metadata.state = RecoverySessionState::Active;

    if (!originalFilePath.isEmpty())
    {
        QFileInfo info(originalFilePath);
        m_metadata.documentTitle = info.fileName();
        m_metadata.originalFileSize = info.size();
    }

    m_sessionDir = RecoveryStore::sessionDirectory(m_metadata.sessionId.toString(QUuid::WithoutBraces));
    QDir().mkpath(m_sessionDir);

    m_metadata.tempSnapshotFilePath = m_sessionDir + QStringLiteral("/snapshot.pdf");
    QString journalPath = m_sessionDir + QStringLiteral("/journal.log");
    m_journal = std::make_unique<RecoveryJournal>(journalPath);

    RecoveryManifest::writeToFile(m_sessionDir + QStringLiteral("/manifest.json"), m_metadata);
}

RecoverySession::~RecoverySession()
{
}

bool RecoverySession::recordAction(const QString& actionName, const QString& details)
{
    m_metadata.uncommittedActionsCount++;
    m_metadata.lastAutosaveTime = QDateTime::currentDateTime();
    if (m_journal)
    {
        m_journal->appendEntry(actionName, details);
    }
    RecoveryManifest::writeToFile(m_sessionDir + QStringLiteral("/manifest.json"), m_metadata);
    return true;
}

bool RecoverySession::performAutosave()
{
    if (!m_document)
        return false;

    bool ok = RecoverySnapshotWriter::writeSnapshot(m_document, m_metadata.tempSnapshotFilePath);
    if (ok)
    {
        m_metadata.lastAutosaveTime = QDateTime::currentDateTime();
        RecoveryManifest::writeToFile(m_sessionDir + QStringLiteral("/manifest.json"), m_metadata);
    }
    return ok;
}

void RecoverySession::closeCleanly()
{
    m_metadata.state = RecoverySessionState::CleanlyClosed;
    RecoveryStore::purgeSession(m_metadata.sessionId.toString(QUuid::WithoutBraces));
}

} // namespace vectorpdf::recovery
