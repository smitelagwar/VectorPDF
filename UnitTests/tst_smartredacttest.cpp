// MIT License
// Copyright (c) 2026 VectorPDF Contributors
#include <QtTest>
#include "redact/detectors/tckimlikdetector.h"
#include "redact/detectors/ibandetector.h"
#include "redact/detectors/paymentcarddetector.h"
#include "redact/detectors/emaildetector.h"
#include "redact/detectors/phonedetector.h"
#include "redact/redactionpatternregistry.h"
#include "redact/redactionreviewmodel.h"

using namespace vectorpdf::tools::redact;

class Tst_SmartRedactTest : public QObject
{
    Q_OBJECT

private slots:
    void testTcKimlikChecksum();
    void testIbanChecksum();
    void testPaymentCardLuhn();
    void testPatternRegistry();
    void testReviewModel();
};

void Tst_SmartRedactTest::testTcKimlikChecksum()
{
    // Synthetic valid TC test
    QCOMPARE(TcKimlikDetector::validateChecksum(QStringLiteral("01234567890")), false); // starts with 0
    QCOMPARE(TcKimlikDetector::validateChecksum(QStringLiteral("1234567890")), false);  // 10 digits
}

void Tst_SmartRedactTest::testIbanChecksum()
{
    // Invalid IBAN tests
    QCOMPARE(IbanDetector::validateIban(QStringLiteral("TR000000000000000000000000")), false);
}

void Tst_SmartRedactTest::testPaymentCardLuhn()
{
    QCOMPARE(PaymentCardDetector::validateLuhn(QStringLiteral("49927398716")), true);
    QCOMPARE(PaymentCardDetector::validateLuhn(QStringLiteral("49927398717")), false);
}

void Tst_SmartRedactTest::testPatternRegistry()
{
    RedactionPatternRegistry& reg = RedactionPatternRegistry::instance();
    QVERIFY(reg.allDetectors().size() >= 9);

    QString sampleText = QStringLiteral("İletişim için test@example.com adresine yazınız.");
    QList<RedactionCandidate> res = reg.scanText(sampleText, 0);
    QCOMPARE(res.size(), 1);
    QCOMPARE(res.at(0).rawText, QStringLiteral("test@example.com"));
}

void Tst_SmartRedactTest::testReviewModel()
{
    RedactionReviewModel model;
    RedactionCandidate c;
    c.detectorName = QStringLiteral("E-mail");
    c.rawText = QStringLiteral("user@domain.com");
    c.selected = true;

    model.setCandidates({c});
    QCOMPARE(model.rowCount(), 1);
    QCOMPARE(model.selectedCandidates().size(), 1);

    model.setSelectAll(false);
    QCOMPARE(model.selectedCandidates().size(), 0);
}

QTEST_MAIN(Tst_SmartRedactTest)
#include "tst_smartredacttest.moc"
