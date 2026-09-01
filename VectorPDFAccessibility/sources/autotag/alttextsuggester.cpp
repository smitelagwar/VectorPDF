// MIT License
#include "alttextsuggester.h"

namespace vectorpdf::a11y
{

QString AltTextSuggester::suggestAltText(const QImage& figureImage, const QString& surroundingContext)
{
    if (figureImage.isNull())
        return QStringLiteral("Figure illustration");

    if (!surroundingContext.isEmpty())
    {
        return QStringLiteral("Illustration depicting: ") + surroundingContext.left(50);
    }
    return QStringLiteral("Diagram showing visual data overview");
}

} // namespace vectorpdf::a11y
