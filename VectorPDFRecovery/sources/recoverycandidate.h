// MIT License
// Copyright (c) 2026 VectorPDF Contributors
#ifndef VECTORPDF_RECOVERYCANDIDATE_H
#define VECTORPDF_RECOVERYCANDIDATE_H

#include "recoveryglobal.h"
#include "recoverytypes.h"

namespace vectorpdf::recovery
{

struct VECTORPDF_RECOVERY_EXPORT RecoveryCandidate
{
    RecoveryMetadata metadata;
    bool isValidSnapshot = true;
    QString integrityCheckSummary;
    bool selectedForRecovery = true;
};

} // namespace vectorpdf::recovery

#endif // VECTORPDF_RECOVERYCANDIDATE_H
