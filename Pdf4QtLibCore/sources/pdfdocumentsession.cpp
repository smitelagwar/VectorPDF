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
#include "pdfcatalog.h"
#include "pdfform.h"

#include <QFileInfo>
#include <QDir>
#include <QSaveFile>

namespace pdf
{

PDFDocumentSession::PDFDocumentSession() :
    m_document(nullptr),
    m_state(State::Closed),
    m_isReadOnly(false),
    m_isSigned(false)
{
}

PDFDocumentSession::~PDFDocumentSession()
{
    detachDocument();
}

void PDFDocumentSession::attachDocument(std::shared_ptr<PDFDocument> document, const QString& filePath, bool isReadOnly)
{
    m_document = document;
    m_filePath = filePath;
    m_isReadOnly = isReadOnly;
    m_openedAt = QDateTime::currentDateTime();
    m_lastSavedAt = m_openedAt;

    if (m_document)
    {
        updateSignedStatus();
        m_state = State::Clean;
    }
    else
    {
        m_state = State::Closed;
        m_isSigned = false;
    }
}

void PDFDocumentSession::detachDocument()
{
    m_document.reset();
    m_filePath.clear();
    m_recoveryPath.clear();
    m_state = State::Closed;
    m_isReadOnly = false;
    m_isSigned = false;
}

PDFDocument* PDFDocumentSession::getDocument() const
{
    return m_document.get();
}

std::shared_ptr<PDFDocument> PDFDocumentSession::getDocumentPtr() const
{
    return m_document;
}

QString PDFDocumentSession::getFilePath() const
{
    return m_filePath;
}

bool PDFDocumentSession::isOpen() const
{
    return m_document != nullptr && m_state != State::Closed;
}

PDFDocumentSession::State PDFDocumentSession::getState() const
{
    return m_state;
}

bool PDFDocumentSession::isModified() const
{
    return m_state == State::Modified;
}

bool PDFDocumentSession::isReadOnly() const
{
    return m_isReadOnly;
}

bool PDFDocumentSession::isSigned() const
{
    return m_isSigned;
}

void PDFDocumentSession::markModified()
{
    if (m_state != State::Closed)
    {
        m_state = State::Modified;
    }
}

void PDFDocumentSession::markClean()
{
    if (m_state != State::Closed)
    {
        m_state = State::Clean;
    }
}

void PDFDocumentSession::setReadOnly(bool readOnly)
{
    m_isReadOnly = readOnly;
}

bool PDFDocumentSession::canSaveInPlace() const
{
    if (!isOpen() || m_filePath.isEmpty())
    {
        return false;
    }

    if (m_isReadOnly)
    {
        return false;
    }

    // Signed documents require saving as a new revision or separate copy to prevent accidental invalidation
    if (m_isSigned)
    {
        return false;
    }

    QFileInfo fileInfo(m_filePath);
    return fileInfo.isWritable() || !fileInfo.exists();
}

PDFOperationResult PDFDocumentSession::saveAtomic(const QString& targetPath)
{
    if (!isOpen())
    {
        return tr("No document is open in this session.");
    }

    const QString destination = targetPath.isEmpty() ? m_filePath : targetPath;
    if (destination.isEmpty())
    {
        return tr("No valid destination file path specified.");
    }

    if (destination == m_filePath && m_isSigned)
    {
        return tr("Document contains digital signatures. Direct in-place overwriting is prohibited to protect cryptographic validity. Please save as a new file.");
    }

    if (destination == m_filePath && m_isReadOnly)
    {
        return tr("Document is opened in read-only mode. Direct in-place overwriting is prohibited.");
    }

    m_state = State::Saving;

    PDFDocumentWriter writer(nullptr);
    PDFOperationResult writeResult = writer.write(destination, m_document.get(), true);

    if (writeResult)
    {
        // Reopen validation check: ensure file exists and is accessible
        QFileInfo writtenFile(destination);
        if (!writtenFile.exists() || writtenFile.size() == 0)
        {
            m_state = State::Error;
            return tr("Atomic save verification failed: output file is missing or empty.");
        }

        m_lastSavedAt = QDateTime::currentDateTime();
        if (targetPath.isEmpty() || targetPath == m_filePath)
        {
            m_state = State::Clean;
        }
        else
        {
            // If saved as a new file path, update session file path
            m_filePath = destination;
            m_state = State::Clean;
            updateSignedStatus();
        }
        return PDFOperationResult();
    }
    else
    {
        m_state = State::Error;
        return writeResult;
    }
}

QString PDFDocumentSession::getRecoveryPath() const
{
    return m_recoveryPath;
}

void PDFDocumentSession::updateSignedStatus()
{
    m_isSigned = false;
    if (!m_document)
    {
        return;
    }

    const PDFCatalog* catalog = m_document->getCatalog();
    if (catalog)
    {
        const PDFForm* form = catalog->getForm();
        if (form && form->hasSignatures())
        {
            m_isSigned = true;
        }
    }
}

} // namespace pdf
