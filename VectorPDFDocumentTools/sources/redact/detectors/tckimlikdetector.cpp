// MIT License
#include "tckimlikdetector.h"
#include <QRegularExpression>

namespace vectorpdf::tools::redact
{

bool TcKimlikDetector::validateChecksum(const QString& tc)
{
    if (tc.length() != 11)
        return false;

    if (tc.at(0) == '0')
        return false;

    int d[11];
    for (int i = 0; i < 11; ++i)
    {
        if (!tc.at(i).isDigit())
            return false;
        d[i] = tc.at(i).digitValue();
    }

    int oddSum = d[0] + d[2] + d[4] + d[6] + d[8];
    int evenSum = d[1] + d[3] + d[5] + d[7];
    int d10 = ((oddSum * 7) - evenSum) % 10;
    if (d10 < 0)
        d10 += 10;

    if (d[9] != d10)
        return false;

    int first10Sum = 0;
    for (int i = 0; i < 10; ++i)
        first10Sum += d[i];

    if (d[10] != (first10Sum % 10))
        return false;

    return true;
}

QList<RedactionCandidate> TcKimlikDetector::detect(const QString& pageText, int pageIndex, const QRectF& defaultRect)
{
    QList<RedactionCandidate> candidates;
    QRegularExpression tcRe(QStringLiteral(r"\b[1-9]\d{10}\b"));
    QRegularExpressionMatchIterator it = tcRe.globalMatch(pageText);

    while (it.hasNext())
    {
        QRegularExpressionMatch match = it.next();
        QString val = match.captured(0);
        if (validateChecksum(val))
        {
            RedactionCandidate c;
            c.detectorId = id();
            c.detectorName = displayName();
            c.rawText = val;
            c.normalizedValue = val;
            c.pageIndex = pageIndex;
            c.boundingRect = defaultRect;
            c.confidence = 0.99;
            c.reason = QStringLiteral("T.C. Kimlik No verified with official checksum");
            c.selected = true;
            candidates.append(c);
        }
    }

    return candidates;
}

} // namespace vectorpdf::tools::redact
