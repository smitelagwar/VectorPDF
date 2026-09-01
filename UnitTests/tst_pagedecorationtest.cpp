// MIT License
// Copyright (c) 2026 VectorPDF Contributors
#include <QtTest>
#include "decorations/pagedecorationdefinition.h"
#include "decorations/headerfooterdefinition.h"
#include "decorations/watermarkdefinition.h"
#include "decorations/backgrounddefinition.h"
#include "decorations/decorationtemplateengine.h"
#include "decorations/pagedecorationmanager.h"

using namespace vectorpdf::tools::decorations;

class Tst_PageDecorationTest : public QObject
{
    Q_OBJECT

private slots:
    void testTargetPagesResolution();
    void testTemplateEngineTokens();
    void testDecorationDefinitions();
};

void Tst_PageDecorationTest::testTargetPagesResolution()
{
    PageDecorationTargetPages allP;
    allP.rangeMode = PageRangeMode::AllPages;
    QList<int> pages = allP.resolvePages(5);
    QCOMPARE(pages.size(), 5);

    PageDecorationTargetPages oddP;
    oddP.rangeMode = PageRangeMode::OddPagesOnly;
    QList<int> oddPages = oddP.resolvePages(5);
    QCOMPARE(oddPages, QList<int>({0, 2, 4}));

    PageDecorationTargetPages customP;
    customP.rangeMode = PageRangeMode::CustomRange;
    customP.customRangeString = QStringLiteral("1-2, 4");
    QList<int> customPages = customP.resolvePages(5);
    QCOMPARE(customPages, QList<int>({0, 1, 3}));
}

void Tst_PageDecorationTest::testTemplateEngineTokens()
{
    TemplateContext ctx;
    ctx.pageIndex = 2; // 3rd page
    ctx.totalPages = 10;
    ctx.filename = QStringLiteral("report.pdf");
    ctx.documentTitle = QStringLiteral("Annual Report");

    QString t1 = DecorationTemplateEngine::evaluate(QStringLiteral("Page {page} of {pages}"), ctx);
    QCOMPARE(t1, QStringLiteral("Page 3 of 10"));

    QString t2 = DecorationTemplateEngine::evaluate(QStringLiteral("Doc: {title} ({filename})"), ctx);
    QCOMPARE(t2, QStringLiteral("Doc: Annual Report (report.pdf)"));

    QString t3 = DecorationTemplateEngine::evaluate(QStringLiteral("ID-{page:000}"), ctx);
    QCOMPARE(t3, QStringLiteral("ID-003"));
}

void Tst_PageDecorationTest::testDecorationDefinitions()
{
    HeaderFooterDefinition hf;
    hf.setSlot(HeaderFooterSlot::BottomCenter, QStringLiteral("{page} / {pages}"));
    QCOMPARE(hf.bottomCenter.enabled, true);
    QCOMPARE(hf.bottomCenter.templateText, QStringLiteral("{page} / {pages}"));

    WatermarkDefinition wm;
    wm.text = QStringLiteral("GİZLİ");
    QCOMPARE(wm.text, QStringLiteral("GİZLİ"));
    QCOMPARE(wm.opacity(), 0.3);

    BackgroundDefinition bg;
    bg.solidColor = Qt::cyan;
    QCOMPARE(bg.isBehindContent(), true);
}

QTEST_MAIN(Tst_PageDecorationTest)
#include "tst_pagedecorationtest.moc"
