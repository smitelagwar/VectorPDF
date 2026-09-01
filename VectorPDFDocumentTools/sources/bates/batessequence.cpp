// MIT License
#include "batessequence.h"

namespace vectorpdf::tools::bates
{

BatesSequence::BatesSequence(const QString& prefix, int startNumber, int numDigits, const QString& suffix)
    : m_prefix(prefix)
    , m_current(startNumber)
    , m_digits(numDigits)
    , m_suffix(suffix)
{
}

QString BatesSequence::nextNumber()
{
    QString numStr = QString("%1").arg(m_current, m_digits, 10, QChar('0'));
    QString result = m_prefix + numStr + m_suffix;
    m_current++;
    return result;
}

QString BatesSequence::currentNumber() const
{
    QString numStr = QString("%1").arg(m_current, m_digits, 10, QChar('0'));
    return m_prefix + numStr + m_suffix;
}

void BatesSequence::reset(int startNumber)
{
    m_current = startNumber;
}

} // namespace vectorpdf::tools::bates
