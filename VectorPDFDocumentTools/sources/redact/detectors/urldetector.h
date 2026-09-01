// MIT License
#ifndef VECTORPDF_URLDETECTOR_H
#define VECTORPDF_URLDETECTOR_H

#include "../iredactiondetector.h"

namespace vectorpdf::tools::redact
{

class VECTORPDF_DOCUMENTTOOLS_EXPORT UrlDetector : public IRedactionDetector
{
public:
    QString id() const override { return QStringLiteral("detector.url"); }
    QString displayName() const override { return QStringLiteral("URL / Web Link"); }
    QList<RedactionCandidate> detect(const QString& pageText, int pageIndex, const QRectF& defaultRect = QRectF(72, 72, 200, 20)) override;
};

} // namespace vectorpdf::tools::redact

#endif // VECTORPDF_URLDETECTOR_H
