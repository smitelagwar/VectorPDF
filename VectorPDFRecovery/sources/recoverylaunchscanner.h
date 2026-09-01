// MIT License
#ifndef VECTORPDF_RECOVERYLAUNCHSCANNER_H
#define VECTORPDF_RECOVERYLAUNCHSCANNER_H

#include "recoveryglobal.h"
#include "recoverycandidate.h"
#include <QList>

namespace vectorpdf::recovery
{

class VECTORPDF_RECOVERY_EXPORT RecoveryLaunchScanner
{
public:
    static QList<RecoveryCandidate> scanForRecoverableSessions();
};

} // namespace vectorpdf::recovery

#endif // VECTORPDF_RECOVERYLAUNCHSCANNER_H
