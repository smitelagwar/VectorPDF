// MIT License
#include "workflowpackagewriter.h"
#include "workflowpackagemanifest.h"
#include <QFile>
#include <QJsonDocument>
#include <QFileInfo>
#include <QDir>

namespace vectorpdf::workflow
{

bool WorkflowPackageWriter::writePackage(const QString& destinationPath, const QString& sourcePdfPath, const WorkflowMetadata& meta, const QList<WorkflowParticipant>& participants)
{
    QFileInfo info(destinationPath);
    QDir().mkpath(info.absolutePath());

    QFile file(destinationPath);
    if (!file.open(QIODevice::WriteOnly))
        return false;

    QJsonObject obj = WorkflowPackageManifest::serialize(meta, participants);
    obj[QStringLiteral("pdfSource")] = QFileInfo(sourcePdfPath).fileName();

    QJsonDocument doc(obj);
    file.write(doc.toJson(QJsonDocument::Indented));
    return true;
}

} // namespace vectorpdf::workflow
