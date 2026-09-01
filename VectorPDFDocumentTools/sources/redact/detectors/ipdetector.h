// MIT License
#ifndef VECTORPDF_IPDETECTOR_H
#define VECTORPDF_IPDETECTOR_H

#include "../iredactiondetector.h"

namespace vectorpdf::tools::redact
{

class VECTORPDF_DOCUMENTTOOLS_EXPORT IpDetector : public IRedactionDetector
{
public:
    QString id() const override { return QStringLiteral("detector.ip"); }
    QString displayName() const override { return QStringLiteral("IP Address (v4/v6)"); }
    QList<RedactionCandidate> detect(const QString& pageText, int pageIndex, const QRectF& defaultRect = QRectF(72, 72, 200, 20)) override;
};

} // namespace vectorpdf::tools::redact

#endif // VECTORPDF_IPDETECTOR_H
