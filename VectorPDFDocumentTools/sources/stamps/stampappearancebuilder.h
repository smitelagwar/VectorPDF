// MIT License
#ifndef VECTORPDF_STAMPAPPEARANCEBUILDER_H
#define VECTORPDF_STAMPAPPEARANCEBUILDER_H

#include "../documenttoolsglobal.h"
#include "stampdefinition.h"
#include <QPainter>
#include <QImage>

namespace vectorpdf::tools::stamps
{

class VECTORPDF_DOCUMENTTOOLS_EXPORT StampAppearanceBuilder
{
public:
    static QImage renderStampToImage(const StampDefinition& def, qreal dpr = 2.0);
    static void paintStamp(QPainter* painter, const StampDefinition& def, const QRectF& targetRect);
};

} // namespace vectorpdf::tools::stamps

#endif // VECTORPDF_STAMPAPPEARANCEBUILDER_H
