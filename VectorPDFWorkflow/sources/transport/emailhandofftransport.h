// MIT License
#ifndef VECTORPDF_EMAILHANDOFFTRANSPORT_H
#define VECTORPDF_EMAILHANDOFFTRANSPORT_H

#include "iworkflowtransport.h"

namespace vectorpdf::workflow
{

class VECTORPDF_WORKFLOW_EXPORT EmailHandoffTransport : public IWorkflowTransport
{
public:
    QString transportId() const override { return QStringLiteral("transport.email"); }
    bool sendPackage(const QString& packagePath, const QString& destinationTarget) override;
    bool checkUpdates(const QString& sourceTarget, QString& updatePathOut) override;
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_EMAILHANDOFFTRANSPORT_H
