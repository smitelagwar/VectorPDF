// MIT License
// Copyright (c) 2026 VectorPDF Contributors
#ifndef VECTORPDF_BACKGROUNDDEFINITION_H
#define VECTORPDF_BACKGROUNDDEFINITION_H

#include "pagedecorationdefinition.h"
#include <QImage>

namespace vectorpdf::tools::decorations
{

enum class BackgroundKind
{
    SolidColor,
    Image,
    PdfPage
};

enum class BackgroundPlacement
{
    Fit,
    Fill,
    Stretch,
    Center,
    OriginalSize,
    Tile
};

class VECTORPDF_DOCUMENTTOOLS_EXPORT BackgroundDefinition : public PageDecorationDefinition
{
public:
    BackgroundDefinition()
        : PageDecorationDefinition(DecorationType::Background)
    {
        setOpacity(1.0);
        setBehindContent(true);
    }

    BackgroundKind kind = BackgroundKind::SolidColor;
    QColor solidColor = Qt::white;

    QImage image;
    QString imageFilePath;

    QString pdfFilePath;
    int pdfSourcePageIndex = 0;

    BackgroundPlacement placement = BackgroundPlacement::Fill;
};

} // namespace vectorpdf::tools::decorations

#endif // VECTORPDF_BACKGROUNDDEFINITION_H
