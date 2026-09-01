// MIT License
#ifndef VECTORPDF_STAMPDEFINITION_H
#define VECTORPDF_STAMPDEFINITION_H

#include "../documenttoolsglobal.h"
#include "stampmetadata.h"
#include <QColor>
#include <QFont>
#include <QImage>
#include <QSize>

namespace vectorpdf::tools::stamps
{

enum class StampRenderType
{
    TextBanner,
    DynamicTimestamped,
    Image,
    PdfVector
};

class VECTORPDF_DOCUMENTTOOLS_EXPORT StampDefinition
{
public:
    StampMetadata metadata;
    StampRenderType renderType = StampRenderType::TextBanner;
    QString mainText = QStringLiteral("ONAYLANDI");
    QString subText;
    QColor primaryColor = QColor(0, 150, 0);
    QColor borderColor = QColor(0, 150, 0);
    QFont font = QFont("Segoe UI", 16, QFont::Bold);
    QImage image;
    QSize preferredSize = QSize(180, 60);
    qreal opacity = 0.9;
    qreal rotationAngle = -15.0;
};

} // namespace vectorpdf::tools::stamps

#endif // VECTORPDF_STAMPDEFINITION_H
