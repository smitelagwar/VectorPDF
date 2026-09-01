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

#ifndef VECTORPDF_DOCUMENTTOOLRESULT_H
#define VECTORPDF_DOCUMENTTOOLRESULT_H

#include "documenttoolsglobal.h"
#include "documenttooltypes.h"
#include <QString>
#include <QStringList>
#include <QVariantMap>

namespace vectorpdf::tools
{

struct VECTORPDF_DOCUMENTTOOLS_EXPORT DocumentToolResult
{
    ToolExecutionStatus status = ToolExecutionStatus::Success;
    QString safeMessage;
    QStringList warnings;
    QVariantMap metrics;

    bool isSuccessful() const
    {
        return status == ToolExecutionStatus::Success || status == ToolExecutionStatus::SuccessWithWarnings;
    }

    static DocumentToolResult success(const QString& message = QString())
    {
        DocumentToolResult r;
        r.status = ToolExecutionStatus::Success;
        r.safeMessage = message;
        return r;
    }

    static DocumentToolResult successWithWarnings(const QString& message, const QStringList& warnings)
    {
        DocumentToolResult r;
        r.status = ToolExecutionStatus::SuccessWithWarnings;
        r.safeMessage = message;
        r.warnings = warnings;
        return r;
    }

    static DocumentToolResult failure(const QString& errorMessage, ToolExecutionStatus status = ToolExecutionStatus::Failed)
    {
        DocumentToolResult r;
        r.status = status;
        r.safeMessage = errorMessage;
        return r;
    }

    static DocumentToolResult cancelled(const QString& message = QStringLiteral("Operation cancelled by user."))
    {
        DocumentToolResult r;
        r.status = ToolExecutionStatus::Cancelled;
        r.safeMessage = message;
        return r;
    }
};

} // namespace vectorpdf::tools

#endif // VECTORPDF_DOCUMENTTOOLRESULT_H
