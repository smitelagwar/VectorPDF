// MIT License
#ifndef VECTORPDF_REDACTIONPATTERNREGISTRY_H
#define VECTORPDF_REDACTIONPATTERNREGISTRY_H

#include "../documenttoolsglobal.h"
#include "iredactiondetector.h"
#include <memory>
#include <QList>
#include <QMap>

namespace vectorpdf::tools::redact
{

class VECTORPDF_DOCUMENTTOOLS_EXPORT RedactionPatternRegistry
{
public:
    static RedactionPatternRegistry& instance();

    void registerDetector(std::shared_ptr<IRedactionDetector> detector);
    std::shared_ptr<IRedactionDetector> detector(const QString& id) const;
    QList<std::shared_ptr<IRedactionDetector>> allDetectors() const;

    QList<RedactionCandidate> scanText(const QString& text, int pageIndex, const QStringList& enabledDetectorIds = QStringList());

private:
    RedactionPatternRegistry();
    QMap<QString, std::shared_ptr<IRedactionDetector>> m_detectors;
};

} // namespace vectorpdf::tools::redact

#endif // VECTORPDF_REDACTIONPATTERNREGISTRY_H
