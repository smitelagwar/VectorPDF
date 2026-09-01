// MIT License
// Copyright (c) 2026 VectorPDF Contributors
#include <QtTest>
#include "accessibilitytypes.h"
#include "accessibilityissue.h"
#include "accessibilityreport.h"
#include "accessibilitychecker.h"

using namespace vectorpdf::a11y;

class Tst_AccessibilityCheckerTest : public QObject
{
    Q_OBJECT

private slots:
    void testTypesAndTagStrings();
    void testReportSummary();
    void testCheckerOnNullDocument();
};

void Tst_AccessibilityCheckerTest::testTypesAndTagStrings()
{
    QCOMPARE(tagTypeToString(StructureTagType::H1), QStringLiteral("H1"));
    QCOMPARE(tagTypeToString(StructureTagType::Table), QStringLiteral("Table"));
    QCOMPARE(stringToTagType(QStringLiteral("Figure")), StructureTagType::Figure);
}

void Tst_AccessibilityCheckerTest::testReportSummary()
{
    AccessibilityReport report;
    AccessibilityIssue i1;
    i1.status = AccessibilityIssueStatus::Fail;
    report.issues.append(i1);

    AccessibilityIssue i2;
    i2.status = AccessibilityIssueStatus::Warning;
    report.issues.append(i2);

    report.recalculateSummary();
    QCOMPARE(report.failureCount, 1);
    QCOMPARE(report.warningCount, 1);
    QCOMPARE(report.isCompliant(), false);
}

void Tst_AccessibilityCheckerTest::testCheckerOnNullDocument()
{
    AccessibilityReport rep = AccessibilityChecker::checkDocument(nullptr);
    QCOMPARE(rep.issues.size(), 0);
}

QTEST_MAIN(Tst_AccessibilityCheckerTest)
#include "tst_accessibilitycheckertest.moc"
