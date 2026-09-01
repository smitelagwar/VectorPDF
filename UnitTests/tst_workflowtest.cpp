// MIT License
// Copyright (c) 2026 VectorPDF Contributors
#include <QtTest>
#include "workflowtypes.h"
#include "package/workflowpackagemanifest.h"
#include "signrequest/signrequeststate.h"
#include "approval/approvalservice.h"
#include "review/reviewmergeengine.h"

using namespace vectorpdf::workflow;

class Tst_WorkflowTest : public QObject
{
    Q_OBJECT

private slots:
    void testPackageManifest();
    void testSignRequestState();
    void testApprovalService();
    void testReviewMergeEngine();
};

void Tst_WorkflowTest::testPackageManifest()
{
    WorkflowMetadata meta;
    meta.title = QStringLiteral("Contract Sign-off");
    meta.initiatorEmail = QStringLiteral("admin@vectorpdf.org");

    QList<WorkflowParticipant> parts;
    WorkflowParticipant p1;
    p1.name = QStringLiteral("Alice");
    p1.email = QStringLiteral("alice@example.com");
    parts.append(p1);

    QJsonObject json = WorkflowPackageManifest::serialize(meta, parts);
    QCOMPARE(json.value(QStringLiteral("title")).toString(), QStringLiteral("Contract Sign-off"));

    WorkflowMetadata rMeta;
    QList<WorkflowParticipant> rParts;
    bool ok = WorkflowPackageManifest::deserialize(json, rMeta, rParts);
    QVERIFY(ok);
    QCOMPARE(rMeta.title, meta.title);
    QCOMPARE(rParts.size(), 1);
    QCOMPARE(rParts.at(0).name, QStringLiteral("Alice"));
}

void Tst_WorkflowTest::testSignRequestState()
{
    SignRequestState state;
    state.totalFields = 4;
    state.filledFields = 2;
    QCOMPARE(state.progressPercentage(), 50.0);
}

void Tst_WorkflowTest::testApprovalService()
{
    ApprovalDefinition def;
    ApprovalStep s1;
    s1.approverName = QStringLiteral("Approver 1");
    s1.decision = ApprovalDecision::Approved;
    def.steps.append(s1);

    QCOMPARE(ApprovalService::isFullyApproved(def), true);

    ApprovalStep s2;
    s2.approverName = QStringLiteral("Approver 2");
    s2.decision = ApprovalDecision::Pending;
    def.steps.append(s2);

    QCOMPARE(ApprovalService::isFullyApproved(def), false);
}

void Tst_WorkflowTest::testReviewMergeEngine()
{
    ReviewDefinition target;
    ReviewThread t1;
    t1.threadId = QUuid::createUuid();
    target.threads.append(t1);

    ReviewThread t2;
    t2.threadId = QUuid::createUuid();
    QList<ReviewThread> incoming = {t2};

    ReviewMergeEngine::mergeIncomingThreads(target, incoming);
    QCOMPARE(target.threads.size(), 2);
}

QTEST_MAIN(Tst_WorkflowTest)
#include "tst_workflowtest.moc"
