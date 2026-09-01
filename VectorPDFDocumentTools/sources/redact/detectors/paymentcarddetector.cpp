// MIT License
#include "paymentcarddetector.h"
#include <QRegularExpression>

namespace vectorpdf::tools::redact
{

bool PaymentCardDetector::validateLuhn(const QString& cardNumber)
{
    QString digits;
    for (QChar c : cardNumber)
    {
        if (c.isDigit())
            digits.append(c);
    }

    if (digits.length() < 11 || digits.length() > 19)
        return false;

    int sum = 0;
    bool alternate = false;
    for (int i = digits.length() - 1; i >= 0; --i)
    {
        int n = digits.at(i).digitValue();
        if (alternate)
        {
            n *= 2;
            if (n > 9)
                n -= 9;
        }
        sum += n;
        alternate = !alternate;
    }

    return (sum % 10 == 0);
}

QList<RedactionCandidate> PaymentCardDetector::detect(const QString& pageText, int pageIndex, const QRectF& defaultRect)
{
    QList<RedactionCandidate> candidates;
    QRegularExpression cardRe(QStringLiteral(r"\b(?:\d[ -]*?){13,19}\b"));
    QRegularExpressionMatchIterator it = cardRe.globalMatch(pageText);

    while (it.hasNext())
    {
        QRegularExpressionMatch match = it.next();
        QString val = match.captured(0);
        if (validateLuhn(val))
        {
            RedactionCandidate c;
            c.detectorId = id();
            c.detectorName = displayName();
            c.rawText = val;
            c.normalizedValue = val.simplified().remove(' ').remove('-');
            c.pageIndex = pageIndex;
            c.boundingRect = defaultRect;
            c.confidence = 0.95;
            c.reason = QStringLiteral("Valid Payment Card (Luhn MOD-10 verified)");
            c.selected = true;
            candidates.append(c);
        }
    }

    return candidates;
}

} // namespace vectorpdf::tools::redact
