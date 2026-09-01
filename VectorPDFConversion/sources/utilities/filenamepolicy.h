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

#ifndef VECTORPDF_FILENAMEPOLICY_H
#define VECTORPDF_FILENAMEPOLICY_H

#include "../conversionglobal.h"
#include "../conversiontypes.h"
#include <QString>

namespace vectorpdf::conversion
{

class VECTORPDF_CONVERSION_EXPORT FilenamePolicy
{
public:
    /// Computes default output file path based on source path and target format
    static QString computeDefaultOutputPath(const QString& sourcePath, ConversionFormat format, const QString& targetDirectory = QString());

    /// Formats a page-by-page file name using a pattern like "{document}_page_{page:000}.png"
    static QString formatPageFileName(const QString& templatePattern, const QString& documentBaseName, int pageIndex0Based, const QString& extension);

    /// Ensures a file path does not collide with an existing file by appending " (1)", " (2)", etc., unless overwrite is true
    static QString resolveCollision(const QString& desiredFilePath, bool overwrite = false);

    /// Checks if target path is identical to source path (to protect original documents)
    static bool isSourceOverwrite(const QString& sourcePath, const QString& targetPath);
};

} // namespace vectorpdf::conversion

#endif // VECTORPDF_FILENAMEPOLICY_H
