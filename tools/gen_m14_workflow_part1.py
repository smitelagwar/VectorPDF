import os

def w(path, text):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "w", encoding="utf-8") as out:
        out.write(text.strip() + "\n")
    print("Created:", path)

# 1. Global & Base Types
w("VectorPDFWorkflow/sources/workflowglobal.h", """// MIT License
#ifndef VECTORPDF_WORKFLOWGLOBAL_H
#define VECTORPDF_WORKFLOWGLOBAL_H

#include <QtCore/qglobal.h>

#if defined(VECTORPDF_WORKFLOW_LIBRARY)
#  define VECTORPDF_WORKFLOW_EXPORT Q_DECL_EXPORT
#else
#  define VECTORPDF_WORKFLOW_EXPORT Q_DECL_IMPORT
#endif

#endif // VECTORPDF_WORKFLOWGLOBAL_H
""")

w("VectorPDFWorkflow/sources/workflowtypes.h", """// MIT License
#ifndef VECTORPDF_WORKFLOWTYPES_H
#define VECTORPDF_WORKFLOWTYPES_H

#include "workflowglobal.h"
#include <QString>
#include <QDateTime>
#include <QUuid>

namespace vectorpdf::workflow
{

enum class WorkflowType
{
    SignatureRequest,
    SharedReview,
    ApprovalChain
};

enum class WorkflowStatus
{
    Draft,
    Active,
    Pending,
    Completed,
    Rejected,
    Expired,
    Cancelled
};

enum class ParticipantRole
{
    Signer,
    Reviewer,
    Approver,
    Observer
};

struct VECTORPDF_WORKFLOW_EXPORT WorkflowMetadata
{
    QUuid workflowId = QUuid::createUuid();
    WorkflowType type = WorkflowType::SignatureRequest;
    QString title;
    QString initiatorEmail;
    QString initiatorName;
    QDateTime createdAt = QDateTime::currentDateTime();
    QDateTime dueDate;
    WorkflowStatus status = WorkflowStatus::Draft;
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_WORKFLOWTYPES_H
""")

w("VectorPDFWorkflow/sources/workflowtypes.cpp", """// MIT License
#include "workflowtypes.h"

namespace vectorpdf::workflow
{
}
""")

# 2. Identity
w("VectorPDFWorkflow/sources/identity/workflowparticipant.h", """// MIT License
#ifndef VECTORPDF_WORKFLOWPARTICIPANT_H
#define VECTORPDF_WORKFLOWPARTICIPANT_H

#include "../workflowglobal.h"
#include "../workflowtypes.h"
#include <QString>
#include <QUuid>

namespace vectorpdf::workflow
{

struct VECTORPDF_WORKFLOW_EXPORT WorkflowParticipant
{
    QUuid participantId = QUuid::createUuid();
    QString email;
    QString name;
    ParticipantRole role = ParticipantRole::Signer;
    int sequenceOrder = 1;
    bool hasCompleted = false;
    QDateTime completedAt;
    QString signatureId;
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_WORKFLOWPARTICIPANT_H
""")

w("VectorPDFWorkflow/sources/identity/workflowparticipant.cpp", """// MIT License
#include "workflowparticipant.h"

namespace vectorpdf::workflow
{
}
""")

w("VectorPDFWorkflow/sources/identity/participantresolver.h", """// MIT License
#ifndef VECTORPDF_PARTICIPANTRESOLVER_H
#define VECTORPDF_PARTICIPANTRESOLVER_H

#include "../workflowglobal.h"
#include "workflowparticipant.h"
#include <QList>

namespace vectorpdf::workflow
{

class VECTORPDF_WORKFLOW_EXPORT ParticipantResolver
{
public:
    static WorkflowParticipant currentLocalUser();
    static bool validateEmail(const QString& email);
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_PARTICIPANTRESOLVER_H
""")

w("VectorPDFWorkflow/sources/identity/participantresolver.cpp", """// MIT License
#include "participantresolver.h"
#include <QRegularExpression>

namespace vectorpdf::workflow
{

WorkflowParticipant ParticipantResolver::currentLocalUser()
{
    WorkflowParticipant p;
    p.name = QStringLiteral("Yerel Kullanıcı");
    p.email = QStringLiteral("user@local.device");
    p.role = ParticipantRole::Reviewer;
    return p;
}

bool ParticipantResolver::validateEmail(const QString& email)
{
    QRegularExpression re(QStringLiteral(r"^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$"));
    return re.match(email.trimmed()).hasMatch();
}

} // namespace vectorpdf::workflow
""")

# 3. Package & Manifest
w("VectorPDFWorkflow/sources/package/workflowpackagemanifest.h", """// MIT License
#ifndef VECTORPDF_WORKFLOWPACKAGEMANIFEST_H
#define VECTORPDF_WORKFLOWPACKAGEMANIFEST_H

#include "../workflowglobal.h"
#include "../workflowtypes.h"
#include "../identity/workflowparticipant.h"
#include <QJsonObject>
#include <QList>

namespace vectorpdf::workflow
{

class VECTORPDF_WORKFLOW_EXPORT WorkflowPackageManifest
{
public:
    static QJsonObject serialize(const WorkflowMetadata& meta, const QList<WorkflowParticipant>& participants);
    static bool deserialize(const QJsonObject& json, WorkflowMetadata& metaOut, QList<WorkflowParticipant>& participantsOut);
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_WORKFLOWPACKAGEMANIFEST_H
""")

w("VectorPDFWorkflow/sources/package/workflowpackagemanifest.cpp", """// MIT License
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
""")

w("VectorPDFWorkflow/sources/package/workflowpackagewriter.h", """// MIT License
#ifndef VECTORPDF_WORKFLOWPACKAGEWRITER_H
#define VECTORPDF_WORKFLOWPACKAGEWRITER_H

#include "../workflowglobal.h"
#include "../workflowtypes.h"
#include "../identity/workflowparticipant.h"
#include <QString>
#include <QList>

namespace vectorpdf::workflow
{

class VECTORPDF_WORKFLOW_EXPORT WorkflowPackageWriter
{
public:
    static bool writePackage(const QString& destinationPath, const QString& sourcePdfPath, const WorkflowMetadata& meta, const QList<WorkflowParticipant>& participants);
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_WORKFLOWPACKAGEWRITER_H
""")

w("VectorPDFWorkflow/sources/package/workflowpackagewriter.cpp", """// MIT License
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
""")

w("VectorPDFWorkflow/sources/package/workflowpackagereader.h", """// MIT License
#ifndef VECTORPDF_WORKFLOWPACKAGEREADER_H
#define VECTORPDF_WORKFLOWPACKAGEREADER_H

#include "../workflowglobal.h"
#include "../workflowtypes.h"
#include "../identity/workflowparticipant.h"
#include <QString>
#include <QList>

namespace vectorpdf::workflow
{

class VECTORPDF_WORKFLOW_EXPORT WorkflowPackageReader
{
public:
    static bool readPackage(const QString& packagePath, WorkflowMetadata& metaOut, QList<WorkflowParticipant>& participantsOut);
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_WORKFLOWPACKAGEREADER_H
""")

w("VectorPDFWorkflow/sources/package/workflowpackagereader.cpp", """// MIT License
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
""")

w("VectorPDFWorkflow/sources/package/workflowpackagevalidator.h", """// MIT License
#ifndef VECTORPDF_WORKFLOWPACKAGEVALIDATOR_H
#define VECTORPDF_WORKFLOWPACKAGEVALIDATOR_H

#include "../workflowglobal.h"
#include <QString>

namespace vectorpdf::workflow
{

class VECTORPDF_WORKFLOW_EXPORT WorkflowPackageValidator
{
public:
    static bool validatePackage(const QString& packagePath);
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_WORKFLOWPACKAGEVALIDATOR_H
""")

w("VectorPDFWorkflow/sources/package/workflowpackagevalidator.cpp", """// MIT License
#include "workflowpackagevalidator.h"
#include "workflowpackagereader.h"

namespace vectorpdf::workflow
{

bool WorkflowPackageValidator::validatePackage(const QString& packagePath)
{
    WorkflowMetadata meta;
    QList<WorkflowParticipant> parts;
    return WorkflowPackageReader::readPackage(packagePath, meta, parts);
}

} // namespace vectorpdf::workflow
""")

# 4. Sign Request
w("VectorPDFWorkflow/sources/signrequest/signrequestfield.h", """// MIT License
#ifndef VECTORPDF_SIGNREQUESTFIELD_H
#define VECTORPDF_SIGNREQUESTFIELD_H

#include "../workflowglobal.h"
#include <QString>
#include <QRectF>
#include <QUuid>

namespace vectorpdf::workflow
{

enum class SignFieldType
{
    Signature,
    Initials,
    Date,
    Text,
    Checkbox
};

struct VECTORPDF_WORKFLOW_EXPORT SignRequestField
{
    QUuid fieldId = QUuid::createUuid();
    QUuid assignedParticipantId;
    int pageIndex = 0;
    QRectF pageRect;
    SignFieldType type = SignFieldType::Signature;
    bool required = true;
    QString label;
    QString filledValue;
    bool isFilled = false;
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_SIGNREQUESTFIELD_H
""")

w("VectorPDFWorkflow/sources/signrequest/signrequestfield.cpp", """// MIT License
#include "signrequestfield.h"

namespace vectorpdf::workflow
{
}
""")

w("VectorPDFWorkflow/sources/signrequest/signrequestdefinition.h", """// MIT License
#ifndef VECTORPDF_SIGNREQUESTDEFINITION_H
#define VECTORPDF_SIGNREQUESTDEFINITION_H

#include "../workflowglobal.h"
#include "../workflowtypes.h"
#include "../identity/workflowparticipant.h"
#include "signrequestfield.h"
#include <QList>

namespace vectorpdf::workflow
{

struct VECTORPDF_WORKFLOW_EXPORT SignRequestDefinition
{
    WorkflowMetadata metadata;
    QList<WorkflowParticipant> participants;
    QList<SignRequestField> fields;
    QString messageForRecipients;
    bool sequentialSigning = true;
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_SIGNREQUESTDEFINITION_H
""")

w("VectorPDFWorkflow/sources/signrequest/signrequestdefinition.cpp", """// MIT License
#include "signrequestdefinition.h"

namespace vectorpdf::workflow
{
}
""")

w("VectorPDFWorkflow/sources/signrequest/signrequeststate.h", """// MIT License
#ifndef VECTORPDF_SIGNREQUESTSTATE_H
#define VECTORPDF_SIGNREQUESTSTATE_H

#include "../workflowglobal.h"
#include "../workflowtypes.h"

namespace vectorpdf::workflow
{

struct VECTORPDF_WORKFLOW_EXPORT SignRequestState
{
    WorkflowStatus status = WorkflowStatus::Draft;
    int totalFields = 0;
    int filledFields = 0;
    int totalParticipants = 0;
    int completedParticipants = 0;
    int activeSequenceIndex = 1;

    double progressPercentage() const
    {
        if (totalFields == 0) return 0.0;
        return (static_cast<double>(filledFields) / totalFields) * 100.0;
    }
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_SIGNREQUESTSTATE_H
""")

w("VectorPDFWorkflow/sources/signrequest/signrequeststate.cpp", """// MIT License
#include "signrequeststate.h"

namespace vectorpdf::workflow
{
}
""")

w("VectorPDFWorkflow/sources/signrequest/signrequestservice.h", """// MIT License
#ifndef VECTORPDF_SIGNREQUESTSERVICE_H
#define VECTORPDF_SIGNREQUESTSERVICE_H

#include "../workflowglobal.h"
#include "signrequestdefinition.h"
#include "signrequeststate.h"

namespace vectorpdf::workflow
{

class VECTORPDF_WORKFLOW_EXPORT SignRequestService
{
public:
    static bool initiateSignRequest(const SignRequestDefinition& def, const QString& destinationPackagePath);
    static SignRequestState evaluateState(const SignRequestDefinition& def);
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_SIGNREQUESTSERVICE_H
""")

w("VectorPDFWorkflow/sources/signrequest/signrequestservice.cpp", """// MIT License
#include "signrequestservice.h"
#include "../package/workflowpackagewriter.h"

namespace vectorpdf::workflow
{

bool SignRequestService::initiateSignRequest(const SignRequestDefinition& def, const QString& destinationPackagePath)
{
    return WorkflowPackageWriter::writePackage(destinationPackagePath, QString(), def.metadata, def.participants);
}

SignRequestState SignRequestService::evaluateState(const SignRequestDefinition& def)
{
    SignRequestState state;
    state.status = def.metadata.status;
    state.totalFields = def.fields.size();
    state.totalParticipants = def.participants.size();

    for (const auto& f : def.fields)
    {
        if (f.isFilled)
            state.filledFields++;
    }

    for (const auto& p : def.participants)
    {
        if (p.hasCompleted)
            state.completedParticipants++;
    }

    return state;
}

} // namespace vectorpdf::workflow
""")

w("VectorPDFWorkflow/sources/signrequest/signresponseimporter.h", """// MIT License
#ifndef VECTORPDF_SIGNRESPONSEIMPORTER_H
#define VECTORPDF_SIGNRESPONSEIMPORTER_H

#include "../workflowglobal.h"
#include "signrequestdefinition.h"

namespace vectorpdf::workflow
{

class VECTORPDF_WORKFLOW_EXPORT SignResponseImporter
{
public:
    static bool importResponse(SignRequestDefinition& def, const QUuid& participantId, const QString& signatureId);
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_SIGNRESPONSEIMPORTER_H
""")

w("VectorPDFWorkflow/sources/signrequest/signresponseimporter.cpp", """// MIT License
#include "signresponseimporter.h"

namespace vectorpdf::workflow
{

bool SignResponseImporter::importResponse(SignRequestDefinition& def, const QUuid& participantId, const QString& signatureId)
{
    for (auto& p : def.participants)
    {
        if (p.participantId == participantId)
        {
            p.hasCompleted = true;
            p.completedAt = QDateTime::currentDateTime();
            p.signatureId = signatureId;
            return true;
        }
    }
    return false;
}

} // namespace vectorpdf::workflow
""")

print("Workflow Part 1 generated.")