// MIT License
#ifndef VECTORPDF_EMAILDETECTOR_H
#define VECTORPDF_EMAILDETECTOR_H

#include "../iredactiondetector.h"

namespace vectorpdf::tools::redact
{

class VECTORPDF_DOCUMENTTOOLS_EXPORT EmailDetector : public IRedactionDetector
{
public:
    QString id() const override { return QStringLiteral("detector.email"); }
    QString displayName() const override { return QStringLiteral("E-mail Address"); }
    QList<RedactionCandidate> detect(const QString& pageText, int pageIndex, const QRectF& defaultRect = QRectF(72, 72, 200, 20)) override;
};

} // namespace vectorpdf::tools::redact

#endif // VECTORPDF_EMAILDETECTOR_H
