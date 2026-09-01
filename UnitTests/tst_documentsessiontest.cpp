// MIT License
//
// Copyright (c) 2026 VectorPDF Project
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "pdfdocumentsession.h"

#include <QtTest>

class DocumentSessionTest : public QObject
{
    Q_OBJECT

public:
    DocumentSessionTest() = default;
    ~DocumentSessionTest() = default;

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testInitialState();
    void testModifiedStateTracking();
    void testReadOnlyProtection();
};

void DocumentSessionTest::initTestCase()
{
}

void DocumentSessionTest::cleanupTestCase()
{
}

void DocumentSessionTest::testInitialState()
{
    pdf::PDFDocumentSession session;
    QCOMPARE(session.isOpen(), false);
    QCOMPARE(session.getState(), pdf::PDFDocumentSession::State::Closed);
    QCOMPARE(session.isModified(), false);
    QCOMPARE(session.isReadOnly(), false);
    QCOMPARE(session.isSigned(), false);
    QCOMPARE(session.canSaveInPlace(), false);
}

void DocumentSessionTest::testModifiedStateTracking()
{
    pdf::PDFDocumentSession session;
    // Without document, markModified is safe no-op
    session.markModified();
    QCOMPARE(session.isModified(), false);

    session.markClean();
    QCOMPARE(session.isModified(), false);
}

void DocumentSessionTest::testReadOnlyProtection()
{
    pdf::PDFDocumentSession session;
    session.setReadOnly(true);
    QCOMPARE(session.isReadOnly(), true);
    QCOMPARE(session.canSaveInPlace(), false);

    session.setReadOnly(false);
    QCOMPARE(session.isReadOnly(), false);
}

QTEST_MAIN(DocumentSessionTest)

#include "tst_documentsessiontest.moc"
