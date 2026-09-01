// MIT License
#ifndef VECTORPDF_BATESSEQUENCE_H
#define VECTORPDF_BATESSEQUENCE_H

#include "../documenttoolsglobal.h"
#include <QString>

namespace vectorpdf::tools::bates
{

class VECTORPDF_DOCUMENTTOOLS_EXPORT BatesSequence
{
public:
    BatesSequence(const QString& prefix = QStringLiteral("CASE-"), int startNumber = 1, int numDigits = 6, const QString& suffix = QString());

    QString nextNumber();
    QString currentNumber() const;
    void reset(int startNumber);
    int currentRawNumber() const { return m_current; }

private:
    QString m_prefix;
    int m_current;
    int m_digits;
    QString m_suffix;
};

} // namespace vectorpdf::tools::bates

#endif // VECTORPDF_BATESSEQUENCE_H
