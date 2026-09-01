// MIT License
#ifndef VECTORPDF_FILESYSTEMTRANSPORT_H
#define VECTORPDF_FILESYSTEMTRANSPORT_H

#include "iworkflowtransport.h"

namespace vectorpdf::workflow
{

class VECTORPDF_WORKFLOW_EXPORT FileSystemTransport : public IWorkflowTransport
{
public:
    QString transportId() const override { return QStringLiteral("transport.filesystem"); }
    bool sendPackage(const QString& packagePath, const QString& destinationTarget) override;
    bool checkUpdates(const QString& sourceTarget, QString& updatePathOut) override;
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_FILESYSTEMTRANSPORT_H
