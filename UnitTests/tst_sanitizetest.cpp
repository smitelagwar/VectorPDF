// MIT License
// Copyright (c) 2026 VectorPDF Contributors
#include <QtTest>
#include "sanitize/sanitizerule.h"
#include "sanitize/sanitizeresult.h"
#include "sanitize/sanitizeanalyzer.h"
#include "sanitize/sanitizedocumentservice.h"

using namespace vectorpdf::tools::sanitize;

class Tst_SanitizeTest : public QObject
{
    Q_OBJECT

private slots:
    void testSanitizePolicy();
    void testSanitizeServiceOnNull();
};

void Tst_SanitizeTest::testSanitizePolicy()
{
    SanitizePolicy policy;
    QCOMPARE(policy.removeMetadata, true);
    QCOMPARE(policy.removeAttachments, true);
    QCOMPARE(policy.removeJavaScript, true);
    QCOMPARE(policy.removeActions, true);
    QCOMPARE(policy.removeHiddenLayers, true);
}

void Tst_SanitizeTest::testSanitizeServiceOnNull()
{
    SanitizePolicy policy;
    SanitizeReport rep = SanitizeDocumentService::sanitizeDocument(nullptr, policy);
    QCOMPARE(rep.result.isSuccessful(), false);
}

QTEST_MAIN(Tst_SanitizeTest)
#include "tst_sanitizetest.moc"
