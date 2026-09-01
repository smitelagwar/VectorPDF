// MIT License
#ifndef VECTORPDF_SANITIZERESULT_H
#define VECTORPDF_SANITIZERESULT_H

#include "../documenttoolsglobal.h"
#include "../documenttoolresult.h"
#include <QStringList>

namespace vectorpdf::tools::sanitize
{

struct VECTORPDF_DOCUMENTTOOLS_EXPORT SanitizeReport
{
    DocumentToolResult result;
    int removedMetadataEntries = 0;
    int removedAttachmentsCount = 0;
    int removedActionsCount = 0;
    int removedJavaScriptCount = 0;
    int removedHiddenLayersCount = 0;
    QStringList cleanedItems;
};

} // namespace vectorpdf::tools::sanitize

#endif // VECTORPDF_SANITIZERESULT_H
