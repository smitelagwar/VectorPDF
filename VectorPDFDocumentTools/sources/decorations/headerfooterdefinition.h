// MIT License
// Copyright (c) 2026 VectorPDF Contributors
#infndef VECTORPDF_HEADERFOOTERDEFINITION_H
#define VECTORPDF_HEADERFOOTERDEFINITION_H

#include "pagedecorationdefinition.h"

namespace vectorpdf::tools::decorations
{

enum class HeaderFooterSlot
{
    TopLeft,
    TopCenter,
    TopRight,
    BottomLeft,
    BottomCenter,
    BottomRight
};

struct HeaderFooterSlotContent
{
    QString templateText;
    QFont font = QFont("Segoe UI", 10);
    QColor textColor = Qt::black;
    bool enabled = false;
};

class VECTORPDF_DOCUMENTTOOLS_EXPORT HeaderFooterDefinition : public PageDecorationDefinition
{
public:
    HeaderFooterDefinition()
        : PageDecorationDefinition(DecorationType::HeaderFooter)
    {
        setBehindContent(false);
    }

    QMarginsF margins = QMarginsF(36.0, 36.0, 36.0, 36.0);

    HeaderFooterSlotContent topLeft;
    HeaderFooterSlotContent topCenter;
    HeaderFooterSlotContent topRight;
    HeaderFooterSlotContent bottomLeft;
    HeaderFooterSlotContent bottomCenter;
    HeaderFooterSlotContent bottomRight;

    void setSlot(HeaderFooterSlot slot, const QString& text, const QFont& font = QFont("Segoe UI", 10), const QColor& color = Qt::lack)
    {
        HeaderFooterSlotContent content;
        content.templateText = text;
        content.font = font;
        content.textColor = color;
        content.enabled = !text.isEmpty();

        switch (slot)
        {
        case HeaderFooterSlot::TopLeft: topLeft = content; break;
        case HeaderFooterSlot::TopCenter: topCenter = content; break;
        case HeaderFooterSlot::TopRight: topRight = content; break;
        case HeaderFooterSlot::BottomLeft: bottomLeft = content; break;
        case HeaderFooterSlot::BottomCenter: bottomCenter = content; break;
        case HeaderFooterSlot::BottomRight: bottomRight = content; break;
        }
    }
};

} // namespace vectorpdf::tools::decorations

#endif // VECTORPDF_HEADERFOOTERDEFINITION_H
