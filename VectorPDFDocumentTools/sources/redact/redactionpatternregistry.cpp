// MIT License
#include "redactionpatternregistry.h"
#include "detectors/emaildetector.h"
#include "detectors/phonedetector.h"
#include "detectors/tckimlikdetector.h"
#include "detectors/ibandetector.h"
#include "detectors/paymentcarddetector.h"
#include "detectors/ipdetector.h"
#include "detectors/urldetector.h"
#include "detectors/datedetector.h"
#include "detectors/turkishplatedetector.h"

namespace vectorpdf::tools::redact
{

RedactionPatternRegistry& RedactionPatternRegistry::instance()
{
    static RedactionPatternRegistry reg;
    return reg;
}

RedactionPatternRegistry::RedactionPatternRegistry()
{
    registerDetector(std::make_shared<EmailDetector>());
    registerDetector(std::make_shared<PhoneDetector>());
    registerDetector(std::make_shared<TcKimlikDetector>());
    registerDetector(std::make_shared<IbanDetector>());
    registerDetector(std::make_shared<PaymentCardDetector>());
    registerDetector(std::make_shared<IpDetector>());
    registerDetector(std::make_shared<UrlDetector>());
    registerDetector(std::make_shared<DateDetector>());
    registerDetector(std::make_shared<TurkishPlateDetector>());
}

void RedactionPatternRegistry::registerDetector(std::shared_ptr<IRedactionDetector> detector)
{
    if (detector)
    {
        m_detectors[detector->id()] = detector;
    }
}

std::shared_ptr<IRedactionDetector> RedactionPatternRegistry::detector(const QString& id) const
{
    return m_detectors.value(id);
}

QList<std::shared_ptr<IRedactionDetector>> RedactionPatternRegistry::allDetectors() const
{
    return m_detectors.values();
}

QList<RedactionCandidate> RedactionPatternRegistry::scanText(const QString& text, int pageIndex, const QStringList& enabledDetectorIds)
{
    QList<RedactionCandidate> results;
    for (const auto& det : m_detectors)
    {
        if (enabledDetectorIds.isEmpty() || enabledDetectorIds.contains(det->id()))
        {
            results.append(det->detect(text, pageIndex));
        }
    }
    return results;
}

} // namespace vectorpdf::tools::redact
