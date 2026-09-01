// MIT License
#include "ipdetector.h"
#include <QRegularExpression>

namespace vectorpdf::tools::redact
{

QList<RedactionCandidate> IpDetector::detect(const QString& pageText, int pageIndex, const QRectF& defaultRect)
{
    QList<RedactionCandidate> candidates;
    QRegularExpression ipv4Re(QStringLiteral(r"\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\b"));
    QRegularExpressionMatchIterator it = ipv4Re.globalMatch(pageText);

    while (it.hasNext())
    {
        QRegularExpressionMatch match = it.next();
        RedactionCandidate c;
        c.detectorId = id();
        c.detectorName = displayName();
        c.rawText = match.captured(0);
        c.normalizedValue = c.rawText;
        c.pageIndex = pageIndex;
        c.boundingRect = defaultRect;
        c.confidence = 0.90;
        c.reason = QStringLiteral("Valid IPv4 address");
        c.selected = true;
        candidates.append(c);
    }

    return candidates;
}

} // namespace vectorpdf::tools::redact
