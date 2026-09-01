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

#ifndef VECTORPDF_OUTPUTVERIFICATION_H
#define VECTORPDF_OUTPUTVERIFICATION_H

#include "../conversionglobal.h"
#include "../conversiontypes.h"
#include <QString>

namespace vectorpdf::conversion
{

class VECTORPDF_CONVERSION_EXPORT OutputVerification
{
public:
    static bool verifyOutput(const QString& filePath, ConversionFormat format, QString* errorMessage = nullptr);

    static bool verifyPdf(const QString& filePath, int expectedPages = 0, QString* errorMessage = nullptr);
    static bool verifyImage(const QString& filePath, QString* errorMessage = nullptr);
    static bool verifyDocx(const QString& filePath, QString* errorMessage = nullptr);
    static bool verifyXlsx(const QString& filePath, QString* errorMessage = nullptr);
    static bool verifyPptx(const QString& filePath, QString* errorMessage = nullptr);
    static bool verifyXfdf(const QString& filePath, QString* errorMessage = nullptr);
    static bool verifyFdf(const QString& filePath, QString* errorMessage = nullptr);
};

} // namespace vectorpdf::conversion

#endif // VECTORPDF_OUTPUTVERIFICATION_H
