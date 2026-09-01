// MIT License
#ifndef VECTORPDF_RECOVERYVALIDATOR_H
#define VECTORPDF_RECOVERYVALIDATOR_H

#include "recoveryglobal.h"
#include "recoverytypes.h"

namespace vectorpdf::recovery
{

class VECTORPDF_RECOVERY_EXPORT RecoveryValidator
{
public:
    static bool validateSnapshotIntegrity(const QString& snapshotFilePath);
};

} // namespace vectorpdf::recovery

#endif // VECTORPDF_RECOVERYVALIDATOR_H
