// MIT License
#include "emaildetector.h"
#include <QRegularExpression>

namespace vectorpdf::tools::redact
{

QList<RedactionCandidate> EmailDetector::detect(const QString& pageText, int pageIndex, const QRectF& defaultRect)
{
    QList<RedactionCandidate> candidates;
    QRegularExpression emailRe(QStringLiteral(r"\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}\b"));
    QRegularExpressionMatchIterator it = emailRe.globalMatch(pageText);

    while (it.hasNext())
    {
        QRegularExpressionMatch match = it.next();
        RedactionCandidate c;
        c.detectorId = id();
        c.detectorName = displayName();
        c.rawText = match.captured(0);
        c.normalizedValue = c.rawText.toLower();
        c.pageIndex = pageIndex;
        c.boundingRect = defaultRect;
        c.confidence = 0.95;
        c.reason = QStringLiteral("Valid e-mail pattern match");
        c.selected = true;
        candidates.append(c);
    }

    return candidates;
}

} // namespace vectorpdf::tools::redact
