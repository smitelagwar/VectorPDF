// MIT License
// Copyright (c) 2026 VectorPDF Contributors
#include <QtTest>
#include "flatten/flattenpolicy.h"
#include "flatten/flattenplan.h"
#include "flatten/flattenreport.h"
#include "flatten/flattenengine.h"

using namespace vectorpdf::tools::flatten;

class Tst_FlattenTest : public QObject
{
    Q_OBJECT

private slots:
    void testFlattenPolicyDefaults();
    void testFlattenEngineAnalyze();
};

void Tst_FlattenTest::testFlattenPolicyDefaults()
{
    FlattenPolicy policy;
    QCOMPARE(policy.forms, true);
    QCOMPARE(policy.annotations, true);
    QCOMPARE(policy.preserveLinks, true);
    QCOMPARE(policy.preserveDigitalSignatures, true);
    QCOMPARE(policy.layers, false);
}

void Tst_FlattenTest::testFlattenEngineAnalyze()
{
    FlattenPolicy policy;
    FlattenPlan plan = FlattenEngine::analyze(nullptr, policy);
    QCOMPARE(plan.policy.forms, true);
}

QTEST_MAIN(Tst_FlattenTest)
#include "tst_flattentest.moc"
