// MIT License
#include "recoveryjournal.h"
#include <QFile>
#include <QTextStream>

namespace vectorpdf::recovery
{

RecoveryJournal::RecoveryJournal(const QString& journalFilePath)
    : m_filePath(journalFilePath)
{
}

bool RecoveryJournal::appendEntry(const QString& actionName, const QString& details)
{
    QFile file(m_filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
        return false;

    QTextStream out(&file);
    m_counter++;
    out << m_counter << "|" << QDateTime::currentDateTime().toString(Qt::ISODate) << "|" << actionName << "|" << details << "
";
    return true;
}

QList<RecoveryJournalEntry> RecoveryJournal::readAllEntries() const
{
    QList<RecoveryJournalEntry> list;
    QFile file(m_filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return list;

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        QStringList parts = line.split('|');
        if (parts.size() >= 3)
        {
            RecoveryJournalEntry entry;
            entry.actionIndex = parts[0].toInt();
            entry.timestamp = QDateTime::fromString(parts[1], Qt::ISODate);
            entry.actionName = parts[2];
            if (parts.size() >= 4)
                entry.details = parts[3];
            list.append(entry);
        }
    }
    return list;
}

void RecoveryJournal::clear()
{
    QFile::remove(m_filePath);
    m_counter = 0;
}

} // namespace vectorpdf::recovery
