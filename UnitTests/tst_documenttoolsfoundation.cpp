// MIT License
// Copyright (c) 2026 VectorPDF Contributors
#include <QtTest>
#include "documenttooltypes.h"
#include "documenttoolresult.h"
#include "documenttoolvalidator.h"

using namespace vectorpdf::tools;

class Tst_DocumentToolsFoundation : public QObject
{
    Q_OBJECT

private slots:
    void testCoordinateTransform();
    void testResultModel();
    void testValidator();
};

void Tst_DocumentToolsFoundation::testCoordinateTransform()
{
    QRectF pageBounds(0, 0, 595, 842);
    QTransform t0 = PageCoordinateHelper::pageTransform(0, pageBounds);
    QCOMPARE(t0.isIdentity(), true);

    QTransform t90 = PageCoordinateHelper::pageTransform(90, pageBounds);
    QCOMPARE(t90.isIdentity(), false);

    QPointF pt(10, 20);
    QPointF mapped0 = PageCoordinateHelper::mapToRotatedPage(pt, 0, pageBounds);
    QCOMPARE(mapped0, pt);

    QRectF box = PageCoordinateHelper::resolvePageBox(pageBounds, QRectF());
    QCOMPARE(box, pageBounds);
}

void Tst_DocumentToolsFoundation::testResultModel()
{
    DocumentToolResult r1 = DocumentToolResult::success(QStringLiteral("OK"));
    QCOMPARE(r1.isSuccessful(), true);
    QCOMPARE(r1.status, ToolExecutionStatus::Success);

    DocumentToolResult r2 = DocumentToolResult::failure(QStringLiteral("Fail"));
    QCOMPARE(r2.isSuccessful(), false);
    QCOMPARE(r2.status, ToolExecutionStatus::Failed);

    DocumentToolResult r3 = DocumentToolResult::cancelled();
    QCOMPARE(r3.status, ToolExecutionStatus::Cancelled);
}

void Tst_DocumentToolsFoundation::testValidator()
{
    DocumentToolResult nullVal = DocumentToolValidator::validateMutationAllowed(nullptr);
    QCOMPARE(nullVal.isSuccessful(), false);
    QCOMPARE(nullVal.status, ToolExecutionStatus::ValidationFailed);
}

QTEST_MAIN(Tst_DocumentToolsFoundation)
#include "tst_documenttoolsfoundation.moc"
