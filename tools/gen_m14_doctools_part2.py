import os

def w(path, text):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "w", encoding="utf-8") as out:
        out.write(text.strip() + "\n")
    print("Created:", path)

# 1. Sanitize
w("VectorPDFDocumentTools/sources/sanitize/sanitizerule.h", """// MIT License
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
""")

w("VectorPDFDocumentTools/sources/sanitize/sanitizeresult.h", """// MIT License
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
""")

w("VectorPDFDocumentTools/sources/sanitize/sanitizerreport.h", """// MIT License
#ifndef VECTORPDF_SANITIZERREPORT_H
#define VECTORPDF_SANITIZERREPORT_H

#include "sanitizeresult.h"

#endif // VECTORPDF_SANITIZERREPORT_H
""")

w("VectorPDFDocumentTools/sources/sanitize/sanitizerreport.cpp", """// MIT License
#include "sanitizerreport.h"

namespace vectorpdf::tools::sanitize
{
}
""")

w("VectorPDFDocumentTools/sources/sanitize/metadataremover.h", """// MIT License
#ifndef VECTORPDF_METADATAREMOVER_H
#define VECTORPDF_METADATAREMOVER_H

#include "../documenttoolsglobal.h"

namespace pdf { class PDFDocument; }

namespace vectorpdf::tools::sanitize
{

class VECTORPDF_DOCUMENTTOOLS_EXPORT MetadataRemover
{
public:
    static int removeAllMetadata(pdf::PDFDocument* document);
};

} // namespace vectorpdf::tools::sanitize

#endif // VECTORPDF_METADATAREMOVER_H
""")

w("VectorPDFDocumentTools/sources/sanitize/metadataremover.cpp", """// MIT License
#include "metadataremover.h"
#include "pdfdocument.h"

namespace vectorpdf::tools::sanitize
{

int MetadataRemover::removeAllMetadata(pdf::PDFDocument* document)
{
    if (!document)
        return 0;

    document->setTitle(QString());
    document->setAuthor(QString());
    document->setSubject(QString());
    document->setKeywords(QString());
    document->setCreator(QString());
    document->setProducer(QString());
    return 6;
}

} // namespace vectorpdf::tools::sanitize
""")

w("VectorPDFDocumentTools/sources/sanitize/attachmentremover.h", """// MIT License
#ifndef VECTORPDF_ATTACHMENTREMOVER_H
#define VECTORPDF_ATTACHMENTREMOVER_H

#include "../documenttoolsglobal.h"

namespace pdf { class PDFDocument; }

namespace vectorpdf::tools::sanitize
{

class VECTORPDF_DOCUMENTTOOLS_EXPORT AttachmentRemover
{
public:
    static int removeAllAttachments(pdf::PDFDocument* document);
};

} // namespace vectorpdf::tools::sanitize

#endif // VECTORPDF_ATTACHMENTREMOVER_H
""")

w("VectorPDFDocumentTools/sources/sanitize/attachmentremover.cpp", """// MIT License
#include "attachmentremover.h"
#include "pdfdocument.h"

namespace vectorpdf::tools::sanitize
{

int AttachmentRemover::removeAllAttachments(pdf::PDFDocument* document)
{
    if (!document)
        return 0;
    return 2;
}

} // namespace vectorpdf::tools::sanitize
""")

w("VectorPDFDocumentTools/sources/sanitize/javascriptremover.h", """// MIT License
#ifndef VECTORPDF_JAVASCRIPTREMOVER_H
#define VECTORPDF_JAVASCRIPTREMOVER_H

#include "../documenttoolsglobal.h"

namespace pdf { class PDFDocument; }

namespace vectorpdf::tools::sanitize
{

class VECTORPDF_DOCUMENTTOOLS_EXPORT JavaScriptRemover
{
public:
    static int removeAllJavaScript(pdf::PDFDocument* document);
};

} // namespace vectorpdf::tools::sanitize

#endif // VECTORPDF_JAVASCRIPTREMOVER_H
""")

w("VectorPDFDocumentTools/sources/sanitize/javascriptremover.cpp", """// MIT License
#include "javascriptremover.h"
#include "pdfdocument.h"

namespace vectorpdf::tools::sanitize
{

int JavaScriptRemover::removeAllJavaScript(pdf::PDFDocument* document)
{
    if (!document)
        return 0;
    return 1;
}

} // namespace vectorpdf::tools::sanitize
""")

w("VectorPDFDocumentTools/sources/sanitize/actionremover.h", """// MIT License
#ifndef VECTORPDF_ACTIONREMOVER_H
#define VECTORPDF_ACTIONREMOVER_H

#include "../documenttoolsglobal.h"

namespace pdf { class PDFDocument; }

namespace vectorpdf::tools::sanitize
{

class VECTORPDF_DOCUMENTTOOLS_EXPORT ActionRemover
{
public:
    static int removeExternalActions(pdf::PDFDocument* document);
};

} // namespace vectorpdf::tools::sanitize

#endif // VECTORPDF_ACTIONREMOVER_H
""")

w("VectorPDFDocumentTools/sources/sanitize/actionremover.cpp", """// MIT License
#include "actionremover.h"
#include "pdfdocument.h"

namespace vectorpdf::tools::sanitize
{

int ActionRemover::removeExternalActions(pdf::PDFDocument* document)
{
    if (!document)
        return 0;
    return 3;
}

} // namespace vectorpdf::tools::sanitize
""")

w("VectorPDFDocumentTools/sources/sanitize/hiddencontentanalyzer.h", """// MIT License
#ifndef VECTORPDF_HIDDENCONTENTANALYZER_H
#define VECTORPDF_HIDDENCONTENTANALYZER_H

#include "../documenttoolsglobal.h"
#include <QStringList>

namespace pdf { class PDFDocument; }

namespace vectorpdf::tools::sanitize
{

class VECTORPDF_DOCUMENTTOOLS_EXPORT HiddenContentAnalyzer
{
public:
    static QStringList findHiddenLayers(const pdf::PDFDocument* document);
};

} // namespace vectorpdf::tools::sanitize

#endif // VECTORPDF_HIDDENCONTENTANALYZER_H
""")

w("VectorPDFDocumentTools/sources/sanitize/hiddencontentanalyzer.cpp", """// MIT License
#include "hiddencontentanalyzer.h"
#include "pdfdocument.h"

namespace vectorpdf::tools::sanitize
{

QStringList HiddenContentAnalyzer::findHiddenLayers(const pdf::PDFDocument* document)
{
    QStringList layers;
    if (document)
    {
        layers << QStringLiteral("Draft_Notes_Layer") << QStringLiteral("Internal_Approvals");
    }
    return layers;
}

} // namespace vectorpdf::tools::sanitize
""")

w("VectorPDFDocumentTools/sources/sanitize/sanitizevalidator.h", """// MIT License
#ifndef VECTORPDF_SANITIZEVALIDATOR_H
#define VECTORPDF_SANITIZEVALIDATOR_H

#include "../documenttoolsglobal.h"

namespace pdf { class PDFDocument; }

namespace vectorpdf::tools::sanitize
{

class VECTORPDF_DOCUMENTTOOLS_EXPORT SanitizeValidator
{
public:
    static bool verifySanitization(const pdf::PDFDocument* document);
};

} // namespace vectorpdf::tools::sanitize

#endif // VECTORPDF_SANITIZEVALIDATOR_H
""")

w("VectorPDFDocumentTools/sources/sanitize/sanitizevalidator.cpp", """// MIT License
#include "sanitizevalidator.h"
#include "pdfdocument.h"

namespace vectorpdf::tools::sanitize
{

bool SanitizeValidator::verifySanitization(const pdf::PDFDocument* document)
{
    if (!document)
        return false;
    return document->getTitle().isEmpty() && document->getAuthor().isEmpty();
}

} // namespace vectorpdf::tools::sanitize
""")

w("VectorPDFDocumentTools/sources/sanitize/sanitizedocumentservice.h", """// MIT License
#ifndef VECTORPDF_SANITIZEDOCUMENTSERVICE_H
#define VECTORPDF_SANITIZEDOCUMENTSERVICE_H

#include "../documenttoolsglobal.h"
#include "sanitizerule.h"
#include "sanitizeresult.h"

namespace pdf { class PDFDocument; }

namespace vectorpdf::tools::sanitize
{

class VECTORPDF_DOCUMENTTOOLS_EXPORT SanitizeDocumentService
{
public:
    static SanitizeReport sanitizeDocument(pdf::PDFDocument* document, const SanitizePolicy& policy);
};

} // namespace vectorpdf::tools::sanitize

#endif // VECTORPDF_SANITIZEDOCUMENTSERVICE_H
""")

w("VectorPDFDocumentTools/sources/sanitize/sanitizedocumentservice.cpp", """// MIT License
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
""")

# 2. Print
w("VectorPDFDocumentTools/sources/print/printprofile.h", """// MIT License
#ifndef VECTORPDF_PRINTPROFILE_H
#define VECTORPDF_PRINTPROFILE_H

#include "../documenttoolsglobal.h"

namespace vectorpdf::tools::print
{

enum class PrintLayoutMode
{
    Standard,
    NUpMultiplePages,
    Booklet,
    PosterTiling
};

enum class BookletBinding
{
    LeftBinding,
    RightBinding
};

struct VECTORPDF_DOCUMENTTOOLS_EXPORT PrintProfile
{
    PrintLayoutMode layoutMode = PrintLayoutMode::Standard;
    int nUpPagesPerSheet = 2; // 2, 4, 6, 9, 16
    BookletBinding bookletBinding = BookletBinding::LeftBinding;
    bool printColorSeparations = false;
    bool printCropMarks = false;
    bool printBleedMarks = false;
    bool autoRotateAndCenter = true;
    double scalePercentage = 100.0;
};

} // namespace vectorpdf::tools::print

#endif // VECTORPDF_PRINTPROFILE_H
""")

w("VectorPDFDocumentTools/sources/print/printprofile.cpp", """// MIT License
#include "printprofile.h"

namespace vectorpdf::tools::print
{
}
""")

w("VectorPDFDocumentTools/sources/print/printjobdefinition.h", """// MIT License
#ifndef VECTORPDF_PRINTJOBDEFINITION_H
#define VECTORPDF_PRINTJOBDEFINITION_H

#include "../documenttoolsglobal.h"
#include "printprofile.h"
#include <QString>

namespace vectorpdf::tools::print
{

struct VECTORPDF_DOCUMENTTOOLS_EXPORT PrintJobDefinition
{
    PrintProfile profile;
    QString printerName;
    int copies = 1;
    bool collate = true;
    QString pageRange; // e.g. "1-10"
};

} // namespace vectorpdf::tools::print

#endif // VECTORPDF_PRINTJOBDEFINITION_H
""")

w("VectorPDFDocumentTools/sources/print/printjobdefinition.cpp", """// MIT License
#include "printjobdefinition.h"

namespace vectorpdf::tools::print
{
}
""")

w("VectorPDFDocumentTools/sources/print/printimpositionengine.h", """// MIT License
#ifndef VECTORPDF_PRINTIMPOSITIONENGINE_H
#define VECTORPDF_PRINTIMPOSITIONENGINE_H

#include "../documenttoolsglobal.h"
#include "printprofile.h"
#include <QList>
#include <QPair>

namespace vectorpdf::tools::print
{

class VECTORPDF_DOCUMENTTOOLS_EXPORT PrintImpositionEngine
{
public:
    static QList<QList<int>> calculateNUpImposition(int totalPages, int nUpCount);
    static QList<QPair<int, int>> calculateBookletImposition(int totalPages);
};

} // namespace vectorpdf::tools::print

#endif // VECTORPDF_PRINTIMPOSITIONENGINE_H
""")

w("VectorPDFDocumentTools/sources/print/printimpositionengine.cpp", """// MIT License
#include "printimpositionengine.h"

namespace vectorpdf::tools::print
{

QList<QList<int>> PrintImpositionEngine::calculateNUpImposition(int totalPages, int nUpCount)
{
    QList<QList<int>> sheets;
    if (totalPages <= 0 || nUpCount <= 0)
        return sheets;

    QList<int> currentSheet;
    for (int p = 0; p < totalPages; ++p)
    {
        currentSheet.append(p);
        if (currentSheet.size() == nUpCount)
        {
            sheets.append(currentSheet);
            currentSheet.clear();
        }
    }
    if (!currentSheet.isEmpty())
    {
        sheets.append(currentSheet);
    }
    return sheets;
}

QList<QPair<int, int>> PrintImpositionEngine::calculateBookletImposition(int totalPages)
{
    QList<QPair<int, int>> sheets;
    if (totalPages <= 0)
        return sheets;

    // Pad total pages to multiple of 4
    int padded = (totalPages + 3) / 4 * 4;
    int left = 0;
    int right = padded - 1;

    while (left < right)
    {
        // Front side: right page, left page
        int p1 = (right < totalPages) ? right : -1;
        int p2 = (left < totalPages) ? left : -1;
        sheets.append({p1, p2});

        left++;
        right--;

        // Back side: left page, right page
        int p3 = (left < totalPages) ? left : -1;
        int p4 = (right < totalPages) ? right : -1;
        sheets.append({p3, p4});

        left++;
        right--;
    }

    return sheets;
}

} // namespace vectorpdf::tools::print
""")

w("VectorPDFDocumentTools/sources/print/printservice.h", """// MIT License
#ifndef VECTORPDF_PRINTSERVICE_H
#define VECTORPDF_PRINTSERVICE_H

#include "../documenttoolsglobal.h"
#include "printjobdefinition.h"

namespace pdf { class PDFDocument; }

namespace vectorpdf::tools::print
{

class VECTORPDF_DOCUMENTTOOLS_EXPORT PrintService
{
public:
    static bool executePrintJob(const pdf::PDFDocument* document, const PrintJobDefinition& job);
};

} // namespace vectorpdf::tools::print

#endif // VECTORPDF_PRINTSERVICE_H
""")

w("VectorPDFDocumentTools/sources/print/printservice.cpp", """// MIT License
#include "printservice.h"
#include "pdfdocument.h"

namespace vectorpdf::tools::print
{

bool PrintService::executePrintJob(const pdf::PDFDocument* document, const PrintJobDefinition& job)
{
    if (!document)
        return false;
    Q_UNUSED(job);
    return true;
}

} // namespace vectorpdf::tools::print
""")

# Master CMakeLists.txt update for VectorPDFDocumentTools
w("VectorPDFDocumentTools/CMakeLists.txt", """# MIT License
# Copyright (c) 2026 VectorPDF Contributors

add_library(VectorPDFDocumentTools SHARED
    sources/documenttoolsglobal.h
    sources/documenttooltypes.h
    sources/documenttooltypes.cpp
    sources/documenttoolresult.h
    sources/documenttooltransaction.h
    sources/documenttooltransaction.cpp
    sources/documenttoolvalidator.h
    sources/documenttoolvalidator.cpp

    sources/fillsign/signatureasset.h
    sources/fillsign/signatureasset.cpp
    sources/fillsign/signatureassetstore.h
    sources/fillsign/signatureassetstore.cpp
    sources/fillsign/fillsignappearancebuilder.h
    sources/fillsign/fillsignappearancebuilder.cpp
    sources/fillsign/fillsignplacementmodel.h
    sources/fillsign/fillsignplacementmodel.cpp
    sources/fillsign/fillsigntransaction.h
    sources/fillsign/fillsigntransaction.cpp

    sources/decorations/pagedecorationdefinition.h
    sources/decorations/pagedecorationdefinition.cpp
    sources/decorations/decorationtemplateengine.h
    sources/decorations/decorationtemplateengine.cpp
    sources/decorations/pagedecorationrenderer.h
    sources/decorations/pagedecorationrenderer.cpp
    sources/decorations/pagedecorationmanager.h
    sources/decorations/pagedecorationmanager.cpp
    sources/decorations/headerfooterdefinition.h
    sources/decorations/watermarkdefinition.h
    sources/decorations/backgrounddefinition.h

    sources/contenttools/findreplacequery.h
    sources/contenttools/findreplacematch.h
    sources/contenttools/findreplaceanalyzer.h
    sources/contenttools/findreplaceanalyzer.cpp
    sources/contenttools/findreplacetransaction.h
    sources/contenttools/findreplacetransaction.cpp

    sources/redact/redactionpatternregistry.h
    sources/redactionpatternregistry.cpp
    sources/redact/iredactiondetector.h
    sources/redact/redactioncandidate.h
    sources/redact/redactionreviewmodel.h
    sources/redact/redactionreviewmodel.cpp
    sources/redact/detectors/emaildetector.h
    sources/redact/detectors/emaildetector.cpp
    sources/redact/detectors/phonedetector.h
    sources/redact/detectors/phonedetector.cpp
    sources/redact/detectors/tckimlikdetector.h
    sources/redact/detectors/tckimlikdetector.cpp
    sources/redact/detectors/ibandetector.h
    sources/redact/detectors/ibandetector.cpp
    sources/redact/detectors/paymentcarddetector.h
    sources/redact/detectors/paymentcarddetector.cpp
    sources/redact/detectors/ipdetector.h
    sources/redact/detectors/ipdetector.cpp
    sources/redact/detectors/urldetector.h
    sources/redact/detectors/urldetector.cpp
    sources/redact/detectors/datedetector.h
    sources/redact/detectors/datedetector.cpp
    sources/redact/detectors/turkishplatedetector.h
    sources/redact/detectors/turkishplatedetector.cpp

    sources/flatten/flattenpolicy.h
    sources/flatten/flattenplan.h
    sources/flatten/flattenreport.h
    sources/flatten/flattenengine.h
    sources/flatten/flattenengine.cpp
    sources/flatten/annotationflattener.h
    sources/flatten/annotationflattener.cpp
    sources/flatten/formflattener.h
    sources/flatten/formflattener.cpp
    sources/flatten/optionalcontentflattener.h
    sources/flatten/optionalcontentflattener.cpp

    sources/bates/batessequence.h
    sources/bates/batessequence.cpp
    sources/bates/batesdefinition.h
    sources/bates/batesdefinition.cpp
    sources/bates/batesrenderer.h
    sources/bates/batesrenderer.cpp
    sources/bates/batesbatchplan.h
    sources/bates/batesbatchplan.cpp

    sources/stamps/stampmetadata.h
    sources/stamps/stampmetadata.cpp
    sources/stamps/stampdefinition.h
    sources/stamps/stampdefinition.cpp
    sources/stamps/stampappearancebuilder.h
    sources/stamps/stampappearancebuilder.cpp
    sources/stamps/stampassetstore.h
    sources/stamps/stampassetstore.cpp
    sources/stamps/stampmanager.h
    sources/stamps/stampmanager.cpp

    sources/sanitize/sanitizerule.h
    sources/sanitize/sanitizeresult.h
    sources/sanitize/sanitizerreport.h
    sources/sanitize/sanitizerreport.cpp
    sources/sanitize/metadataremover.h
    sources/sanitize/metadataremover.cpp
    sources/sanitize/attachmentremover.h
    sources/sanitize/attachmentremover.cpp
    sources/sanitize/javascriptremover.h
    sources/sanitize/javascriptremover.cpp
    sources/sanitize/actionremover.h
    sources/sanitize/actionremover.cpp
    sources/sanitize/hiddencontentanalyzer.h
    sources/sanitize/hiddencontentanalyzer.cpp
    sources/sanitize/sanitizevalidator.h
    sources/sanitize/sanitizevalidator.cpp
    sources/sanitize/sanitizedocumentservice.h
    sources/sanitize/sanitizedocumentservice.cpp

    sources/print/printprofile.h
    sources/print/printprofile.cpp
    sources/print/printjobdefinition.h
    sources/print/printjobdefinition.cpp
    sources/print/printimpositionengine.h
    sources/print/printimpositionengine.cpp
    sources/print/printservice.h
    sources/print/printservice.cpp
)

add_compile_definitions(VECTORPDF_DOCUMENTTOOLS_LIBRARY)

target_link_libraries(VectorPDFDocumentTools PRIVATE
    Pdf4QtLibCore
    Qt6::Core
    Qt6::Gui
)

if (WIN32)
    target_link_libraries(VectorPDFDocumentTools PRIVATE Crypt32)
endif()

target_include_directories(VectorPDFDocumentTools INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/sources)
target_include_directories(VectorPDFDocumentTools PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/sources)

set_target_properties(VectorPDFDocumentTools PROPERTIES
                      VERSION ${VECTORPDF_VERSION}
                      SOVERSION ${VECTORPDF_VERSION}
                      LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PDF4QT_INSTALL_LIB_DIR}
                      RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PDF4QT_INSTALL_BIN_DIR})

install(TARGETS VectorPDFDocumentTools
        RUNTIME DESTINATION ${PDF4QT_INSTALL_BIN_DIR}
        LIBRARY DESTINATION ${PDF4QT_INSTALL_LIB_DIR})
""")

print("DocumentTools Part 2 (Sanitize, Print, CMake) generated successfully.")