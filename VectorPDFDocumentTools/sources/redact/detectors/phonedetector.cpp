// MIT License
#include "phonedetector.h"
#include <QRegularExpression>

namespace vectorpdf::tools::redact
{

QList<RedactionCandidate> PhoneDetector::detect(const QString& pageText, int pageIndex, const QRectF& defaultRect)
{
    QList<RedactionCandidate> candidates;
    QRegularExpression phoneRe(QStringLiteral(r"(?:\+90|0)?\s*\(?5\d{2}\)?[-.\s]*\d{3}[-.\s]*\d{2}[-.\s]*\d{2}|\b\+?\d{1,3}[-.\s]?\(?\d{3}\)?[-.\s]?\d{3}[-.\s]?\d{4}\b"));
    QRegularExpressionMatchIterator it = phoneRe.globalMatch(pageText);

    while (it.hasNext())
    {
        QRegularExpressionMatch match = it.next();
        QString val = match.captured(0);
        QString digitsOnly;
        for (QChar ch : val)
        {
            if (ch.isDigit())
                digitsOnly.append(ch);
        }

        if (digitsOnly.length() >= 10 && digitsOnly.length() <= 15)
        {
            RedactionCandidate c;
            c.detectorId = id();
            c.detectorName = displayName();
            c.rawText = val;
            c.normalizedValue = digitsOnly;
            c.pageIndex = pageIndex;
            c.boundingRect = defaultRect;
            c.confidence = 0.90;
            c.reason = QStringLiteral("Phone number candidate");
            c.selected = true;
            candidates.append(c);
        }
    }

    return candidates;
}

} // namespace vectorpdf::tools::redact
