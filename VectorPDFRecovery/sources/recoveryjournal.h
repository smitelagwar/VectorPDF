// MIT License
#ifndef VECTORPDF_RECOVERYJOURNAL_H
#define VECTORPDF_RECOVERYJOURNAL_H

#include "recoveryglobal.h"
#include <QString>
#include <QDateTime>

namespace vectorpdf::recovery
{

struct RecoveryJournalEntry
{
    int actionIndex = 0;
    QString actionName;
    QDateTime timestamp = QDateTime::currentDateTime();
    QString details;
};

class VECTORPDF_RECOVERY_EXPORT RecoveryJournal
{
public:
    explicit RecoveryJournal(const QString& journalFilePath);

    bool appendEntry(const QString& actionName, const QString& details = QString());
    QList<RecoveryJournalEntry> readAllEntries() const;
    void clear();

private:
    QString m_filePath;
    int m_counter = 0;
};

} // namespace vectorpdf::recovery

#endif // VECTORPDF_RECOVERYJOURNAL_H
