// MIT License
#ifndef VECTORPDF_DESKEWPROCESSOR_H
#define VECTORPDF_DESKEWPROCESSOR_H

#include "../scantoolsglobal.h"
#include <QImage>

namespace vectorpdf::scan
{

class VECTORPDF_SCANTOOLS_EXPORT DeskewProcessor
{
public:
    static double estimateSkewAngle(const QImage& image);
    static QImage deskewImage(const QImage& image, double angle);
};

} // namespace vectorpdf::scan

#endif // VECTORPDF_DESKEWPROCESSOR_H
