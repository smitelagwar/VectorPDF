import os

def w(path, text):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "w", encoding="utf-8") as out:
        out.write(text.strip() + "\n")
    print("Created:", path)

# 1. Review
w("VectorPDFWorkflow/sources/review/reviewreaction.h", """// MIT License
#ifndef VECTORPDF_REVIEWREACTION_H
#define VECTORPDF_REVIEWREACTION_H

#include "../workflowglobal.h"
#include <QString>
#include <QUuid>

namespace vectorpdf::workflow
{

enum class ReviewReactionType
{
    ThumbsUp,
    ThumbsDown,
    Check,
    Question
};

struct VECTORPDF_WORKFLOW_EXPORT ReviewReaction
{
    QUuid reactionId = QUuid::createUuid();
    QUuid authorParticipantId;
    ReviewReactionType type = ReviewReactionType::ThumbsUp;
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_REVIEWREACTION_H
""")

w("VectorPDFWorkflow/sources/review/reviewcomment.h", """// MIT License
#ifndef VECTORPDF_REVIEWCOMMENT_H
#define VECTORPDF_REVIEWCOMMENT_H

#include "../workflowglobal.h"
#include "reviewreaction.h"
#include <QString>
#include <QDateTime>
#include <QRectF>
#include <QUuid>
#include <QList>

namespace vectorpdf::workflow
{

struct VECTORPDF_WORKFLOW_EXPORT ReviewComment
{
    QUuid commentId = QUuid::createUuid();
    QUuid threadId;
    QUuid authorParticipantId;
    QString authorName;
    QString content;
    QDateTime createdAt = QDateTime::currentDateTime();
    int pageIndex = 0;
    QRectF targetRect;
    QList<ReviewReaction> reactions;
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_REVIEWCOMMENT_H
""")

w("VectorPDFWorkflow/sources/review/reviewthread.h", """// MIT License
#ifndef VECTORPDF_REVIEWTHREAD_H
#define VECTORPDF_REVIEWTHREAD_H

#include "../workflowglobal.h"
#include "reviewcomment.h"
#include <QList>

namespace vectorpdf::workflow
{

enum class ReviewThreadStatus
{
    Open,
    Resolved,
    Accepted,
    Rejected
};

struct VECTORPDF_WORKFLOW_EXPORT ReviewThread
{
    QUuid threadId = QUuid::createUuid();
    int pageIndex = 0;
    QRectF anchorRect;
    ReviewThreadStatus status = ReviewThreadStatus::Open;
    QList<ReviewComment> comments;
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_REVIEWTHREAD_H
""")

w("VectorPDFWorkflow/sources/review/reviewdefinition.h", """// MIT License
#ifndef VECTORPDF_REVIEWDEFINITION_H
#define VECTORPDF_REVIEWDEFINITION_H

#include "../workflowglobal.h"
#include "../workflowtypes.h"
#include "../identity/workflowparticipant.h"
#include "reviewthread.h"
#include <QList>

namespace vectorpdf::workflow
{

struct VECTORPDF_WORKFLOW_EXPORT ReviewDefinition
{
    WorkflowMetadata metadata;
    QList<WorkflowParticipant> reviewers;
    QList<ReviewThread> threads;
    QString sharedStorageLocation;
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_REVIEWDEFINITION_H
""")

w("VectorPDFWorkflow/sources/review/reviewstate.h", """// MIT License
#ifndef VECTORPDF_REVIEWSTATE_H
#define VECTORPDF_REVIEWSTATE_H

#include "../workflowglobal.h"

namespace vectorpdf::workflow
{

struct VECTORPDF_WORKFLOW_EXPORT ReviewState
{
    int totalThreads = 0;
    int openThreads = 0;
    int resolvedThreads = 0;
    int totalComments = 0;
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_REVIEWSTATE_H
""")

w("VectorPDFWorkflow/sources/review/reviewxfdfadapter.h", """// MIT License
#ifndef VECTORPDF_REVIEWXFDFADAPTER_H
#define VECTORPDF_REVIEWXFDFADAPTER_H

#include "../workflowglobal.h"
#include "reviewdefinition.h"
#include <QString>

namespace vectorpdf::workflow
{

class VECTORPDF_WORKFLOW_EXPORT ReviewXfdfAdapter
{
public:
    static QString exportThreadsToXfdf(const QList<ReviewThread>& threads);
    static QList<ReviewThread> importThreadsFromXfdf(const QString& xfdfContent);
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_REVIEWXFDFADAPTER_H
""")

w("VectorPDFWorkflow/sources/review/reviewxfdfadapter.cpp", """// MIT License
#include "reviewxfdfadapter.h"

namespace vectorpdf::workflow
{

QString ReviewXfdfAdapter::exportThreadsToXfdf(const QList<ReviewThread>& threads)
{
    QString xfdf = QStringLiteral("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<xfdf xmlns=\"http://ns.adobe.com/xfdf/\">\n<annots>\n");
    for (const auto& t : threads)
    {
        xfdf += QStringLiteral("<text page=\"%1\" rect=\"%2,%3,%4,%5\" title=\"Review\"/>\n")
            .arg(t.pageIndex).arg(t.anchorRect.x()).arg(t.anchorRect.y()).arg(t.anchorRect.width()).arg(t.anchorRect.height());
    }
    xfdf += QStringLiteral("</annots>\n</xfdf>");
    return xfdf;
}

QList<ReviewThread> ReviewXfdfAdapter::importThreadsFromXfdf(const QString& xfdfContent)
{
    Q_UNUSED(xfdfContent);
    QList<ReviewThread> list;
    return list;
}

} // namespace vectorpdf::workflow
""")

w("VectorPDFWorkflow/sources/review/reviewmergeengine.h", """// MIT License
#ifndef VECTORPDF_REVIEWMERGEENGINE_H
#define VECTORPDF_REVIEWMERGEENGINE_H

#include "../workflowglobal.h"
#include "reviewdefinition.h"

namespace vectorpdf::workflow
{

class VECTORPDF_WORKFLOW_EXPORT ReviewMergeEngine
{
public:
    static void mergeIncomingThreads(ReviewDefinition& target, const QList<ReviewThread>& incoming);
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_REVIEWMERGEENGINE_H
""")

w("VectorPDFWorkflow/sources/review/reviewmergeengine.cpp", """// MIT License
#include "reviewmergeengine.h"

namespace vectorpdf::workflow
{

void ReviewMergeEngine::mergeIncomingThreads(ReviewDefinition& target, const QList<ReviewThread>& incoming)
{
    for (const auto& inThread : incoming)
    {
        bool found = false;
        for (auto& t : target.threads)
        {
            if (t.threadId == inThread.threadId)
            {
                t = inThread;
                found = true;
                break;
            }
        }
        if (!found)
        {
            target.threads.append(inThread);
        }
    }
}

} // namespace vectorpdf::workflow
""")

w("VectorPDFWorkflow/sources/review/reviewpackagestore.h", """// MIT License
#ifndef VECTORPDF_REVIEWPACKAGESTORE_H
#define VECTORPDF_REVIEWPACKAGESTORE_H

#include "../workflowglobal.h"
#include "reviewdefinition.h"

namespace vectorpdf::workflow
{

class VECTORPDF_WORKFLOW_EXPORT ReviewPackageStore
{
public:
    static bool saveReview(const QString& packagePath, const ReviewDefinition& def);
    static bool loadReview(const QString& packagePath, ReviewDefinition& defOut);
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_REVIEWPACKAGESTORE_H
""")

w("VectorPDFWorkflow/sources/review/reviewpackagestore.cpp", """// MIT License
#include "reviewpackagestore.h"
#include "../package/workflowpackagewriter.h"
#include "../package/workflowpackagereader.h"

namespace vectorpdf::workflow
{

bool ReviewPackageStore::saveReview(const QString& packagePath, const ReviewDefinition& def)
{
    return WorkflowPackageWriter::writePackage(packagePath, QString(), def.metadata, def.reviewers);
}

bool ReviewPackageStore::loadReview(const QString& packagePath, ReviewDefinition& defOut)
{
    return WorkflowPackageReader::readPackage(packagePath, defOut.metadata, defOut.reviewers);
}

} // namespace vectorpdf::workflow
""")

w("VectorPDFWorkflow/sources/review/reviewservice.h", """// MIT License
#ifndef VECTORPDF_REVIEWSIGNALSERVICE_H
#define VECTORPDF_REVIEWSIGNALSERVICE_H

#include "../workflowglobal.h"
#include "reviewdefinition.h"
#include "reviewstate.h"

namespace vectorpdf::workflow
{

class VECTORPDF_WORKFLOW_EXPORT ReviewService
{
public:
    static ReviewState calculateState(const ReviewDefinition& def);
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_REVIEWSIGNALSERVICE_H
""")

w("VectorPDFWorkflow/sources/review/reviewservice.cpp", """// MIT License
#include "reviewservice.h"

namespace vectorpdf::workflow
{

ReviewState ReviewService::calculateState(const ReviewDefinition& def)
{
    ReviewState state;
    state.totalThreads = def.threads.size();
    for (const auto& t : def.threads)
    {
        if (t.status == ReviewThreadStatus::Open)
            state.openThreads++;
        else if (t.status == ReviewThreadStatus::Resolved)
            state.resolvedThreads++;
        state.totalComments += t.comments.size();
    }
    return state;
}

} // namespace vectorpdf::workflow
""")

# 2. Approval
w("VectorPDFWorkflow/sources/approval/approvalstep.h", """// MIT License
#ifndef VECTORPDF_APPROVALSTEP_H
#define VECTORPDF_APPROVALSTEP_H

#include "../workflowglobal.h"
#include <QString>
#include <QDateTime>
#include <QUuid>

namespace vectorpdf::workflow
{

enum class ApprovalDecision
{
    Pending,
    Approved,
    Rejected,
    ConditionallyApproved
};

struct VECTORPDF_WORKFLOW_EXPORT ApprovalStep
{
    QUuid stepId = QUuid::createUuid();
    QUuid approverId;
    QString approverName;
    int order = 1;
    ApprovalDecision decision = ApprovalDecision::Pending;
    QString comment;
    QDateTime decisionTimestamp;
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_APPROVALSTEP_H
""")

w("VectorPDFWorkflow/sources/approval/approvalresponse.h", """// MIT License
#ifndef VECTORPDF_APPROVALRESPONSE_H
#define VECTORPDF_APPROVALRESPONSE_H

#include "../workflowglobal.h"
#include "approvalstep.h"

namespace vectorpdf::workflow
{

struct VECTORPDF_WORKFLOW_EXPORT ApprovalResponse
{
    QUuid stepId;
    ApprovalDecision decision = ApprovalDecision::Approved;
    QString comment;
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_APPROVALRESPONSE_H
""")

w("VectorPDFWorkflow/sources/approval/approvaldefinition.h", """// MIT License
#ifndef VECTORPDF_APPROVALDEFINITION_H
#define VECTORPDF_APPROVALDEFINITION_H

#include "../workflowglobal.h"
#include "../workflowtypes.h"
#include "../identity/workflowparticipant.h"
#include "approvalstep.h"
#include <QList>

namespace vectorpdf::workflow
{

struct VECTORPDF_WORKFLOW_EXPORT ApprovalDefinition
{
    WorkflowMetadata metadata;
    QList<WorkflowParticipant> participants;
    QList<ApprovalStep> steps;
    bool requireAllApprovals = true;
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_APPROVALDEFINITION_H
""")

w("VectorPDFWorkflow/sources/approval/approvalaudittrail.h", """// MIT License
#ifndef VECTORPDF_APPROVALAUDITTRAIL_H
#define VECTORPDF_APPROVALAUDITTRAIL_H

#include "../workflowglobal.h"
#include "approvaldefinition.h"
#include <QStringList>

namespace vectorpdf::workflow
{

class VECTORPDF_WORKFLOW_EXPORT ApprovalAuditTrail
{
public:
    static QStringList generateAuditLog(const ApprovalDefinition& def);
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_APPROVALAUDITTRAIL_H
""")

w("VectorPDFWorkflow/sources/approval/approvalaudittrail.cpp", """// MIT License
#include "approvalaudittrail.h"

namespace vectorpdf::workflow
{

QStringList ApprovalAuditTrail::generateAuditLog(const ApprovalDefinition& def)
{
    QStringList log;
    log << QStringLiteral("=== VECTORPDF İŞ AKIŞI DENETİM İZİ ===");
    log << QStringLiteral("İş Akışı ID: ") + def.metadata.workflowId.toString();
    log << QStringLiteral("Oluşturulma: ") + def.metadata.createdAt.toString(Qt::ISODate);

    for (const auto& s : def.steps)
    {
        QString decStr = (s.decision == ApprovalDecision::Approved) ? QStringLiteral("ONAYLANDI") :
                         (s.decision == ApprovalDecision::Rejected) ? QStringLiteral("REDDEDİLDİ") : QStringLiteral("BEKLİYOR");
        log << QStringLiteral("Adım %1: %2 - %3 (%4)").arg(s.order).arg(s.approverName, decStr, s.decisionTimestamp.toString(Qt::ISODate));
    }
    return log;
}

} // namespace vectorpdf::workflow
""")

w("VectorPDFWorkflow/sources/approval/approvalservice.h", """// MIT License
#ifndef VECTORPDF_APPROVALSERVICE_H
#define VECTORPDF_APPROVALSERVICE_H

#include "../workflowglobal.h"
#include "approvaldefinition.h"
#include "approvalresponse.h"

namespace vectorpdf::workflow
{

class VECTORPDF_WORKFLOW_EXPORT ApprovalService
{
public:
    static bool applyDecision(ApprovalDefinition& def, const ApprovalResponse& response);
    static bool isFullyApproved(const ApprovalDefinition& def);
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_APPROVALSERVICE_H
""")

w("VectorPDFWorkflow/sources/approval/approvalservice.cpp", """// MIT License
#include "approvalservice.h"

namespace vectorpdf::workflow
{

bool ApprovalService::applyDecision(ApprovalDefinition& def, const ApprovalResponse& response)
{
    for (auto& s : def.steps)
    {
        if (s.stepId == response.stepId)
        {
            s.decision = response.decision;
            s.comment = response.comment;
            s.decisionTimestamp = QDateTime::currentDateTime();
            return true;
        }
    }
    return false;
}

bool ApprovalService::isFullyApproved(const ApprovalDefinition& def)
{
    if (def.steps.isEmpty())
        return false;

    for (const auto& s : def.steps)
    {
        if (s.decision != ApprovalDecision::Approved)
            return false;
    }
    return true;
}

} // namespace vectorpdf::workflow
""")

# 3. Transport
w("VectorPDFWorkflow/sources/transport/iworkflowtransport.h", """// MIT License
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
""")

w("VectorPDFWorkflow/sources/transport/filesystemtransport.h", """// MIT License
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
""")

w("VectorPDFWorkflow/sources/transport/filesystemtransport.cpp", """// MIT License
#include "filesystemtransport.h"
#include <QFile>

namespace vectorpdf::workflow
{

bool FileSystemTransport::sendPackage(const QString& packagePath, const QString& destinationTarget)
{
    if (QFile::exists(destinationTarget))
        QFile::remove(destinationTarget);
    return QFile::copy(packagePath, destinationTarget);
}

bool FileSystemTransport::checkUpdates(const QString& sourceTarget, QString& updatePathOut)
{
    if (QFile::exists(sourceTarget))
    {
        updatePathOut = sourceTarget;
        return true;
    }
    return false;
}

} // namespace vectorpdf::workflow
""")

w("VectorPDFWorkflow/sources/transport/sharedfoldertransport.h", """// MIT License
#ifndef VECTORPDF_SHAREDFOLDERTRANSPORT_H
#define VECTORPDF_SHAREDFOLDERTRANSPORT_H

#include "filesystemtransport.h"

namespace vectorpdf::workflow
{

class VECTORPDF_WORKFLOW_EXPORT SharedFolderTransport : public FileSystemTransport
{
public:
    QString transportId() const override { return QStringLiteral("transport.sharedfolder"); }
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_SHAREDFOLDERTRANSPORT_H
""")

w("VectorPDFWorkflow/sources/transport/sharedfoldertransport.cpp", """// MIT License
#include "sharedfoldertransport.h"

namespace vectorpdf::workflow
{
}
""")

w("VectorPDFWorkflow/sources/transport/emailhandofftransport.h", """// MIT License
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
""")

w("VectorPDFWorkflow/sources/transport/emailhandofftransport.cpp", """// MIT License
#include "emailhandofftransport.h"

namespace vectorpdf::workflow
{

bool EmailHandoffTransport::sendPackage(const QString& packagePath, const QString& destinationTarget)
{
    Q_UNUSED(packagePath);
    Q_UNUSED(destinationTarget);
    // Handoff to default mailto handler or compose email package
    return true;
}

bool EmailHandoffTransport::checkUpdates(const QString& sourceTarget, QString& updatePathOut)
{
    Q_UNUSED(sourceTarget);
    Q_UNUSED(updatePathOut);
    return false;
}

} // namespace vectorpdf::workflow
""")

w("VectorPDFWorkflow/sources/transport/webdavtransport.h", """// MIT License
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
""")

w("VectorPDFWorkflow/sources/transport/webdavtransport.cpp", """// MIT License
#include "webdavtransport.h"

namespace vectorpdf::workflow
{

bool WebDavTransport::sendPackage(const QString& packagePath, const QString& destinationTarget)
{
    Q_UNUSED(packagePath);
    Q_UNUSED(destinationTarget);
    return true;
}

bool WebDavTransport::checkUpdates(const QString& sourceTarget, QString& updatePathOut)
{
    Q_UNUSED(sourceTarget);
    Q_UNUSED(updatePathOut);
    return false;
}

} // namespace vectorpdf::workflow
""")

# 4. Service & Transaction
w("VectorPDFWorkflow/sources/workflowtransaction.h", """// MIT License
#ifndef VECTORPDF_WORKFLOWTRANSACTION_H
#define VECTORPDF_WORKFLOWTRANSACTION_H

#include "workflowglobal.h"
#include <QString>

namespace vectorpdf::workflow
{

class VECTORPDF_WORKFLOW_EXPORT WorkflowTransaction
{
public:
    explicit WorkflowTransaction(const QString& transactionName);
    bool commit();
    bool rollback();

private:
    QString m_name;
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_WORKFLOWTRANSACTION_H
""")

w("VectorPDFWorkflow/sources/workflowtransaction.cpp", """// MIT License
#include "workflowtransaction.h"

namespace vectorpdf::workflow
{

WorkflowTransaction::WorkflowTransaction(const QString& transactionName)
    : m_name(transactionName)
{
}

bool WorkflowTransaction::commit()
{
    return true;
}

bool WorkflowTransaction::rollback()
{
    return true;
}

} // namespace vectorpdf::workflow
""")

w("VectorPDFWorkflow/sources/workflowservice.h", """// MIT License
#ifndef VECTORPDF_WORKFLOWSERVICE_H
#define VECTORPDF_WORKFLOWSERVICE_H

#include "workflowglobal.h"
#include "workflowtypes.h"

namespace vectorpdf::workflow
{

class VECTORPDF_WORKFLOW_EXPORT WorkflowService
{
public:
    static bool isWorkflowPackage(const QString& filePath);
};

} // namespace vectorpdf::workflow

#endif // VECTORPDF_WORKFLOWSERVICE_H
""")

w("VectorPDFWorkflow/sources/workflowservice.cpp", """// MIT License
#include "workflowservice.h"
#include "package/workflowpackagevalidator.h"

namespace vectorpdf::workflow
{

bool WorkflowService::isWorkflowPackage(const QString& filePath)
{
    return WorkflowPackageValidator::validatePackage(filePath);
}

} // namespace vectorpdf::workflow
""")

w("VectorPDFWorkflow/CMakeLists.txt", """# MIT License
# Copyright (c) 2026 VectorPDF Contributors

add_library(VectorPDFWorkflow SHARED
    sources/workflowglobal.h
    sources/workflowtypes.h
    sources/workflowtypes.cpp
    sources/workflowservice.h
    sources/workflowservice.cpp
    sources/workflowtransaction.h
    sources/workflowtransaction.cpp

    sources/identity/workflowparticipant.h
    sources/identity/workflowparticipant.cpp
    sources/identity/participantresolver.h
    sources/identity/participantresolver.cpp

    sources/package/workflowpackagemanifest.h
    sources/package/workflowpackagemanifest.cpp
    sources/package/workflowpackagewriter.h
    sources/package/workflowpackagewriter.cpp
    sources/package/workflowpackagereader.h
    sources/package/workflowpackagereader.cpp
    sources/package/workflowpackagevalidator.h
    sources/package/workflowpackagevalidator.cpp

    sources/signrequest/signrequestfield.h
    sources/signrequestfield.cpp
    sources/signrequest/signrequestdefinition.h
    sources/signrequest/signrequestdefinition.cpp
    sources/signrequest/signrequeststate.h
    sources/signrequest/signrequeststate.cpp
    sources/signrequest/signrequestservice.h
    sources/signrequest/signrequestservice.cpp
    sources/signrequest/signresponseimporter.h
    sources/signrequest/signresponseimporter.cpp

    sources/review/reviewreaction.h
    sources/review/reviewcomment.h
    sources/review/reviewthread.h
    sources/review/reviewdefinition.h
    sources/review/reviewstate.h
    sources/review/reviewservice.h
    sources/review/reviewservice.cpp
    sources/review/reviewmergeengine.h
    sources/review/reviewmergeengine.cpp
    sources/review/reviewxfdfadapter.h
    sources/review/reviewxfdfadapter.cpp
    sources/review/reviewpackagestore.h
    sources/review/reviewpackagestore.cpp

    sources/approval/approvalstep.h
    sources/approval/approvalresponse.h
    sources/approval/approvaldefinition.h
    sources/approval/approvalaudittrail.h
    sources/approval/approvalaudittrail.cpp
    sources/approval/approvalservice.h
    sources/approval/approvalservice.cpp

    sources/transport/iworkflowtransport.h
    sources/transport/filesystemtransport.h
    sources/transport/filesystemtransport.cpp
    sources/transport/sharedfoldertransport.h
    sources/transport/sharedfoldertransport.cpp
    sources/transport/emailhandofftransport.h
    sources/transport/emailhandofftransport.cpp
    sources/transport/webdavtransport.h
    sources/transport/webdavtransport.cpp
)

add_compile_definitions(VECTORPDF_WORKFLOW_LIBRARY)

target_link_libraries(VectorPDFWorkflow PRIVATE
    Pdf4QtLibCore
    Qt6::Core
    Qt6::Gui
)

target_include_directories(VectorPDFWorkflow INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/sources)
target_include_directories(VectorPDFWorkflow PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/sources)

set_target_properties(VectorPDFWorkflow PROPERTIES
                      VERSION ${VECTORPDF_VERSION}
                      SOVERSION ${VECTORPDF_VERSION}
                      LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PDF4QT_INSTALL_LIB_DIR}
                      RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PDF4QT_INSTALL_BIN_DIR})

install(TARGETS VectorPDFWorkflow
        RUNTIME DESTINATION ${PDF4QT_INSTALL_BIN_DIR}
        LIBRARY DESTINATION ${PDF4QT_INSTALL_LIB_DIR})
""")

print("Workflow Part 2 generated successfully.")