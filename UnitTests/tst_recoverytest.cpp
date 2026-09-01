// MIT License
// Copyright (c) 2026 VectorPDF Contributors
#include <QtTest>
#include "recoverytypes.h"
#include "recoverymanifest.h"
#include "recoverycrypto.h"
#include "recoveryjournal.h"
#include "recoverystore.h"

using namespace vectorpdf::recovery;

class Tst_RecoveryTest : public QObject
{
    Q_OBJECT

private slots:
    void testRecoveryManifestSerialization();
    void testRecoveryCrypto();
    void testRecoveryJournal();
    void testRecoveryStorePaths();
};

void Tst_RecoveryTest::testRecoveryManifestSerialization()
{
    RecoveryMetadata meta;
    meta.sessionId = QUuid::createUuid();
    meta.originalFilePath = QStringLiteral("C:/docs/report.pdf");
    meta.documentTitle = QStringLiteral("Annual Report");
    meta.uncommittedActionsCount = 5;
    meta.state = RecoverySessionState::Active;

    QJsonObject json = RecoveryManifest::serialize(meta);
    QCOMPARE(json.contains(QStringLiteral("sessionId")), true);
    QCOMPARE(json.value(QStringLiteral("uncommittedActionsCount")).toInt(), 5);

    RecoveryMetadata restored = RecoveryManifest::deserialize(json);
    QCOMPARE(restored.sessionId, meta.sessionId);
    QCOMPARE(restored.originalFilePath, meta.originalFilePath);
    QCOMPARE(restored.uncommittedActionsCount, 5);
}

void Tst_RecoveryTest::testRecoveryCrypto()
{
    QByteArray plain = "Confidential VectorPDF Recovery Payload";
    QByteArray cipher = RecoveryCrypto::protectData(plain);
    QVERIFY(!cipher.isEmpty());

    QByteArray decrypted = RecoveryCrypto::unprotectData(cipher);
    QCOMPARE(decrypted, plain);
}

void Tst_RecoveryTest::testRecoveryJournal()
{
    QString tempJournal = QDir::tempPath() + QStringLiteral("/test_journal.log");
    QFile::remove(tempJournal);

    RecoveryJournal journal(tempJournal);
    journal.appendEntry(QStringLiteral("AddWatermark"), QStringLiteral("CONFIDENTIAL"));
    journal.appendEntry(QStringLiteral("DeletePage"), QStringLiteral("Page 2"));

    QList<RecoveryJournalEntry> entries = journal.readAllEntries();
    QCOMPARE(entries.size(), 2);
    QCOMPARE(entries.at(0).actionName, QStringLiteral("AddWatermark"));
    QCOMPARE(entries.at(1).actionName, QStringLiteral("DeletePage"));

    journal.clear();
    QCOMPARE(journal.readAllEntries().size(), 0);
}

void Tst_RecoveryTest::testRecoveryStorePaths()
{
    QString defDir = RecoveryStore::defaultRecoveryDirectory();
    QVERIFY(!defDir.isEmpty());

    QString sDir = RecoveryStore::sessionDirectory(QStringLiteral("test-session-123"));
    QVERIFY(sDir.endsWith(QStringLiteral("test-session-123")));
}

QTEST_MAIN(Tst_RecoveryTest)
#include "tst_recoverytest.moc"
