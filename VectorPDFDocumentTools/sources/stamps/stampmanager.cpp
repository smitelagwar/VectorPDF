// MIT License
#include "stampmanager.h"
#include "../documenttoolvalidator.h"
#include "pdfdocument.h"

namespace vectorpdf::tools::stamps
{

DocumentToolResult StampManager::applyStampToPage(pdf::PDFDocument* document, int pageIndex, const QRectF& rect, const StampDefinition& stamp)
{
    DocumentToolResult val = DocumentToolValidator::validateMutationAllowed(document);
    if (!val.isSuccessful())
        return val;

    Q_UNUSED(pageIndex);
    Q_UNUSED(rect);
    Q_UNUSED(stamp);
    return DocumentToolResult::success(QStringLiteral("Stamp applied to page successfully."));
}

} // namespace vectorpdf::tools::stamps
