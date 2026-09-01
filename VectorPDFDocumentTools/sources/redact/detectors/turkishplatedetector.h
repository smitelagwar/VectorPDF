// MIT License
#ifndef VECTORPDF_TURKISHPLATEDETECTOR_H
#define VECTORPDF_TURKISHPLATEDETECTOR_H

#include "../iredactiondetector.h"

namespace vectorpdf::tools::redact
{

class VECTORPDF_DOCUMENTTOOLS_EXPORT TurkishPlateDetector : public IRedactionDetector
{
public:
    QString id() const override { return QStringLiteral("detector.plate"); }
    QString displayName() const override { return QStringLiteral("Turkish Vehicle Plate"); }
    QList<RedactionCandidate> detect(const QString& pageText, int pageIndex, const QRectF& defaultRect = QRectF(72, 72, 200, 20)) override;
};

} // namespace vectorpdf::tools::redact

#endif // VECTORPDF_TURKISHPLATEDETECTOR_H
