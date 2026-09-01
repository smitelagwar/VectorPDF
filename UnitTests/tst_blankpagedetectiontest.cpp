// MIT License
// Copyright (c) 2026 VectorPDF Contributors
#include <QtTest>
#include "blankpage/blankpagefeatures.h"
#include "blankpage/blankpagepolicy.h"
#include "blankpage/blankpagedetector.h"

using namespace vectorpdf::scan;

class Tst_BlankPageDetectionTest : public QObject
{
    Q_OBJECT

private slots:
    void testBlankImageDetection();
    void testNonBlankImageDetection();
};

void Tst_BlankPageDetectionTest::testBlankImageDetection()
{
    QImage whiteImg(400, 600, QImage::Format_RGB32);
    whiteImg.fill(Qt::white);

    BlankPagePolicy policy;
    BlankPageFeatures feat = BlankPageDetector::analyzeImage(whiteImg, 0, policy);

    QCOMPARE(feat.pageIndex, 0);
    QCOMPARE(feat.darkPixelPercentage, 0.0);
    QCOMPARE(feat.isClassifiedBlank, true);
}

void Tst_BlankPageDetectionTest::testNonBlankImageDetection()
{
    QImage textImg(400, 600, QImage::Format_RGB32);
    textImg.fill(Qt::white);

    // Draw dark rectangle (simulating content)
    QPainter p(&textImg);
    p.fillRect(50, 50, 300, 400, Qt::black);
    p.end();

    BlankPagePolicy policy;
    BlankPageFeatures feat = BlankPageDetector::analyzeImage(textImg, 1, policy);

    QCOMPARE(feat.pageIndex, 1);
    QVERIFY(feat.darkPixelPercentage > 10.0);
    QCOMPARE(feat.isClassifiedBlank, false);
}

QTEST_MAIN(Tst_BlankPageDetectionTest)
#include "tst_blankpagedetectiontest.moc"
