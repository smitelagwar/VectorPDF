// MIT License
#include "datedetector.h"
#include <QRegularExpression>

namespace vectorpdf::tools::redact
{

QList<RedactionCandidate> DateDetector::detect(const QString& pageText, int pageIndex, const QRectF& defaultRect)
{
    QList<RedactionCandidate> candidates;
    QRegularExpression dateRe(QStringLiteral(r"\b(?:(0[1-9]|[12][0-9]|3[01])[-/.](0[1-9]|1[012])[-/.](19|20)\d\d|(19|20)\d\d[-/.](0[1-9]|1[012])[-/.](0[1-9]|[12][0-9]|3[01]))\b"));
    QRegularExpressionMatchIterator it = dateRe.globalMatch(pageText);

    while (it.hasNext())
    {
        QRegularExpressionMatch match = it.next();
        QString val = match.captured(0);
        RedactionCandidate c;
        c.detectorId = id();
        c.detectorName = displayName();
        c.rawText = val;
        c.normalizedValue = val;
        c.pageIndex = pageIndex;
        c.boundingRect = defaultRect;
        c.confidence = 0.85;
        c.reason = QStringLiteral("Valid date pattern");
        c.selected = false;
        candidates.append(c);
    }

    return candidates;
}

} // namespace vectorpdf::tools::redact
