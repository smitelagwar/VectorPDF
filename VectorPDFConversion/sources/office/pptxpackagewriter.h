// MIT License
//
// Copyright (c) 2026 VectorPDF Contributors
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef VECTORPDF_PPTXPACKAGEWRITER_H
#define VECTORPDF_PPTXPACKAGEWRITER_H

#include "../conversionglobal.h"
#include "ooxmlpackagewriter.h"
#include <QString>
#include <QList>
#include <QImage>
#include <QSizeF>

namespace vectorpdf::conversion
{

struct PptxSlideTextBox
{
    QString text;
    QRectF rectPt;
    bool bold = false;
    int fontSizePt = 18;
};

struct PptxSlide
{
    QImage backgroundImage; // Visual Fidelity mode
    QList<PptxSlideTextBox> textBoxes;
    QSizeF slideSizePt = QSizeF(720, 540); // 4:3 default (720x540 pt = 10x7.5 inch)
};

class VECTORPDF_CONVERSION_EXPORT PptxPackageWriter
{
public:
    PptxPackageWriter();
    ~PptxPackageWriter() = default;

    /// Adds a slide with a high-resolution rendered image of the PDF page
    void addVisualSlide(const QImage& pageImage, const QSizeF& pageSizePoints);

    /// Adds an editable slide with text boxes
    void addEditableSlide(const QList<PptxSlideTextBox>& textBoxes, const QSizeF& pageSizePoints);

    /// Generates and saves the .pptx presentation archive
    bool save(const QString& targetFilePath, QString* errorMessage = nullptr);

private:
    void generateRels();
    void generateContentTypes();
    void generateTheme();
    void generateSlideMaster();
    void generateSlideLayout();
    void generatePresentation();
    void generateSlides();

    OoxmlPackageWriter m_pkg;
    QList<PptxSlide> m_slides;
};

} // namespace vectorpdf::conversion

#endif // VECTORPDF_PPTXPACKAGEWRITER_H
