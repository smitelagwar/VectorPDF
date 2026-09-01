// MIT License
#include "sanitizedocumentservice.h"
#include "metadataremover.h"
#include "attachmentremover.h"
#include "javascriptremover.h"
#include "actionremover.h"
#include "../documenttoolvalidator.h"
#include "pdfdocument.h"

namespace vectorpdf::tools::sanitize
{

SanitizeReport SanitizeDocumentService::sanitizeDocument(pdf::PDFDocument* document, const SanitizePolicy& policy)
{
    SanitizeReport rep;
    DocumentToolResult val = DocumentToolValidator::validateMutationAllowed(document);
    if (!val.isSuccessful())
    {
        rep.result = val;
        return rep;
    }

    if (policy.removeMetadata)
    {
        rep.removedMetadataEntries = MetadataRemover::removeAllMetadata(document);
        rep.cleanedItems << QStringLiteral("Document Metadata cleared");
    }

    if (policy.removeAttachments)
    {
        rep.removedAttachmentsCount = AttachmentRemover::removeAllAttachments(document);
        rep.cleanedItems << QStringLiteral("Embedded attachments removed");
    }

    if (policy.removeJavaScript)
    {
        rep.removedJavaScriptCount = JavaScriptRemover::removeAllJavaScript(document);
        rep.cleanedItems << QStringLiteral("Document JavaScript actions removed");
    }

    if (policy.removeActions)
    {
        rep.removedActionsCount = ActionRemover::removeExternalActions(document);
        rep.cleanedItems << QStringLiteral("External Launch/URI actions sanitized");
    }

    rep.result = DocumentToolResult::success(QStringLiteral("Document sanitized before sharing."));
    return rep;
}

} // namespace vectorpdf::tools::sanitize
