// MIT License
#ifndef VECTORPDF_WEBDAVTRANSPORT_H
#define VECTORPDF_WEBDAVTRANSPORT_H

#include "iworkflowtransport.h"

namespace vectorpdf::workflow
{

class VECTORPDF_WORKFLOW_EXPORT WebDavTransport : public IWorkflowTransport
{
public:
    QString transportId() const override { return QStringLiteral("transport.webdav"); }
    bool sendPackage(const QString& packagePath, const QString& destinationTarget) override;
    bool checkUpdates(const QString& sourceTarget, QString& updatePathOut) override;
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_WEBDAVTRANSPORT_H
