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

#ifndef VECTORPDF_MULTIPAGETIFFWRITER_H
#define VECTORPDF_MULTIPAGETIFFWRITER_H

#include "../conversionglobal.h"
#include <QImage>
#include <QList>
#include <QString>

namespace vectorpdf::conversion
{

class VECTORPDF_CONVERSION_EXPORT MultiPageTiffWriter
{
public:
    /// Writes multiple QImages as consecutive directories (pages) in a single TIFF file
    static bool writeMultiPageTiff(const QList<QImage>& pages,
                                   const QString& targetTiffPath,
                                   int dpi = 300,
                                   QString* errorMessage = nullptr);

    /// Reads and counts the number of Image File Directories (pages) in a TIFF file
    static int countDirectories(const QString& tiffPath, QString* errorMessage = nullptr);
};

} // namespace vectorpdf::conversion

#endif // VECTORPDF_MULTIPAGETIFFWRITER_H
