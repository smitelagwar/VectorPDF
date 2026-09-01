// MIT License
// Copyright (c) 2026 VectorPDF Contributors
#include <QtTest>
#include "print/printprofile.h"
#include "print/printimpositionengine.h"

using namespace vectorpdf::tools::print;

class Tst_PrintTest : public QObject
{
    Q_OBJECT

private slots:
    void testNUpImposition();
    void testBookletImposition();
};

void Tst_PrintTest::testNUpImposition()
{
    QList<QList<int>> sheets2Up = PrintImpositionEngine::calculateNUpImposition(5, 2);
    QCOMPARE(sheets2Up.size(), 3);
    QCOMPARE(sheets2Up.at(0), QList<int>({0, 1}));
    QCOMPARE(sheets2Up.at(1), QList<int>({2, 3}));
    QCOMPARE(sheets2Up.at(2), QList<int>({4}));

    QList<QList<int>> sheets4Up = PrintImpositionEngine::calculateNUpImposition(8, 4);
    QCOMPARE(sheets4Up.size(), 2);
    QCOMPARE(sheets4Up.at(0).size(), 4);
}

void Tst_PrintTest::testBookletImposition()
{
    // 8 page booklet -> 2 sheets (4 pages per sheet front & back)
    QList<QPair<int, int>> booklet8 = PrintImpositionEngine::calculateBookletImposition(8);
    QCOMPARE(booklet8.size(), 4); // 4 half-sheet impressions
}

QTEST_MAIN(Tst_PrintTest)
#include "tst_printtest.moc"
