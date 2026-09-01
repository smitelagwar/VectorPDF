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

#ifndef PDFDOCUMENTSESSION_H
#define PDFDOCUMENTSESSION_H

#include "pdfglobal.h"
#include "pdfdocument.h"
#include "pdfdocumentwriter.h"
#include "pdfutils.h"

#include <QString>
#include <QDateTime>
#include <memory>
#include <functional>

namespace pdf
{

/// Manages the operational lifecycle, dirty state, signed document protection,
/// and verified atomic saving of a PDF document in VectorPDF.
class PDF4QTLIBCORESHARED_EXPORT PDFDocumentSession
{
    Q_DECLARE_TR_FUNCTIONS(pdf::PDFDocumentSession)

public:
    enum class State
    {
        Closed,
        Clean,
        Modified,
        Saving,
        Error
    };

    explicit PDFDocumentSession();
    virtual ~PDFDocumentSession();

    /// Attach a loaded document to this session
    void attachDocument(std::shared_ptr<PDFDocument> document, const QString& filePath, bool isReadOnly = false);

    /// Detach document and reset session state
    void detachDocument();

    /// Returns current document pointer
    PDFDocument* getDocument() const;

    /// Returns shared pointer to current document
    std::shared_ptr<PDFDocument> getDocumentPtr() const;

    /// File path of the document
    QString getFilePath() const;

    /// True if document is currently loaded
    bool isOpen() const;

    /// Session state
    State getState() const;

    /// True if document has unsaved modifications
    bool isModified() const;

    /// True if opened in read-only mode or file cannot be written to
    bool isReadOnly() const;

    /// True if document contains digital signatures
    bool isSigned() const;

    /// Marks the document as modified (dirty)
    void markModified();

    /// Marks the document as clean (synced with storage)
    void markClean();

    /// Sets read-only mode
    void setReadOnly(bool readOnly);

    /// Checks whether in-place save is safe and allowed
    bool canSaveInPlace() const;

    /// Atomically saves the document to destination path with verification reopen.
    /// If targetPath is empty, saves to original filePath.
    PDFOperationResult saveAtomic(const QString& targetPath = QString());

    /// Recovery file path associated with this session
    QString getRecoveryPath() const;

private:
    std::shared_ptr<PDFDocument> m_document;
    QString m_filePath;
    QString m_recoveryPath;
    State m_state;
    bool m_isReadOnly;
    bool m_isSigned;
    QDateTime m_openedAt;
    QDateTime m_lastSavedAt;

    void updateSignedStatus();
};

} // namespace pdf

#endif // PDFDOCUMENTSESSION_H
