// MIT License
#ifndef VECTORPDF_SHADOWREMOVER_H
#define VECTORPDF_SHADOWREMOVER_H

#include "../scantoolsglobal.h"
#include <QImage>

namespace vectorpdf::scan
{

class VECTORPDF_SCANTOOLS_EXPORT ShadowRemover
{
public:
    static QImage removeBorderShadows(const QImage& image);
};

} // namespace vectorpdf::scan

#endif // VECTORPDF_SHADOWREMOVER_H
