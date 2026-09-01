// MIT License
#ifndef VECTORPDF_BATESDEFINITION_H
#define VECTORPDF_BATESDEFINITION_H

#include "../documenttoolsglobal.h"
#include "../decorations/pagedecorationdefinition.h"
#include "batessequence.h"
#include <QFont>
#include <QColor>

namespace vectorpdf::tools::bates
{

enum class BatesPosition
{
    TopLeft,
    TopCenter,
    TopRight,
    BottomLeft,
    BottomCenter,
    BottomRight
};

class VECTORPDF_DOCUMENTTOOLS_EXPORT BatesDefinition
{
public:
    BatesSequence sequence{QStringLiteral("DOC-"), 1, 6, QString()};
    BatesPosition position = BatesPosition::BottomRight;
    QFont font = QFont("Segoe UI", 10);
    QColor textColor = Qt::black;
    qreal marginPt = 36.0; // 0.5 inch
    decorations::PageDecorationTargetPages targetPages;
};

} // namespace vectorpdf::tools::bates

#endif // VECTORPDF_BATESDEFINITION_H
