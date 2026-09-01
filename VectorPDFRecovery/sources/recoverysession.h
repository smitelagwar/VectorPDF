// MIT License
#ifndef VECTORPDF_RECOVERYSESSION_H
#define VECTORPDF_RECOVERYSESSION_H

#include "recoveryglobal.h"
#include "recoverytypes.h"
#include "recoveryjournal.h"
#include <memory>

namespace pdf { class PDFDocument; }

namespace vectorpdf::recovery
{

class VECTORPDF_RECOVERY_EXPORT RecoverySession
{
public:
    RecoverySession(const QString& originalFilePath, pdf::PDFDocument* document);
    ~RecoverySession();

    QUuid sessionId() const { return m_metadata.sessionId; }
    RecoveryMetadata metadata() const { return m_metadata; }

    bool recordAction(const QString& actionName, const QString& details = QString());
    bool performAutosave();
    void closeCleanly();

private:
    RecoveryMetadata m_metadata;
    pdf::PDFDocument* m_document = nullptr;
    std::unique_ptr<RecoveryJournal> m_journal;
    QString m_sessionDir;
};

} // namespace vectorpdf::recovery

#endif // VECTORPDF_RECOVERYSESSION_H
