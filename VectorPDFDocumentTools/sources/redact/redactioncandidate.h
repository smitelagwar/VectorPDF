// MIT License
#ifndef VECTORPDF_REDACTIONCANDIDATE_H
#define VECTORPDF_REDACTIONCANDIDATE_H

#include "../documenttoolsglobal.h"
#include <QString>
#include <QRectF>

namespace vectorpdf::tools::redact
{

struct VECTORPDF_DOCUMENTTOOLS_EXPORT RedactionCandidate
{
    QString detectorId;
    QString detectorName;
    QString rawText;
    QString normalizedValue;
    int pageIndex = 0;
    QRectF boundingRect;
    double confidence = 1.0;
    QString reason;
    bool selected = true;
};

} // namespace vectorpdf::tools::redact

#endif // VECTORPDF_REDACTIONCANDIDATE_H
