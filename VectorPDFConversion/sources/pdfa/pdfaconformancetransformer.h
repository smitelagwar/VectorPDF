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

#ifndef VECTORPDF_PDFACONFORMANCETRANSFORMER_H
#define VECTORPDF_PDFACONFORMANCETRANSFORMER_H

#include "../conversionglobal.h"
#include "pdfaconformanceprofile.h"
#include "pdfaconversionreport.h"

namespace pdf
{
class PDFDocument;
}

namespace vectorpdf::conversion
{

class VECTORPDF_CONVERSION_EXPORT PdfAConformanceTransformer
{
public:
    /// Transforms the document into a candidate PDF/A file at targetPath
    static bool transform(const pdf::PDFDocument* document,
                          PdfAProfile profile,
                          PdfATransformationMode mode,
                          const QString& targetPath,
                          PdfAConversionReport* outReport,
                          QString* errorMessage = nullptr);

    /// Generates strict ISO 19005 compliant XMP XML metadata packet
    static QByteArray generatePdfAXmpPacket(PdfAProfile profile,
                                            const QString& title = QStringLiteral("VectorPDF Document"),
                                            const QString& producer = QStringLiteral("VectorPDF Archival Engine"));
};

} // namespace vectorpdf::conversion

#endif // VECTORPDF_PDFACONFORMANCETRANSFORMER_H
