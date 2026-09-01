// MIT License
#ifndef VECTORPDF_PUNCHHOLEREMOVER_H
#define VECTORPDF_PUNCHHOLEREMOVER_H

#include "../scantoolsglobal.h"
#include <QImage>

namespace vectorpdf::scan
{

class VECTORPDF_SCANTOOLS_EXPORT PunchHoleRemover
{
public:
    static QImage removePunchHoles(const QImage& image, int& removedCount);
};

} // namespace vectorpdf::scan

#endif // VECTORPDF_PUNCHHOLEREMOVER_H
