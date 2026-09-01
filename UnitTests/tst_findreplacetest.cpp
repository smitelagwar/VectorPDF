// MIT License
// Copyright (c) 2026 VectorPDF Contributors
#include <QtTest>
#include "contenttools/findreplacequery.h"
#include "contenttools/findreplacematch.h"
#include "contenttools/findreplaceanalyzer.h"
#include "contenttools/findreplacetransaction.h"

using namespace vectorpdf::tools::content;

class Tst_FindReplaceTest : public QObject
{
    Q_OBJECT

private slots:
    void testTurkishCasePreservation();
    void testTurkishLocaleMatching();
    void testTransactionModel();
};

void Tst_FindReplaceTest::testTurkishCasePreservation()
{
    QString res1 = FindReplaceAnalyzer::computeReplacementWithPreservedCase(QStringLiteral("istanbul"), QStringLiteral("ankara"));
    QCOMPARE(res1, QStringLiteral("ankara"));

    QString res2 = FindReplaceAnalyzer::computeReplacementWithPreservedCase(QStringLiteral("İSTANBUL"), QStringLiteral("ankara"));
    QCOMPARE(res2, QStringLiteral("ANKARA"));

    QString res3 = FindReplaceAnalyzer::computeReplacementWithPreservedCase(QStringLiteral("İstanbul"), QStringLiteral("ankara"));
    QCOMPARE(res3, QStringLiteral("Ankara"));
}

void Tst_FindReplaceTest::testTurkishLocaleMatching()
{
    bool m1 = FindReplaceAnalyzer::isTurkishLocaleAwareMatch(QStringLiteral("İSTANBUL TİCARET"), QStringLiteral("istanbul"), false);
    QCOMPARE(m1, true);

    bool m2 = FindReplaceAnalyzer::isTurkishLocaleAwareMatch(QStringLiteral("ışık"), QStringLiteral("IŞIK"), false);
    QCOMPARE(m2, true);
}

void Tst_FindReplaceTest::testTransactionModel()
{
    QList<FindReplaceMatch> matches;
    FindReplaceMatch m;
    m.matchedText = QStringLiteral("old");
    m.proposedReplacement = QStringLiteral("new");
    m.selected = true;
    matches.append(m);

    FindReplaceTransaction tr(matches);
    QCOMPARE(tr.name(), QStringLiteral("Find & Replace Transaction"));
}

QTEST_MAIN(Tst_FindReplaceTest)
#include "tst_findreplacetest.moc"
