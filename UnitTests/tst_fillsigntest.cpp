// MIT License
// Copyright (c) 2026 VectorPDF Contributors
#include <QtTest>
#include "fillsign/signatureasset.h"
#include "fillsign/signatureassetstore.h"
#include "fillsign/fillsignappearancebuilder.h"
#include "fillsign/fillsignplacementmodel.h"

using namespace vectorpdf::tools::fillsign;

class Tst_FillSignTest : public QObject
{
    Q_OBJECT

private slots:
    void testAssetSerialization();
    void testAssetStore();
    void testPlacementModel();
    void testAppearanceBuilder();
};

void Tst_FillSignTest::testAssetSerialization()
{
    SignatureAsset a;
    a.id = QUuid::createUuid();
    a.type = SignatureAssetType::Typed;
    a.displayName = QStringLiteral("Ahmet Yılmaz");
    a.typedText = QStringLiteral("Ahmet Yılmaz");
    a.fontPresetName = QStringLiteral("Formal Cursive");
    a.isInitial = false;

    QByteArray bytes = a.serialize();
    QVERIFY(!bytes.isEmpty());

    SignatureAsset restored = SignatureAsset::deserialize(bytes);
    QCOMPARE(restored.id, a.id);
    QCOMPARE(restored.type, a.type);
    QCOMPARE(restored.displayName, a.displayName);
    QCOMPARE(restored.typedText, a.typedText);
}

void Tst_FillSignTest::testAssetStore()
{
    SignatureAssetStore& store = SignatureAssetStore::instance();
    store.clearAll(false);

    SignatureAsset s1;
    s1.id = QUuid::createUuid();
    s1.displayName = QStringLiteral("Sig 1");
    s1.isInitial = false;
    store.addAsset(s1, false);

    SignatureAsset i1;
    i1.id = QUuid::createUuid();
    i1.displayName = QStringLiteral("Init 1");
    i1.isInitial = true;
    store.addAsset(i1, false);

    QCOMPARE(store.signatures().size(), 1);
    QCOMPARE(store.initials().size(), 1);

    store.removeAsset(s1.id);
    QCOMPARE(store.signatures().size(), 0);
}

void Tst_FillSignTest::testPlacementModel()
{
    FillSignPlacementModel model;
    VisualSignaturePlacement p;
    p.placementId = QUuid::createUuid();
    p.pageIndex = 0;
    p.pageBoundingRect = QRectF(100, 100, 150, 50);

    model.addPlacement(p);
    QCOMPARE(model.placementsForPage(0).size(), 1);
    QCOMPARE(model.placementsForPage(1).size(), 0);

    model.nudgePlacement(p.placementId, 10, 5);
    QCOMPARE(model.placementsForPage(0).at(0).pageBoundingRect.x(), 110.0);
}

void Tst_FillSignTest::testAppearanceBuilder()
{
    SignatureAsset a;
    a.type = SignatureAssetType::Typed;
    a.typedText = QStringLiteral("Test User");
    QImage img = FillSignAppearanceBuilder::renderToImage(a, QSizeF(200, 80));
    QVERIFY(!img.isNull());
    QCOMPARE(img.width(), 400); // dpr = 2.0
}

QTEST_MAIN(Tst_FillSignTest)
#include "tst_fillsigntest.moc"
