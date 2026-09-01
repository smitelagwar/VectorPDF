// MIT License
// Copyright (c) 2026 VectorPDF Contributors
#ifndef VECTORPDF_PAGEDECORATIONMANAGER_H
#define VECTORPDF_PAGEDECORATIONMANAGER_H

#include "../documenttoolsglobal.h"
#include "../documenttoolresult.h"
#include "pagedecorationdefinition.h"
#include "headerfooterdefinition.h"
#include "watermarkdefinition.h"
#include "backgrounddefinition.h"
#include <memory>
#include <QList>

namespace pdf
{
class PDFDocument;
}

namespace vectorpdf::tools::decorations
{

class VECTORPDF_DOCUMENTTOOLS_EXPORT PageDecorationManager
{
public:
    PageDecorationManager() = default;

    DocumentToolResult applyHeaderFooter(pdf::PDFDocument* document, const HeaderFooterDefinition& definition);
    DocumentToolResult applyWatermark(pdf::PDFDocument* document, const WatermarkDefinition& definition);
    DocumentToolResult applyBackground(pdf::PDFDocument* document, const BackgroundDefinition& definition);

    DocumentToolResult removeDecorations(pdf::PDFDocument* document, DecorationType type);
    DocumentToolResult removeDecorationById(pdf::PDFDocument* document, const QUuid& id);
};

} // namespace vectorpdf::tools::decorations

#endif // VECTORPDF_PAGEDECORATIONMANAGER_H
