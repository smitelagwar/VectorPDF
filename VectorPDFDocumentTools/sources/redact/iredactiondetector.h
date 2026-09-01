// MIT License
#ifndef VECTORPDF_IREDACTIONDETECTOR_H
#define VECTORPDF_IREDACTIONDETECTOR_H

#include "../documenttoolsglobal.h"
#include "redactioncandidate.h"
#include <QString>
#include <QList>

namespace vectorpdf::tools::redact
{

class VECTORPDF_DOCUMENTTOOLS_EXPORT IRedactionDetector
{
public:
    virtual ~IRedactionDetector() = default;

    virtual QString id() const = 0;
    virtual QString displayName() const = 0;
    virtual QList<RedactionCandidate> detect(const QString& pageText, int pageIndex, const QRectF& defaultRect = QRectF(72, 72, 200, 20)) = 0;
};

} // namespace vectorpdf::tools::redact

#endif // VECTORPDF_IREDACTIONDETECTOR_H
