// MIT License
// Copyright (c) 2026 VectorPDF Contributors
#include <QtTest>
#include "structure/pdfstructuretreemodel.h"
#include "structure/mcidallocator.h"
#include "readingorder/readingordermodel.h"

using namespace vectorpdf::a11y;

class Tst_StructureTreeTest : public QObject
{
    Q_OBJECT

private slots:
    void testStructureTreeModel();
    void testMcidAllocator();
    void testReadingOrderModel();
};

void Tst_StructureTreeTest::testStructureTreeModel()
{
    PDFStructureTreeModel model;
    QCOMPARE(model.rowCount(), 0);
}

void Tst_StructureTreeTest::testMcidAllocator()
{
    McidAllocator alloc;
    int id0 = alloc.allocateNextMcid(0);
    int id1 = alloc.allocateNextMcid(0);
    int idPage1 = alloc.allocateNextMcid(1);

    QCOMPARE(id0, 0);
    QCOMPARE(id1, 1);
    QCOMPARE(idPage1, 0);

    alloc.resetPage(0);
    int id0Reset = alloc.allocateNextMcid(0);
    QCOMPARE(id0Reset, 0);
}

void Tst_StructureTreeTest::testReadingOrderModel()
{
    ReadingOrderModel model;
    ReadingBlock b1;
    b1.order = 1;
    b1.pageIndex = 0;
    b1.textSnippet = QStringLiteral("Heading");

    ReadingBlock b2;
    b2.order = 2;
    b2.pageIndex = 0;
    b2.textSnippet = QStringLiteral("Paragraph");

    model.setBlocks({b2, b1});
    QList<ReadingBlock> page0 = model.blocksForPage(0);
    QCOMPARE(page0.size(), 2);
    QCOMPARE(page0.at(0).order, 1); // Sorted by order
}

QTEST_MAIN(Tst_StructureTreeTest)
#include "tst_structuretreetest.moc"
