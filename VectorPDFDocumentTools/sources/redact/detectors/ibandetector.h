// MIT License
#ifndef VECTORPDF_IBANDETECTOR_H
#define VECTORPDF_IBANDETECTOR_H

#include "../iredactiondetector.h"

namespace vectorpdf::tools::redact
{

class VECTORPDF_DOCUMENTTOOLS_EXPORT IbanDetector : public IRedactionDetector
{
public:
    QString id() const override { return QStringLiteral("detector.iban"); }
    QString displayName() const override { return QStringLiteral("IBAN Bank Account"); }
    QList<RedactionCandidate> detect(const QString& pageText, int pageIndex, const QRectF& defaultRect = QRectF(72, 72, 200, 20)) override;

    static bool validateIban(const QString& iban);
};

} // namespace vectorpdf::tools::redact

#endif // VECTORPDF_IBANDETECTOR_H
