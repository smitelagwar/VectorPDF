// MIT License
#include "workflowpackagemanifest.h"
#include <QJsonArray>

namespace vectorpdf::workflow
{

QJsonObject WorkflowPackageManifest::serialize(const WorkflowMetadata& meta, const QList<WorkflowParticipant>& participants)
{
    QJsonObject obj;
    obj[QStringLiteral("workflowId")] = meta.workflowId.toString();
    obj[QStringLiteral("type")] = static_cast<int>(meta.type);
    obj[QStringLiteral("title")] = meta.title;
    obj[QStringLiteral("initiatorEmail")] = meta.initiatorEmail;
    obj[QStringLiteral("initiatorName")] = meta.initiatorName;
    obj[QStringLiteral("createdAt")] = meta.createdAt.toString(Qt::ISODate);
    obj[QStringLiteral("status")] = static_cast<int>(meta.status);

    QJsonArray pArr;
    for (const auto& p : participants)
    {
        QJsonObject pObj;
        pObj[QStringLiteral("id")] = p.participantId.toString();
        pObj[QStringLiteral("email")] = p.email;
        pObj[QStringLiteral("name")] = p.name;
        pObj[QStringLiteral("role")] = static_cast<int>(p.role);
        pObj[QStringLiteral("order")] = p.sequenceOrder;
        pObj[QStringLiteral("hasCompleted")] = p.hasCompleted;
        pArr.append(pObj);
    }
    obj[QStringLiteral("participants")] = pArr;

    return obj;
}

bool WorkflowPackageManifest::deserialize(const QJsonObject& json, WorkflowMetadata& metaOut, QList<WorkflowParticipant>& participantsOut)
{
    metaOut.workflowId = QUuid::fromString(json[QStringLiteral("workflowId")].toString());
    metaOut.type = static_cast<WorkflowType>(json[QStringLiteral("type")].toInt());
    metaOut.title = json[QStringLiteral("title")].toString();
    metaOut.initiatorEmail = json[QStringLiteral("initiatorEmail")].toString();
    metaOut.initiatorName = json[QStringLiteral("initiatorName")].toString();
    metaOut.createdAt = QDateTime::fromString(json[QStringLiteral("createdAt")].toString(), Qt::ISODate);
    metaOut.status = static_cast<WorkflowStatus>(json[QStringLiteral("status")].toInt());

    participantsOut.clear();
    QJsonArray pArr = json[QStringLiteral("participants")].toArray();
    for (const auto& val : pArr)
    {
        QJsonObject pObj = val.toObject();
        WorkflowParticipant p;
        p.participantId = QUuid::fromString(pObj[QStringLiteral("id")].toString());
        p.email = pObj[QStringLiteral("email")].toString();
        p.name = pObj[QStringLiteral("name")].toString();
        p.role = static_cast<ParticipantRole>(pObj[QStringLiteral("role")].toInt());
        p.sequenceOrder = pObj[QStringLiteral("order")].toInt();
        p.hasCompleted = pObj[QStringLiteral("hasCompleted")].toBool();
        participantsOut.append(p);
    }
    return true;
}

} // namespace vectorpdf::workflow
