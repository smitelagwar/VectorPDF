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

#ifndef VECTORPDF_VERAPDFWORKER_H
#define VECTORPDF_VERAPDFWORKER_H

#include "conversionworkerprotocol.h"
#include <QString>

namespace vectorpdf::conversion
{

struct VeraPdfValidationReport
{
    bool isValidated = false;
    bool isCompliant = false;
    QString profile;
    QString statement;
    QStringList failedRules;
};

class VECTORPDF_CONVERSION_EXPORT VeraPdfWorker
{
public:
    explicit VeraPdfWorker(const QString& executablePath = QString());

    bool isAvailable() const;
    QString executablePath() const;

    /// Validates a PDF file against the requested PDF/A profile
    VeraPdfValidationReport validate(const QString& pdfFilePath, ConversionFormat profile, CancelToken* cancelToken = nullptr);

private:
    VeraPdfValidationReport validateInternalStructural(const QString& pdfFilePath, ConversionFormat profile);

    QString m_executablePath;
};

} // namespace vectorpdf::conversion

#endif // VECTORPDF_VERAPDFWORKER_H
