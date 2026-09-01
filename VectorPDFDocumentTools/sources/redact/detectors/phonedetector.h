// MIT License
#ifndef VECTORPDF_PHONEDETECTOR_H
#define VECTORPDF_PHONEDETECTOR_H

#include "../iredactiondetector.h"

namespace vectorpdf::tools::redact
{

class VECTORPDF_DOCUMENTTOOLS_EXPORT PhoneDetector : public IRedactionDetector
{
public:
    QString id() const override { return QStringLiteral("detector.phone"); }
    QString displayName() const override { return QStringLiteral("Phone Number"); }
    QList<RedactionCandidate> detect(const QString& pageText, int pageIndex, const QRectF& defaultRect = QRectF(72, 72, 200, 20)) override;
};

} // namespace vectorpdf::tools::redact

#endif // VECTORPDF_PHONEDETECTOR_H
