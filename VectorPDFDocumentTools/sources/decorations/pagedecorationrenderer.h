// MIT License
// Copyright (c) 2026 VectorPDF Contributors
#ifndef VECTORPDF_PAGEDECORATIONRENDERER_H
#define VECTORPDF_PAGEDECORATIONRENDERER_H

#include "../documenttoolsglobal.h"
#include "pagedecorationdefinition.h"
#include "headerfooterdefinition.h"
#include "watermarkdefinition.h"
#include "backgrounddefinition.h"
#include "decorationtemplateengine.h"
#include <QPainter>
#include <QRectF>

namespace vectorpdf::tools::decorations
{

class VECTORPDF_DOCUMENTTOOLS_EXPORT PageDecorationRenderer
{
public:
    static void renderHeaderFooter(QPainter* painter, const HeaderFooterDefinition& def, const QRectF& pageBounds, const TemplateContext& context);
    static void renderWatermark(QPainter* painter, const WatermarkDefinition& def, const QRectF& pageBounds, const TemplateContext& context);
    static void renderBackground(QPainter* painter, const BackgroundDefinition& def, const QRectF& pageBounds);
};

} // namespace vectorpdf::tools::decorations

#endif // VECTORPDF_PAGEDECORATIONRENDERER_H
