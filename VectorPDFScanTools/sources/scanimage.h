// MIT License
#ifndef VECTORPDF_SCANIMAGE_H
#define VECTORPDF_SCANIMAGE_H

#include "scantoolsglobal.h"
#include <QImage>

namespace vectorpdf::scan
{

class VECTORPDF_SCANTOOLS_EXPORT ScanImage
{
public:
    ScanImage() = default;
    explicit ScanImage(const QImage& image);

    QImage image() const { return m_image; }
    void setImage(const QImage& img) { m_image = img; }
    bool isNull() const { return m_image.isNull(); }

    int width() const { return m_image.width(); }
    int height() const { return m_image.height(); }

private:
    QImage m_image;
};

} // namespace vectorpdf::scan

#endif // VECTORPDF_SCANIMAGE_H
