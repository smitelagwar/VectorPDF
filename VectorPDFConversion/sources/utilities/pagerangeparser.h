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

#ifndef VECTORPDF_PAGERANGEPARSER_H
#define VECTORPDF_PAGERANGEPARSER_H

#include "../conversionglobal.h"
#include <QString>
#include <QList>

namespace vectorpdf::conversion
{

class VECTORPDF_CONVERSION_EXPORT PageRangeParser
{
public:
    /// Parses a 1-based page range string (e.g. "1-3, 5, 8-10") into a sorted list of 0-based page indices.
    /// totalPages specifies the document's total page count. If totalPages <= 0, no upper bound check is enforced.
    static QList<int> parse(const QString& rangeStr, int totalPages = 0, QString* errorMessage = nullptr);

    /// Formats a list of 0-based page indices back into a 1-based human-readable string (e.g. "1-3, 5, 8-10").
    static QString format(const QList<int>& pages);
};

} // namespace vectorpdf::conversion

#endif // VECTORPDF_PAGERANGEPARSER_H
