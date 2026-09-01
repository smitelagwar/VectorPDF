// MIT License
#include "recoverylaunchscanner.h"
#include "recoverystore.h"
#include "recoverymanifest.h"
#include "recoveryvalidator.h"

namespace vectorpdf::recovery
{

QList<RecoveryCandidate> RecoveryLaunchScanner::scanForRecoverableSessions()
{
    QList<RecoveryCandidate> candidates;
    QStringList manifests = RecoveryStore::listAllManifestFiles();

    for (const QString& mPath : manifests)
    {
        RecoveryMetadata meta;
        if (RecoveryManifest::readFromFile(mPath, meta))
        {
            if (meta.state == RecoverySessionState::Active || meta.state == RecoverySessionState::AbruptlyTerminated)
            {
                RecoveryCandidate cand;
                cand.metadata = meta;
                cand.isValidSnapshot = RecoveryValidator::validateSnapshotIntegrity(meta.tempSnapshotFilePath);
                cand.integrityCheckSummary = cand.isValidSnapshot ? 
                    QStringLiteral("Valid snapshot found") : QStringLiteral("Corrupted or incomplete snapshot");
                cand.selectedForRecovery = cand.isValidSnapshot;
                candidates.append(cand);
            }
        }
    }
    return candidates;
}

} // namespace vectorpdf::recovery
