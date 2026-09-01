// MIT License
// Copyright (c) 2026 VectorPDF Contributors
#ifndef VECTORPDF_RECOVERYTYPES_H
#define VECTORPDF_RECOVERYTYPES_H

#include "recoveryglobal.h"
#include <QString>
#include <QDateTime>
#include <QUuid>

namespace vectorpdf::recovery
{

enum class RecoverySessionState
{
    Active,
    CleanlyClosed,
    AbruptlyTerminated,
    Restored,
    Discarded
};

enum class RecoveryPriority
{
    High,
    Medium,
    Low
};

struct VECTORPDF_RECOVERY_EXPORT RecoveryMetadata
{
    QUuid sessionId = QUuid::createUuid();
    QString originalFilePath;
    QString documentTitle;
    QDateTime sessionStartTime = QDateTime::currentDateTime();
    QDateTime lastAutosaveTime = QDateTime::currentDateTime();
    int uncommittedActionsCount = 0;
    RecoverySessionState state = RecoverySessionState::Active;
    qint64 originalFileSize = 0;
    QString tempSnapshotFilePath;
    bool isEncrypted = false;
};

} // namespace vectorpdf::recovery

#endif // VECTORPDF_RECOVERYTYPES_H
