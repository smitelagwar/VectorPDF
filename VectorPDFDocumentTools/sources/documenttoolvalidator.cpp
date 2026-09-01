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

#include "documenttoolvalidator.h"
#include "pdfdocument.h"
#include <QFileInfo>
#include <QFile>

namespace vectorpdf::tools
{

bool DocumentToolValidator::isSignedDocument(const pdf::PDFDocument* document)
{
    if (!document)
    {
        return false;
    }
    // Check if document has signature fields or digital signatures
    return document->isSigned();
}

DocumentToolResult DocumentToolValidator::validateMutationAllowed(const pdf::PDFDocument* document, bool allowSignedMutation)
{
    if (!document)
    {
        return DocumentToolResult::failure(QStringLiteral("No active PDF document provided."), ToolExecutionStatus::ValidationFailed);
    }

    if (isSignedDocument(document) && !allowSignedMutation)
    {
        return DocumentToolResult::failure(
            QStringLiteral("Document contains digital signatures. Direct in-place mutation will invalidate signatures. Please save as a new copy."),
            ToolExecutionStatus::ValidationFailed
        );
    }

    return DocumentToolResult::success();
}

DocumentToolResult DocumentToolValidator::validateTemporaryOutputFile(const QString& tempFilePath)
{
    QFileInfo info(tempFilePath);
    if (!info.exists() || info.size() <= 0)
    {
        return DocumentToolResult::failure(QStringLiteral("Generated temporary output file is missing or empty: ") + tempFilePath);
    }

    QFile file(tempFilePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        return DocumentToolResult::failure(QStringLiteral("Cannot read generated output file for validation: ") + tempFilePath);
    }

    QByteArray header = file.read(1024);
    file.close();

    if (!header.startsWith("%PDF-"))
    {
        return DocumentToolResult::failure(QStringLiteral("Generated output is not a valid PDF file structure."));
    }

    return DocumentToolResult::success();
}

} // namespace vectorpdf::tools
