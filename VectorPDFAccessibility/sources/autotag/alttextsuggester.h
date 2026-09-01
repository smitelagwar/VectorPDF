// MIT License
#ifndef VECTORPDF_ALTTEXTSUGGESTER_H
#define VECTORPDF_ALTTEXTSUGGESTER_H

#include "../accessibilityglobal.h"
#include <QString>
#include <QImage>

namespace vectorpdf::a11y
{

class VECTORPDF_ACCESSIBILITY_EXPORT AltTextSuggester
{
public:
    static QString suggestAltText(const QImage& figureImage, const QString& surroundingContext = QString());
};

} // namespace vectorpdf::a11y

#endif // VECTORPDF_ALTTEXTSUGGESTER_H
