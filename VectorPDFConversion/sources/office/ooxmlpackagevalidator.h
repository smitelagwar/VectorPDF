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

#ifndef VECTORPDF_OOXMLPACKAGEVALIDATOR_H
#define VECTORPDF_OOXMLPACKAGEVALIDATOR_H

#include "../conversionglobal.h"
#include "../conversiontypes.h"
#include <QString>
#include <QStringList>

namespace vectorpdf::conversion
{

struct OoxmlValidationResult
{
    bool isValid = false;
    QStringList archiveEntries;
    QStringList missingRequiredParts;
    QStringList xmlValidationErrors;
    QString errorMessage;
};

class VECTORPDF_CONVERSION_EXPORT OoxmlPackageValidator
{
public:
    /// Validates an OOXML package (.docx, .xlsx, .pptx)
    static OoxmlValidationResult validatePackage(const QString& filePath, ConversionFormat format);

    /// Extracts list of filenames stored in the ZIP archive
    static QStringList listZipEntries(const QString& zipFilePath, QString* errorMessage = nullptr);
};

} // namespace vectorpdf::conversion

#endif // VECTORPDF_OOXMLPACKAGEVALIDATOR_H
