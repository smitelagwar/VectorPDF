// MIT License
#include "ibandetector.h"
#include <QRegularExpression>

namespace vectorpdf::tools::redact
{

bool IbanDetector::validateIban(const QString& rawIban)
{
    QString iban;
    for (QChar c : rawIban)
    {
        if (c.isLetterOrNumber())
            iban.append(c.toUpper());
    }

    if (iban.length() < 15 || iban.length() > 34)
        return false;

    QString rearranged = iban.mid(4) + iban.left(4);
    QString numericString;
    for (QChar c : rearranged)
    {
        if (c.isDigit())
        {
            numericString.append(c);
        }
        else if (c >= 'A' && c <= 'Z')
        {
            numericString.append(QString::number(c.unicode() - 'A' + 10));
        }
        else
        {
            return false;
        }
    }

    int remainder = 0;
    for (int i = 0; i < numericString.length(); ++i)
    {
        remainder = (remainder * 10 + numericString.at(i).digitValue()) % 97;
    }

    return remainder == 1;
}

QList<RedactionCandidate> IbanDetector::detect(const QString& pageText, int pageIndex, const QRectF& defaultRect)
{
    QList<RedactionCandidate> candidates;
    QRegularExpression ibanRe(QStringLiteral(r"\b[A-Z]{2}\d{2}[A-Za-z0-9\s]{11,30}\b"));
    QRegularExpressionMatchIterator it = ibanRe.globalMatch(pageText);

    while (it.hasNext())
    {
        QRegularExpressionMatch match = it.next();
        QString val = match.captured(0);
        if (validateIban(val))
        {
            RedactionCandidate c;
            c.detectorId = id();
            c.detectorName = displayName();
            c.rawText = val;
            c.normalizedValue = val.simplified().remove(' ');
            c.pageIndex = pageIndex;
            c.boundingRect = defaultRect;
            c.confidence = 0.98;
            c.reason = QStringLiteral("Valid ISO 13616 IBAN MOD-97 checksum");
            c.selected = true;
            candidates.append(c);
        }
    }

    return candidates;
}

} // namespace vectorpdf::tools::redact
