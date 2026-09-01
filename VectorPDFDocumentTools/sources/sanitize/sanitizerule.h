// MIT License
#ifndef VECTORPDF_SANITIZERULE_H
#define VECTORPDF_SANITIZERULE_H

#include "../documenttoolsglobal.h"

namespace vectorpdf::tools::sanitize
{

struct VECTORPDF_DOCUMENTTOOLS_EXPORT SanitizePolicy
{
    bool removeMetadata = true;
    bool removeAttachments = true;
    bool removeBookmarks = false;
    bool removeHiddenLayers = true;
    bool removeActions = true;
    bool removeJavaScript = true;
    bool removeDeletedObjectResiduals = true;
    bool removeThumbnails = false;
    bool flattenAnnotations = false;
};

} // namespace vectorpdf::tools::sanitize

#endif // VECTORPDF_SANITIZERULE_H
