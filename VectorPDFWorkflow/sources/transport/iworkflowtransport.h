// MIT License
#ifndef VECTORPDF_IWORKFLOWTRANSPORT_H
#define VECTORPDF_IWORKFLOWTRANSPORT_H

#include "../workflowglobal.h"
#include <QString>

namespace vectorpdf::workflow
{

class VECTORPDF_WORKFLOW_EXPORT IWorkflowTransport
{
public:
    virtual ~IWorkflowTransport() = default;
    virtual QString transportId() const = 0;
    virtual bool sendPackage(const QString& packagePath, const QString& destinationTarget) = 0;
    virtual bool checkUpdates(const QString& sourceTarget, QString& updatePathOut) = 0;
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_IWORKFLOWTRANSPORT_H
