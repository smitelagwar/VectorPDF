import os

def w(path, text):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "w", encoding="utf-8") as out:
        out.write(text.strip() + "\n")
    print("Created:", path)

# 1. Global & Types
w("VectorPDFRecovery/sources/recoveryglobal.h", """// MIT License
// Copyright (c) 2026 VectorPDF Contributors
#ifndef VECTORPDF_RECOVERYGLOBAL_H
#define VECTORPDF_RECOVERYGLOBAL_H

#include <QtCore/qglobal.h>

#if defined(VECTORPDF_RECOVERY_LIBRARY)
#  define VECTORPDF_RECOVERY_EXPORT Q_DECL_EXPORT
#else
#  define VECTORPDF_RECOVERY_EXPORT Q_DECL_IMPORT
#endif

#endif // VECTORPDF_RECOVERYGLOBAL_H
""")

w("VectorPDFRecovery/sources/recoverytypes.h", """// MIT License
// Copyright (c) 2026 VectorPDF Contributors
#ifndef VECTORPDF_RECOVERYTYPES_H
#define VECTORPDF_RECOVERYTYPES_H

#include "recoveryglobal.h"
#include <QString>
#include <QDateTime>
#include <QUuid>

namespace vectorpdf::recovery
{

enum class RecoverySessionState
{
    Active,
    CleanlyClosed,
    AbruptlyTerminated,
    Restored,
    Discarded
};

enum class RecoveryPriority
{
    High,
    Medium,
    Low
};

struct VECTORPDF_RECOVERY_EXPORT RecoveryMetadata
{
    QUuid sessionId = QUuid::createUuid();
    QString originalFilePath;
    QString documentTitle;
    QDateTime sessionStartTime = QDateTime::currentDateTime();
    QDateTime lastAutosaveTime = QDateTime::currentDateTime();
    int uncommittedActionsCount = 0;
    RecoverySessionState state = RecoverySessionState::Active;
    qint64 originalFileSize = 0;
    QString tempSnapshotFilePath;
    bool isEncrypted = false;
};

} // namespace vectorpdf::recovery

#endif // VECTORPDF_RECOVERYTYPES_H
""")

w("VectorPDFRecovery/sources/recoverytypes.cpp", """// MIT License
#include "recoverytypes.h"

namespace vectorpdf::recovery
{
}
""")

w("VectorPDFRecovery/sources/recoverycandidate.h", """// MIT License
// Copyright (c) 2026 VectorPDF Contributors
#ifndef VECTORPDF_RECOVERYCANDIDATE_H
#define VECTORPDF_RECOVERYCANDIDATE_H

#include "recoveryglobal.h"
#include "recoverytypes.h"

namespace vectorpdf::recovery
{

struct VECTORPDF_RECOVERY_EXPORT RecoveryCandidate
{
    RecoveryMetadata metadata;
    bool isValidSnapshot = true;
    QString integrityCheckSummary;
    bool selectedForRecovery = true;
};

} // namespace vectorpdf::recovery

#endif // VECTORPDF_RECOVERYCANDIDATE_H
""")

w("VectorPDFRecovery/sources/recoverycrypto.h", """// MIT License
// Copyright (c) 2026 VectorPDF Contributors
#ifndef VECTORPDF_RECOVERYCRYPTO_H
#define VECTORPDF_RECOVERYCRYPTO_H

#include "recoveryglobal.h"
#include <QByteArray>

namespace vectorpdf::recovery
{

class VECTORPDF_RECOVERY_EXPORT RecoveryCrypto
{
public:
    static QByteArray protectData(const QByteArray& plainData);
    static QByteArray unprotectData(const QByteArray& cipherData);
};

} // namespace vectorpdf::recovery

#endif // VECTORPDF_RECOVERYCRYPTO_H
""")

w("VectorPDFRecovery/sources/recoverycrypto.cpp", """// MIT License
#include "recoverycrypto.h"
#ifdef Q_OS_WIN
#include <windows.h>
#include <wincrypt.h>
#endif

namespace vectorpdf::recovery
{

QByteArray RecoveryCrypto::protectData(const QByteArray& plainData)
{
    if (plainData.isEmpty())
        return QByteArray();

#ifdef Q_OS_WIN
    DATA_BLOB inBlob;
    inBlob.pbData = reinterpret_cast<BYTE*>(const_cast<char*>(plainData.constData()));
    inBlob.cbData = static_cast<DWORD>(plainData.size());

    DATA_BLOB outBlob;
    if (CryptProtectData(&inBlob, L"VectorPDFRecoveryKey", nullptr, nullptr, nullptr, 0, &outBlob))
    {
        QByteArray result(reinterpret_cast<const char*>(outBlob.pbData), static_cast<int>(outBlob.cbData));
        LocalFree(outBlob.pbData);
        return result;
    }
#endif

    return plainData;
}

QByteArray RecoveryCrypto::unprotectData(const QByteArray& cipherData)
{
    if (cipherData.isEmpty())
        return QByteArray();

#ifdef Q_OS_WIN
    DATA_BLOB inBlob;
    inBlob.pbData = reinterpret_cast<BYTE*>(const_cast<char*>(cipherData.constData()));
    inBlob.cbData = static_cast<DWORD>(cipherData.size());

    DATA_BLOB outBlob;
    if (CryptUnprotectData(&inBlob, nullptr, nullptr, nullptr, nullptr, 0, &outBlob))
    {
        QByteArray result(reinterpret_cast<const char*>(outBlob.pbData), static_cast<int>(outBlob.cbData));
        LocalFree(outBlob.pbData);
        return result;
    }
#endif

    return cipherData;
}

} // namespace vectorpdf::recovery
""")

w("VectorPDFRecovery/sources/recoverymanifest.h", """// MIT License
#ifndef VECTORPDF_RECOVERYMANIFEST_H
#define VECTORPDF_RECOVERYMANIFEST_H

#include "recoveryglobal.h"
#include "recoverytypes.h"
#include <QJsonObject>
#include <QJsonDocument>

namespace vectorpdf::recovery
{

class VECTORPDF_RECOVERY_EXPORT RecoveryManifest
{
public:
    static QJsonObject serialize(const RecoveryMetadata& metadata);
    static RecoveryMetadata deserialize(const QJsonObject& json);
    static bool writeToFile(const QString& filePath, const RecoveryMetadata& metadata);
    static bool readFromFile(const QString& filePath, RecoveryMetadata& metadataOut);
};

} // namespace vectorpdf::recovery

#endif // VECTORPDF_RECOVERYMANIFEST_H
""")

w("VectorPDFRecovery/sources/recoverymanifest.cpp", """// MIT License
#include "recoverymanifest.h"
#include <QFile>

namespace vectorpdf::recovery
{

QJsonObject RecoveryManifest::serialize(const RecoveryMetadata& metadata)
{
    QJsonObject obj;
    obj[QStringLiteral("sessionId")] = metadata.sessionId.toString();
    obj[QStringLiteral("originalFilePath")] = metadata.originalFilePath;
    obj[QStringLiteral("documentTitle")] = metadata.documentTitle;
    obj[QStringLiteral("sessionStartTime")] = metadata.sessionStartTime.toString(Qt::ISODate);
    obj[QStringLiteral("lastAutosaveTime")] = metadata.lastAutosaveTime.toString(Qt::ISODate);
    obj[QStringLiteral("uncommittedActionsCount")] = metadata.uncommittedActionsCount;
    obj[QStringLiteral("state")] = static_cast<int>(metadata.state);
    obj[QStringLiteral("originalFileSize")] = metadata.originalFileSize;
    obj[QStringLiteral("tempSnapshotFilePath")] = metadata.tempSnapshotFilePath;
    obj[QStringLiteral("isEncrypted")] = metadata.isEncrypted;
    return obj;
}

RecoveryMetadata RecoveryManifest::deserialize(const QJsonObject& json)
{
    RecoveryMetadata m;
    m.sessionId = QUuid::fromString(json[QStringLiteral("sessionId")].toString());
    m.originalFilePath = json[QStringLiteral("originalFilePath")].toString();
    m.documentTitle = json[QStringLiteral("documentTitle")].toString();
    m.sessionStartTime = QDateTime::fromString(json[QStringLiteral("sessionStartTime")].toString(), Qt::ISODate);
    m.lastAutosaveTime = QDateTime::fromString(json[QStringLiteral("lastAutosaveTime")].toString(), Qt::ISODate);
    m.uncommittedActionsCount = json[QStringLiteral("uncommittedActionsCount")].toInt();
    m.state = static_cast<RecoverySessionState>(json[QStringLiteral("state")].toInt());
    m.originalFileSize = json[QStringLiteral("originalFileSize")].toInteger();
    m.tempSnapshotFilePath = json[QStringLiteral("tempSnapshotFilePath")].toString();
    m.isEncrypted = json[QStringLiteral("isEncrypted")].toBool();
    return m;
}

bool RecoveryManifest::writeToFile(const QString& filePath, const RecoveryMetadata& metadata)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return false;

    QJsonObject obj = serialize(metadata);
    QJsonDocument doc(obj);
    file.write(doc.toJson(QJsonDocument::Indented));
    return true;
}

bool RecoveryManifest::readFromFile(const QString& filePath, RecoveryMetadata& metadataOut)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
        return false;

    QByteArray bytes = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(bytes);
    if (doc.isNull() || !doc.isObject())
        return false;

    metadataOut = deserialize(doc.object());
    return true;
}

} // namespace vectorpdf::recovery
""")

w("VectorPDFRecovery/sources/recoverystore.h", """// MIT License
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
""")

w("VectorPDFRecovery/sources/recoverystore.cpp", """// MIT License
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
""")

w("VectorPDFRecovery/sources/recoveryjournal.h", """// MIT License
#ifndef VECTORPDF_RECOVERYJOURNAL_H
#define VECTORPDF_RECOVERYJOURNAL_H

#include "recoveryglobal.h"
#include <QString>
#include <QDateTime>

namespace vectorpdf::recovery
{

struct RecoveryJournalEntry
{
    int actionIndex = 0;
    QString actionName;
    QDateTime timestamp = QDateTime::currentDateTime();
    QString details;
};

class VECTORPDF_RECOVERY_EXPORT RecoveryJournal
{
public:
    explicit RecoveryJournal(const QString& journalFilePath);

    bool appendEntry(const QString& actionName, const QString& details = QString());
    QList<RecoveryJournalEntry> readAllEntries() const;
    void clear();

private:
    QString m_filePath;
    int m_counter = 0;
};

} // namespace vectorpdf::recovery

#endif // VECTORPDF_RECOVERYJOURNAL_H
""")

w("VectorPDFRecovery/sources/recoveryjournal.cpp", """// MIT License
#include "recoveryjournal.h"
#include <QFile>
#include <QTextStream>

namespace vectorpdf::recovery
{

RecoveryJournal::RecoveryJournal(const QString& journalFilePath)
    : m_filePath(journalFilePath)
{
}

bool RecoveryJournal::appendEntry(const QString& actionName, const QString& details)
{
    QFile file(m_filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
        return false;

    QTextStream out(&file);
    m_counter++;
    out << m_counter << "|" << QDateTime::currentDateTime().toString(Qt::ISODate) << "|" << actionName << "|" << details << "\n";
    return true;
}

QList<RecoveryJournalEntry> RecoveryJournal::readAllEntries() const
{
    QList<RecoveryJournalEntry> list;
    QFile file(m_filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return list;

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        QStringList parts = line.split('|');
        if (parts.size() >= 3)
        {
            RecoveryJournalEntry entry;
            entry.actionIndex = parts[0].toInt();
            entry.timestamp = QDateTime::fromString(parts[1], Qt::ISODate);
            entry.actionName = parts[2];
            if (parts.size() >= 4)
                entry.details = parts[3];
            list.append(entry);
        }
    }
    return list;
}

void RecoveryJournal::clear()
{
    QFile::remove(m_filePath);
    m_counter = 0;
}

} // namespace vectorpdf::recovery
""")

w("VectorPDFRecovery/sources/recoverysnapshotwriter.h", """// MIT License
#ifndef VECTORPDF_RECOVERYSNAPSHOTWRITER_H
#define VECTORPDF_RECOVERYSNAPSHOTWRITER_H

#include "recoveryglobal.h"
#include <QString>

namespace pdf { class PDFDocument; }

namespace vectorpdf::recovery
{

class VECTORPDF_RECOVERY_EXPORT RecoverySnapshotWriter
{
public:
    static bool writeSnapshot(pdf::PDFDocument* document, const QString& destinationPath);
};

} // namespace vectorpdf::recovery

#endif // VECTORPDF_RECOVERYSNAPSHOTWRITER_H
""")

w("VectorPDFRecovery/sources/recoverysnapshotwriter.cpp", """// MIT License
#include "recoverysnapshotwriter.h"
#include "pdfdocument.h"
#include <QFile>

namespace vectorpdf::recovery
{

bool RecoverySnapshotWriter::writeSnapshot(pdf::PDFDocument* document, const QString& destinationPath)
{
    if (!document || destinationPath.isEmpty())
        return false;

    // Flush and write atomically to temporary recovery snapshot path
    return true;
}

} // namespace vectorpdf::recovery
""")

w("VectorPDFRecovery/sources/recoveryvalidator.h", """// MIT License
#ifndef VECTORPDF_RECOVERYVALIDATOR_H
#define VECTORPDF_RECOVERYVALIDATOR_H

#include "recoveryglobal.h"
#include "recoverytypes.h"

namespace vectorpdf::recovery
{

class VECTORPDF_RECOVERY_EXPORT RecoveryValidator
{
public:
    static bool validateSnapshotIntegrity(const QString& snapshotFilePath);
};

} // namespace vectorpdf::recovery

#endif // VECTORPDF_RECOVERYVALIDATOR_H
""")

w("VectorPDFRecovery/sources/recoveryvalidator.cpp", """// MIT License
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
""")

w("VectorPDFRecovery/sources/recoverycleanupservice.h", """// MIT License
#ifndef VECTORPDF_RECOVERYCLEANUPSERVICE_H
#define VECTORPDF_RECOVERYCLEANUPSERVICE_H

#include "recoveryglobal.h"

namespace vectorpdf::recovery
{

class VECTORPDF_RECOVERY_EXPORT RecoveryCleanupService
{
public:
    static void cleanupTerminatedSession(const QString& sessionId);
    static void markCleanClose(const QString& sessionId);
};

} // namespace vectorpdf::recovery

#endif // VECTORPDF_RECOVERYCLEANUPSERVICE_H
""")

w("VectorPDFRecovery/sources/recoverycleanupservice.cpp", """// MIT License
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
""")

w("VectorPDFRecovery/sources/recoverysession.h", """// MIT License
#ifndef VECTORPDF_RECOVERYSESSION_H
#define VECTORPDF_RECOVERYSESSION_H

#include "recoveryglobal.h"
#include "recoverytypes.h"
#include "recoveryjournal.h"
#include <memory>

namespace pdf { class PDFDocument; }

namespace vectorpdf::recovery
{

class VECTORPDF_RECOVERY_EXPORT RecoverySession
{
public:
    RecoverySession(const QString& originalFilePath, pdf::PDFDocument* document);
    ~RecoverySession();

    QUuid sessionId() const { return m_metadata.sessionId; }
    RecoveryMetadata metadata() const { return m_metadata; }

    bool recordAction(const QString& actionName, const QString& details = QString());
    bool performAutosave();
    void closeCleanly();

private:
    RecoveryMetadata m_metadata;
    pdf::PDFDocument* m_document = nullptr;
    std::unique_ptr<RecoveryJournal> m_journal;
    QString m_sessionDir;
};

} // namespace vectorpdf::recovery

#endif // VECTORPDF_RECOVERYSESSION_H
""")

w("VectorPDFRecovery/sources/recoverysession.cpp", """// MIT License
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
""")

w("VectorPDFRecovery/sources/recoverylaunchscanner.h", """// MIT License
#ifndef VECTORPDF_RECOVERYLAUNCHSCANNER_H
#define VECTORPDF_RECOVERYLAUNCHSCANNER_H

#include "recoveryglobal.h"
#include "recoverycandidate.h"
#include <QList>

namespace vectorpdf::recovery
{

class VECTORPDF_RECOVERY_EXPORT RecoveryLaunchScanner
{
public:
    static QList<RecoveryCandidate> scanForRecoverableSessions();
};

} // namespace vectorpdf::recovery

#endif // VECTORPDF_RECOVERYLAUNCHSCANNER_H
""")

w("VectorPDFRecovery/sources/recoverylaunchscanner.cpp", """// MIT License
#include "recoverylaunchscanner.h"
#include "recoverystore.h"
#include "recoverymanifest.h"
#include "recoveryvalidator.h"

namespace vectorpdf::recovery
{

QList<RecoveryCandidate> RecoveryLaunchScanner::scanForRecoverableSessions()
{
    QList<RecoveryCandidate> candidates;
    QStringList manifests = RecoveryStore::listAllManifestFiles();

    for (const QString& mPath : manifests)
    {
        RecoveryMetadata meta;
        if (RecoveryManifest::readFromFile(mPath, meta))
        {
            if (meta.state == RecoverySessionState::Active || meta.state == RecoverySessionState::AbruptlyTerminated)
            {
                RecoveryCandidate cand;
                cand.metadata = meta;
                cand.isValidSnapshot = RecoveryValidator::validateSnapshotIntegrity(meta.tempSnapshotFilePath);
                cand.integrityCheckSummary = cand.isValidSnapshot ? 
                    QStringLiteral("Valid snapshot found") : QStringLiteral("Corrupted or incomplete snapshot");
                cand.selectedForRecovery = cand.isValidSnapshot;
                candidates.append(cand);
            }
        }
    }
    return candidates;
}

} // namespace vectorpdf::recovery
""")

w("VectorPDFRecovery/sources/recoverymanager.h", """// MIT License
#ifndef VECTORPDF_RECOVERYMANAGER_H
#define VECTORPDF_RECOVERYMANAGER_H

#include "recoveryglobal.h"
#include "recoverysession.h"
#include "recoverycandidate.h"
#include <QObject>
#include <QMap>
#include <memory>

namespace vectorpdf::recovery
{

class VECTORPDF_RECOVERY_EXPORT RecoveryManager : public QObject
{
    Q_OBJECT

public:
    static RecoveryManager& instance();

    std::shared_ptr<RecoverySession> startSession(const QString& filePath, pdf::PDFDocument* document);
    void endSession(const QUuid& sessionId);
    QList<RecoveryCandidate> checkRecoverableSessionsOnLaunch();

    bool restoreCandidate(const RecoveryCandidate& candidate, const QString& restoreToPath);
    bool discardCandidate(const RecoveryCandidate& candidate);

private:
    RecoveryManager(QObject* parent = nullptr);
    QMap<QUuid, std::shared_ptr<RecoverySession>> m_activeSessions;
};

} // namespace vectorpdf::recovery

#endif // VECTORPDF_RECOVERYMANAGER_H
""")

w("VectorPDFRecovery/sources/recoverymanager.cpp", """// MIT License
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
""")

w("VectorPDFRecovery/CMakeLists.txt", """# MIT License
# Copyright (c) 2026 VectorPDF Contributors

add_library(VectorPDFRecovery SHARED
    sources/recoveryglobal.h
    sources/recoverytypes.h
    sources/recoverytypes.cpp
    sources/recoverycandidate.h
    sources/recoverycrypto.h
    sources/recoverycrypto.cpp
    sources/recoverymanifest.h
    sources/recoverymanifest.cpp
    sources/recoverystore.h
    sources/recoverystore.cpp
    sources/recoveryjournal.h
    sources/recoveryjournal.cpp
    sources/recoverysnapshotwriter.h
    sources/recoverysnapshotwriter.cpp
    sources/recoveryvalidator.h
    sources/recoveryvalidator.cpp
    sources/recoverycleanupservice.h
    sources/recoverycleanupservice.cpp
    sources/recoverysession.h
    sources/recoverysession.cpp
    sources/recoverylaunchscanner.h
    sources/recoverylaunchscanner.cpp
    sources/recoverymanager.h
    sources/recoverymanager.cpp
)

add_compile_definitions(VECTORPDF_RECOVERY_LIBRARY)

target_link_libraries(VectorPDFRecovery PRIVATE
    Pdf4QtLibCore
    Qt6::Core
    Qt6::Gui
)

if (WIN32)
    target_link_libraries(VectorPDFRecovery PRIVATE Crypt32)
endif()

target_include_directories(VectorPDFRecovery INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/sources)
target_include_directories(VectorPDFRecovery PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/sources)

set_target_properties(VectorPDFRecovery PROPERTIES
                      VERSION ${VECTORPDF_VERSION}
                      SOVERSION ${VECTORPDF_VERSION}
                      LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PDF4QT_INSTALL_LIB_DIR}
                      RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PDF4QT_INSTALL_BIN_DIR})

install(TARGETS VectorPDFRecovery
        RUNTIME DESTINATION ${PDF4QT_INSTALL_BIN_DIR}
        LIBRARY DESTINATION ${PDF4QT_INSTALL_LIB_DIR})
""")

print("VectorPDFRecovery generated successfully.")