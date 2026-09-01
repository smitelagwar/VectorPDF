// MIT License
#ifndef VECTORPDF_RECOVERYCLEANUPSERVICE_H
#define VECTORPDF_RECOVERYCLEANUPSERVICE_H

#include "recoveryglobal.h"

namespace vectorpdf::recovery
{

class VECTORPDF_RECOVERY_EXPORT RecoveryCleanupService
{
public:
    static void cleanupTerminatedSession(const QString& sessionId);
    static void markCleanClose(const QString& sessionId);
};

} // namespace vectorpdf::recovery

#endif // VECTORPDF_RECOVERYCLEANUPSERVICE_H
