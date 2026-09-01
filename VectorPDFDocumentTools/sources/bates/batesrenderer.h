// MIT License
#ifndef VECTORPDF_BATESRENDERER_H
#define VECTORPDF_BATESRENDERER_H

#include "../documenttoolsglobal.h"
#include "batesdefinition.h"
#include <QPainter>
#include <QRectF>

namespace vectorpdf::tools::bates
{

class VECTORPDF_DOCUMENTTOOLS_EXPORT BatesRenderer
{
public:
    static void renderBatesStamp(QPainter* painter, const QString& batesText, const BatesDefinition& def, const QRectF& pageBounds);
};

} // namespace vectorpdf::tools::bates

#endif // VECTORPDF_BATESRENDERER_H
