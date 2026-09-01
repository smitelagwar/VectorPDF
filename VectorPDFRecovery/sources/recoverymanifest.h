// MIT License
#ifndef VECTORPDF_RECOVERYMANIFEST_H
#define VECTORPDF_RECOVERYMANIFEST_H

#include "recoveryglobal.h"
#include "recoverytypes.h"
#include <QJsonObject>
#include <QJsonDocument>

namespace vectorpdf::recovery
{

class VECTORPDF_RECOVERY_EXPORT RecoveryManifest
{
public:
    static QJsonObject serialize(const RecoveryMetadata& metadata);
    static RecoveryMetadata deserialize(const QJsonObject& json);
    static bool writeToFile(const QString& filePath, const RecoveryMetadata& metadata);
    static bool readFromFile(const QString& filePath, RecoveryMetadata& metadataOut);
};

} // namespace vectorpdf::recovery

#endif // VECTORPDF_RECOVERYMANIFEST_H
