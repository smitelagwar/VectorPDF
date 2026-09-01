// MIT License
// Copyright (c) 2026 VectorPDF Contributors
#ifndef VECTORPDF_WATERMARKDEFINITION_H
#define VECTORPDF_WATERMARKDEFINITION_H

#include "pagedecorationdefinition.h"
#include <QImage>
#include <QPointF>

namespace vectorpdf::tools::decorations
{

enum class WatermarkKind
{
    Text,
    Image,
    PdfPage
};

class VECTORPDF_DOCUMENTTOOLS_EXPORT WatermarkDefinition : public PageDecorationDefinition
{
public:
    WatermarkDefinition()
        : PageDecorationDefinition(DecorationType::Watermark)
    {
        setOpacity(0.3);
        setBehindContent(false);
    }

    WatermarkKind kind = WatermarkKind::Text;
    QString text = QStringLiteral("CONFIDENTIAL");
    QFont font = QFont("Segoe UI", 48, QFont::Bold);
    QColor textColor = QColor(180, 0, 0);

    QImage image;
    QString imageFilePath;

    QString pdfFilePath;
    int pdfSourcePageIndex = 0;

    qreal rotationAngle = -45.0;
    qreal scale = 1.0;
    QPointF offset = QPointF(0.0, 0.0);
    bool tile = false;
};

} // namespace vectorpdf::tools::decorations

#endif // VECTORPDF_WATERMARKDEFINITION_H
