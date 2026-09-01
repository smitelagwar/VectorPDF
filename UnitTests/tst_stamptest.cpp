// MIT License
// Copyright (c) 2026 VectorPDF Contributors
#include <QtTest>
#include "stamps/stampdefinition.h"
#include "stamps/stampassetstore.h"
#include "stamps/stampappearancebuilder.h"

using namespace vectorpdf::tools::stamps;

class Tst_StampTest : public QObject
{
    Q_OBJECT

private slots:
    void testAssetStore();
    void testAppearanceBuilder();
};

void Tst_StampTest::testAssetStore()
{
    StampAssetStore& store = StampAssetStore::instance();
    QVERIFY(store.standardStamps().size() >= 3);

    StampDefinition custom;
    custom.metadata.name = QStringLiteral("ÖZEL ONAY");
    store.addCustomStamp(custom);
    QCOMPARE(store.customStamps().size(), 1);

    store.removeCustomStamp(custom.metadata.stampId);
    QCOMPARE(store.customStamps().size(), 0);
}

void Tst_StampTest::testAppearanceBuilder()
{
    StampDefinition def;
    def.mainText = QStringLiteral("ONAYLANDI");
    def.renderType = StampRenderType::DynamicTimestamped;

    QImage img = StampAppearanceBuilder::renderStampToImage(def, 2.0);
    QVERIFY(!img.isNull());
    QCOMPARE(img.width(), def.preferredSize.width() * 2);
}

QTEST_MAIN(Tst_StampTest)
#include "tst_stamptest.moc"
