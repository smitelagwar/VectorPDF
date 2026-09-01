// MIT License
#ifndef VECTORPDF_TCKIMLIKDETECTOR_H
#define VECTORPDF_TCKIMLIKDETECTOR_H

#include "../iredactiondetector.h"

namespace vectorpdf::tools::redact
{

class VECTORPDF_DOCUMENTTOOLS_EXPORT TcKimlikDetector : public IRedactionDetector
{
public:
    QString id() const override { return QStringLiteral("detector.tckimlik"); }
    QString displayName() const override { return QStringLiteral("T.C. Kimlik Numarası"); }
    QList<RedactionCandidate> detect(const QString& pageText, int pageIndex, const QRectF& defaultRect = QRectF(72, 72, 200, 20)) override;

    static bool validateChecksum(const QString& tcString);
};

} // namespace vectorpdf::tools::redact

#endif // VECTORPDF_TCKIMLIKDETECTOR_H
