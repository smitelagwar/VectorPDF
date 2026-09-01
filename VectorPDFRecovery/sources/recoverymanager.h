// MIT License
#ifndef VECTORPDF_RECOVERYMANAGER_H
#define VECTORPDF_RECOVERYMANAGER_H

#include "recoveryglobal.h"
#include "recoverysession.h"
#include "recoverycandidate.h"
#include <QObject>
#include <QMap>
#include <memory>

namespace vectorpdf::recovery
{

class VECTORPDF_RECOVERY_EXPORT RecoveryManager : public QObject
{
    Q_OBJECT

public:
    static RecoveryManager& instance();

    std::shared_ptr<RecoverySession> startSession(const QString& filePath, pdf::PDFDocument* document);
    void endSession(const QUuid& sessionId);
    QList<RecoveryCandidate> checkRecoverableSessionsOnLaunch();

    bool restoreCandidate(const RecoveryCandidate& candidate, const QString& restoreToPath);
    bool discardCandidate(const RecoveryCandidate& candidate);

private:
    RecoveryManager(QObject* parent = nullptr);
    QMap<QUuid, std::shared_ptr<RecoverySession>> m_activeSessions;
};

} // namespace vectorpdf::recovery

#endif // VECTORPDF_RECOVERYMANAGER_H
