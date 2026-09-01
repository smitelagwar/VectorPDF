// MIT License
#ifndef VECTORPDF_DATEDETECTOR_H
#define VECTORPDF_DATEDETECTOR_H

#include "../iredactiondetector.h"

namespace vectorpdf::tools::redact
{

class VECTORPDF_DOCUMENTTOOLS_EXPORT DateDetector : public IRedactionDetector
{
public:
    QString id() const override { return QStringLiteral("detector.date"); }
    QString displayName() const override { return QStringLiteral("Date"); }
    QList<RedactionCandidate> detect(const QString& pageText, int pageIndex, const QRectF& defaultRect = QRectF(72, 72, 200, 20)) override;
};

} // namespace vectorpdf::tools::redact

#endif // VECTORPDF_DATEDETECTOR_H
