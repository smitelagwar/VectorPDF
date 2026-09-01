// MIT License
// Copyright (c) 2026 VectorPDF Contributors
#include <QtTest>
#include "scanimage.h"
#include "cleanup/orientationdetector.h"
#include "cleanup/deskewprocessor.h"
#include "cleanup/punchholeremover.h"
#include "scanprocessingpipeline.h"

using namespace vectorpdf::scan;

class Tst_ScanCleanupTest : public QObject
{
    Q_OBJECT

private slots:
    void testScanImage();
    void testDeskewProcessor();
    void testPunchHoleRemover();
    void testPipeline();
};

void Tst_ScanCleanupTest::testScanImage()
{
    QImage raw(200, 300, QImage::Format_RGB32);
    raw.fill(Qt::white);
    ScanImage scan(raw);

    QCOMPARE(scan.isNull(), false);
    QCOMPARE(scan.width(), 200);
    QCOMPARE(scan.height(), 300);
}

void Tst_ScanCleanupTest::testDeskewProcessor()
{
    QImage raw(100, 100, QImage::Format_RGB32);
    raw.fill(Qt::white);

    QImage deskewed = DeskewProcessor::deskewImage(raw, 2.0);
    QCOMPARE(deskewed.isNull(), false);
}

void Tst_ScanCleanupTest::testPunchHoleRemover()
{
    QImage raw(100, 100, QImage::Format_RGB32);
    raw.fill(Qt::white);

    int count = 0;
    QImage clean = PunchHoleRemover::removePunchHoles(raw, count);
    QCOMPARE(clean.isNull(), false);
}

void Tst_ScanCleanupTest::testPipeline()
{
    QImage raw(100, 100, QImage::Format_RGB32);
    raw.fill(Qt::white);
    ScanImage scan(raw);

    ScanProcessingProfile profile;
    ScanProcessingResult res = ScanProcessingPipeline::process(scan, profile);
    QCOMPARE(res.success, true);
    QCOMPARE(res.processedImage.isNull(), false);
}

QTEST_MAIN(Tst_ScanCleanupTest)
#include "tst_scancleanuptest.moc"
