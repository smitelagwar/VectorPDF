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

#ifndef VECTORPDF_CONVERSIONRESULT_H
#define VECTORPDF_CONVERSIONRESULT_H

#include "conversiontypes.h"
#include <QString>
#include <QStringList>
#include <QVariantMap>

namespace vectorpdf::conversion
{

struct VECTORPDF_CONVERSION_EXPORT ConversionResult
{
    QString jobId;
    ConversionStatus status = ConversionStatus::Queued;
    QString outputPath;
    QStringList generatedFiles;
    QString errorCode;
    QString safeMessage;
    QStringList warnings;
    QVariantMap metrics;

    int totalPagesProcessed = 0;
    qint64 elapsedMilliseconds = 0;
    qint64 outputSizeBytes = 0;

    bool isSuccess() const
    {
        return status == ConversionStatus::Success || status == ConversionStatus::SuccessWithWarnings;
    }

    static ConversionResult success(const QString& outputPath, const QStringList& generatedFiles = {})
    {
        ConversionResult r;
        r.status = ConversionStatus::Success;
        r.outputPath = outputPath;
        r.generatedFiles = generatedFiles.isEmpty() && !outputPath.isEmpty() ? QStringList{outputPath} : generatedFiles;
        r.safeMessage = QStringLiteral("Conversion completed successfully.");
        return r;
    }

    static ConversionResult failure(const QString& errorCode, const QString& safeMessage)
    {
        ConversionResult r;
        r.status = ConversionStatus::Failed;
        r.errorCode = errorCode;
        r.safeMessage = safeMessage;
        return r;
    }

    static ConversionResult cancelled(const QString& safeMessage = QStringLiteral("Conversion cancelled by user."))
    {
        ConversionResult r;
        r.status = ConversionStatus::Cancelled;
        r.safeMessage = safeMessage;
        return r;
    }

    static ConversionResult unsupported(const QString& safeMessage = QStringLiteral("Conversion format or feature is not supported."))
    {
        ConversionResult r;
        r.status = ConversionStatus::Unsupported;
        r.safeMessage = safeMessage;
        return r;
    }
};

} // namespace vectorpdf::conversion

#endif // VECTORPDF_CONVERSIONRESULT_H
