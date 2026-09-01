// MIT License
#include "recoverymanifest.h"
#include <QFile>

namespace vectorpdf::recovery
{

QJsonObject RecoveryManifest::serialize(const RecoveryMetadata& metadata)
{
    QJsonObject obj;
    obj[QStringLiteral("sessionId")] = metadata.sessionId.toString();
    obj[QStringLiteral("originalFilePath")] = metadata.originalFilePath;
    obj[QStringLiteral("documentTitle")] = metadata.documentTitle;
    obj[QStringLiteral("sessionStartTime")] = metadata.sessionStartTime.toString(Qt::ISODate);
    obj[QStringLiteral("lastAutosaveTime")] = metadata.lastAutosaveTime.toString(Qt::ISODate);
    obj[QStringLiteral("uncommittedActionsCount")] = metadata.uncommittedActionsCount;
    obj[QStringLiteral("state")] = static_cast<int>(metadata.state);
    obj[QStringLiteral("originalFileSize")] = metadata.originalFileSize;
    obj[QStringLiteral("tempSnapshotFilePath")] = metadata.tempSnapshotFilePath;
    obj[QStringLiteral("isEncrypted")] = metadata.isEncrypted;
    return obj;
}

RecoveryMetadata RecoveryManifest::deserialize(const QJsonObject& json)
{
    RecoveryMetadata m;
    m.sessionId = QUuid::fromString(json[QStringLiteral("sessionId")].toString());
    m.originalFilePath = json[QStringLiteral("originalFilePath")].toString();
    m.documentTitle = json[QStringLiteral("documentTitle")].toString();
    m.sessionStartTime = QDateTime::fromString(json[QStringLiteral("sessionStartTime")].toString(), Qt::ISODate);
    m.lastAutosaveTime = QDateTime::fromString(json[QStringLiteral("lastAutosaveTime")].toString(), Qt::ISODate);
    m.uncommittedActionsCount = json[QStringLiteral("uncommittedActionsCount")].toInt();
    m.state = static_cast<RecoverySessionState>(json[QStringLiteral("state")].toInt());
    m.originalFileSize = json[QStringLiteral("originalFileSize")].toInteger();
    m.tempSnapshotFilePath = json[QStringLiteral("tempSnapshotFilePath")].toString();
    m.isEncrypted = json[QStringLiteral("isEncrypted")].toBool();
    return m;
}

bool RecoveryManifest::writeToFile(const QString& filePath, const RecoveryMetadata& metadata)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return false;

    QJsonObject obj = serialize(metadata);
    QJsonDocument doc(obj);
    file.write(doc.toJson(QJsonDocument::Indented));
    return true;
}

bool RecoveryManifest::readFromFile(const QString& filePath, RecoveryMetadata& metadataOut)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
        return false;

    QByteArray bytes = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(bytes);
    if (doc.isNull() || !doc.isObject())
        return false;

    metadataOut = deserialize(doc.object());
    return true;
}

} // namespace vectorpdf::recovery
