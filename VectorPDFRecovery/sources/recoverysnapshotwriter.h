// MIT License
#ifndef VECTORPDF_RECOVERYSNAPSHOTWRITER_H
#define VECTORPDF_RECOVERYSNAPSHOTWRITER_H

#include "recoveryglobal.h"
#include <QString>

namespace pdf { class PDFDocument; }

namespace vectorpdf::recovery
{

class VECTORPDF_RECOVERY_EXPORT RecoverySnapshotWriter
{
public:
    static bool writeSnapshot(pdf::PDFDocument* document, const QString& destinationPath);
};

} // namespace vectorpdf::recovery

#endif // VECTORPDF_RECOVERYSNAPSHOTWRITER_H
