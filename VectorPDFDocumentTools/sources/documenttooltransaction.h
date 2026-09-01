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

#ifndef VECTORPDF_DOCUMENTTOOLTRANSACTION_H
#define VECTORPDF_DOCUMENTTOOLTRANSACTION_H

#include "documenttoolsglobal.h"
#include "documenttoolresult.h"
#include <QString>
#include <functional>
#include <memory>

namespace pdf
{
class PDFDocument;
}

namespace vectorpdf::tools
{

class VECTORPDF_DOCUMENTTOOLS_EXPORT DocumentToolTransaction
{
public:
    explicit DocumentToolTransaction(const QString& transactionName);
    virtual ~DocumentToolTransaction() = default;

    QString name() const;
    virtual DocumentToolResult execute(pdf::PDFDocument* targetDocument) = 0;
    virtual DocumentToolResult rollback(pdf::PDFDocument* targetDocument) = 0;

private:
    QString m_name;
};

} // namespace vectorpdf::tools

#endif // VECTORPDF_DOCUMENTTOOLTRANSACTION_H
