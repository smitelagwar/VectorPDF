// MIT License
#include "turkishplatedetector.h"
#include <QRegularExpression>

namespace vectorpdf::tools::redact
{

QList<RedactionCandidate> TurkishPlateDetector::detect(const QString& pageText, int pageIndex, const QRectF& defaultRect)
{
    QList<RedactionCandidate> candidates;
    QRegularExpression plateRe(QStringLiteral(r"\b(0[1-9]|[1-7][0-9]|8[01])\s*([A-Z]{1,3})\s*(\d{2,5})\b"));
    QRegularExpressionMatchIterator it = plateRe.globalMatch(pageText);

    while (it.hasNext())
    {
        QRegularExpressionMatch match = it.next();
        QString val = match.captured(0);
        RedactionCandidate c;
        c.detectorId = id();
        c.detectorName = displayName();
        c.rawText = val;
        c.normalizedValue = QStringLiteral("%1 %2 %3").arg(match.captured(1), match.captured(2), match.captured(3));
        c.pageIndex = pageIndex;
        c.boundingRect = defaultRect;
        c.confidence = 0.80;
        c.reason = QStringLiteral("Turkish vehicle registration plate candidate");
        c.selected = false;
        candidates.append(c);
    }

    return candidates;
}

} // namespace vectorpdf::tools::redact
