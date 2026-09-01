// MIT License
#ifndef VECTORPDF_PAYMENTCARDDETECTOR_H
#define VECTORPDF_PAYMENTCARDDETECTOR_H

#include "../iredactiondetector.h"

namespace vectorpdf::tools::redact
{

class VECTORPDF_DOCUMENTTOOLS_EXPORT PaymentCardDetector : public IRedactionDetector
{
public:
    QString id() const override { return QStringLiteral("detector.paymentcard"); }
    QString displayName() const override { return QStringLiteral("Credit/Debit Card"); }
    QList<RedactionCandidate> detect(const QString& pageText, int pageIndex, const QRectF& defaultRect = QRectF(72, 72, 200, 20)) override;

    static bool validateLuhn(const QString& cardNumber);
};

} // namespace vectorpdf::tools::redact

#endif // VECTORPDF_PAYMENTCARDDETECTOR_H
