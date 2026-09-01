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

#ifndef VECTORPDF_PDFIMAGEEXPORTER_H
#define VECTORPDF_PDFIMAGEEXPORTER_H

#include "../conversionglobal.h"
#include "../conversiontypes.h"
#include "../conversionrequest.h"
#include "../conversionresult.h"
#include "../workers/conversionworkerprotocol.h"
#include <QImage>

namespace pdf
{
class PDFDocument;
}

namespace vectorpdf::conversion
{

class VECTORPDF_CONVERSION_EXPORT PdfImageExporter : public IConversionWorker
{
public:
    PdfImageExporter() = default;
    virtual ~PdfImageExporter() override = default;

    virtual ConversionResult execute(const ConversionRequest& request,
                                     const ProgressCallback& progress = nullptr,
                                     CancelToken* cancelToken = nullptr) override;

    /// Renders a single page of a PDF document to a QImage at specified DPI
    static QImage renderPageToImage(const pdf::PDFDocument* document,
                                    int pageIndex,
                                    int dpi = 300,
                                    bool transparentBackground = false,
                                    bool grayscale = false);
};

} // namespace vectorpdf::conversion

#endif // VECTORPDF_PDFIMAGEEXPORTER_H
