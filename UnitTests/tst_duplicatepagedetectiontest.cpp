// MIT License
// Copyright (c) 2026 VectorPDF Contributors
#include <QtTest>
#include "duplicate/pageduplicatefingerprint.h"
#include "duplicate/duplicatecluster.h"
#include "duplicate/duplicatepolicy.h"
#include "duplicate/duplicatepagedetector.h"

using namespace vectorpdf::scan;

class Tst_DuplicatePageDetectionTest : public QObject
{
    Q_OBJECT

private slots:
    void testDifferenceHashIdentity();
    void testTextFingerprint();
    void testDuplicateDetector();
};

void Tst_DuplicatePageDetectionTest::testDifferenceHashIdentity()
{
    QImage img1(100, 100, QImage::Format_RGB32);
    img1.fill(Qt::white);

    QImage img2 = img1; // Identical

    quint64 h1 = PageDuplicateFingerprint::computeDifferenceHash(img1);
    quint64 h2 = PageDuplicateFingerprint::computeDifferenceHash(img2);

    QCOMPARE(h1, h2);
    QCOMPARE(PageDuplicateFingerprint::hammingDistance(h1, h2), 0);
    QCOMPARE(PageDuplicateFingerprint::calculateSimilarity(h1, h2), 1.0);
}

void Tst_DuplicatePageDetectionTest::testTextFingerprint()
{
    QString t1 = QStringLiteral("Sample Document Text");
    QString t2 = QStringLiteral("sample document text   ");

    QString hash1 = PageDuplicateFingerprint::computeTextFingerprint(t1);
    QString hash2 = PageDuplicateFingerprint::computeTextFingerprint(t2);

    QCOMPARE(hash1, hash2);
}

void Tst_DuplicatePageDetectionTest::testDuplicateDetector()
{
    QImage p1(100, 100, QImage::Format_RGB32);
    p1.fill(Qt::white);
    QImage p2(100, 100, QImage::Format_RGB32);
    p2.fill(Qt::white); // Identical to p1

    DuplicatePolicy policy;
    QList<DuplicateCluster> clusters = DuplicatePageDetector::findDuplicates({p1, p2}, policy);

    QCOMPARE(clusters.size(), 1);
    QCOMPARE(clusters.at(0).primaryPageIndex, 0);
    QCOMPARE(clusters.at(0).duplicatePageIndices, QList<int>({1}));
}

QTEST_MAIN(Tst_DuplicatePageDetectionTest)
#include "tst_duplicatepagedetectiontest.moc"
