// MIT License
// Copyright (c) 2026 VectorPDF Contributors
#include <QtTest>
#include "bates/batessequence.h"
#include "bates/batesdefinition.h"
#include "bates/batesbatchplan.h"

using namespace vectorpdf::tools::bates;

class Tst_BatesTest : public QObject
{
    Q_OBJECT

private slots:
    void testSequenceFormatting();
    void testBatchPlan();
};

void Tst_BatesTest::testSequenceFormatting()
{
    BatesSequence seq(QStringLiteral("CONF-"), 1, 6, QStringLiteral("-US"));
    QCOMPARE(seq.currentNumber(), QStringLiteral("CONF-000001-US"));
    QCOMPARE(seq.nextNumber(), QStringLiteral("CONF-000001-US"));
    QCOMPARE(seq.currentNumber(), QStringLiteral("CONF-000002-US"));

    seq.reset(99);
    QCOMPARE(seq.currentNumber(), QStringLiteral("CONF-000099-US"));
}

void Tst_BatesTest::testBatchPlan()
{
    BatesBatchPlan plan;
    plan.definition.sequence = BatesSequence(QStringLiteral("CASE-"), 1, 6);
    plan.targetFilePaths = {QStringLiteral("doc1.pdf"), QStringLiteral("doc2.pdf")};

    plan.calculatePlan({3, 2});
    QCOMPARE(plan.totalPages, 5);
    QCOMPARE(plan.startEndNumbersByFile.value(QStringLiteral("doc1.pdf")), QStringLiteral("CASE-000001 - CASE-000003"));
    QCOMPARE(plan.startEndNumbersByFile.value(QStringLiteral("doc2.pdf")), QStringLiteral("CASE-000004 - CASE-000005"));
}

QTEST_MAIN(Tst_BatesTest)
#include "tst_batestest.moc"
