// MIT License
// Copyright (c) 2026 VectorPDF Contributors
#include <QtTest>
#include "autotag/autotagcandidate.h"
#include "autotag/autotagplan.h"
#include "autotag/alttextsuggester.h"
#include "validators/verapdfuaworker.h"

using namespace vectorpdf::a11y;

class Tst_AutoTagTest : public QObject
{
    Q_OBJECT

private slots:
    void testAutoTagPlanMetrics();
    void testAltTextSuggester();
    void testVeraPdfWorkerAvailability();
};

void Tst_AutoTagTest::testAutoTagPlanMetrics()
{
    AutoTagPlan plan;
    AutoTagCandidate h1;
    h1.tagType = StructureTagType::H1;
    plan.candidates.append(h1);

    AutoTagCandidate p1;
    p1.tagType = StructureTagType::P;
    plan.candidates.append(p1);

    AutoTagCandidate art;
    art.tagType = StructureTagType::Artifact;
    art.isArtifactCandidate = true;
    plan.candidates.append(art);

    plan.updateMetrics();
    QCOMPARE(plan.headingCount, 1);
    QCOMPARE(plan.paragraphCount, 1);
    QCOMPARE(plan.artifactCount, 1);
}

void Tst_AutoTagTest::testAltTextSuggester()
{
    QImage img(100, 100, QImage::Format_RGB32);
    img.fill(Qt::white);

    QString alt = AltTextSuggester::suggestAltText(img, QStringLiteral("Sales figures 2026"));
    QVERIFY(!alt.isEmpty());
    QVERIFY(alt.contains(QStringLiteral("Sales figures 2026")));
}

void Tst_AutoTagTest::testVeraPdfWorkerAvailability()
{
    PdfUaValidationReport rep = VeraPdfUaWorker::validatePdfUa(QStringLiteral("test.pdf"));
    QCOMPARE(rep.validatorEngineName, QStringLiteral("veraPDF PDF/UA-1 Validator"));
}

QTEST_MAIN(Tst_AutoTagTest)
#include "tst_autotagtest.moc"
