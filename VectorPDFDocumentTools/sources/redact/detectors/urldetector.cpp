// MIT License
#include "urldetector.h"
#include <QRegularExpression>

namespace vectorpdf::tools::redact
{

QList<RedactionCandidate> UrlDetector::detect(const QString& pageText, int pageIndex, const QRectF& defaultRect)
{
    QList<RedactionCandidate> candidates;
    QRegularExpression urlRe(QStringLiteral(r"https?://(?:www\.)?[-a-zA-Z0-9@:%._+~#=]{1,256}\.[a-zA-Z0-9()]{1,6}\b(?:[-a-zA-Z0-9()@:%_+.~#?&/=]*)"));
    QRegularExpressionMatchIterator it = urlRe.globalMatch(pageText);

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
        c.reason = QStringLiteral("Web URL match");
        c.selected = true;
        candidates.append(c);
    }

    return candidates;
}

} // namespace vectorpdf::tools::redact
