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

#include "conversionvalidator.h"
#include "utilities/filenamepolicy.h"
#include <QFile>
#include <QDir>
#include <QFileInfo>

namespace vectorpdf::conversion
{

bool ConversionValidator::validateRequest(const ConversionRequest& request, QString* errorMessage)
{
    if (request.sourcePath.isEmpty() && request.sourcePaths.isEmpty())
    {
        if (errorMessage) *errorMessage = QStringLiteral("No source file or folder specified.");
        return false;
    }

    if (!request.sourcePath.isEmpty() && !QFile::exists(request.sourcePath) && !QDir(request.sourcePath).exists())
    {
        if (errorMessage) *errorMessage = QStringLiteral("Source path does not exist: '%1'").arg(request.sourcePath);
        return false;
    }

    if (FilenamePolicy::isSourceOverwrite(request.sourcePath, request.outputPath))
    {
        if (errorMessage) *errorMessage = QStringLiteral("Target output path cannot be identical to source path.");
        return false;
    }

    return true;
}

bool ConversionValidator::validateResult(const ConversionResult& result, ConversionFormat format, QString* errorMessage)
{
    if (!result.isSuccess())
    {
        if (errorMessage) *errorMessage = result.safeMessage;
        return false;
    }

    if (result.outputPath.isEmpty())
    {
        if (errorMessage) *errorMessage = QStringLiteral("Result does not contain an output path.");
        return false;
    }

    return OutputVerification::verifyOutput(result.outputPath, format, errorMessage);
}

} // namespace vectorpdf::conversion
