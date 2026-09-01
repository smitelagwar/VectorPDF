// MIT License
#include "workflowpackagereader.h"
#include "workflowpackagemanifest.h"
#include <QFile>
#include <QJsonDocument>

namespace vectorpdf::workflow
{

bool WorkflowPackageReader::readPackage(const QString& packagePath, WorkflowMetadata& metaOut, QList<WorkflowParticipant>& participantsOut)
{
    QFile file(packagePath);
    if (!file.open(QIODevice::ReadOnly))
        return false;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (doc.isNull() || !doc.isObject())
        return false;

    return WorkflowPackageManifest::deserialize(doc.object(), metaOut, participantsOut);
}

} // namespace vectorpdf::workflow
