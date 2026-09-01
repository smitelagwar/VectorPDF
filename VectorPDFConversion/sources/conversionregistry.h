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

#ifndef VECTORPDF_CONVERSIONREGISTRY_H
#define VECTORPDF_CONVERSIONREGISTRY_H

#include "conversionglobal.h"
#include "conversiontypes.h"
#include "conversionrequest.h"
#include "conversionresult.h"
#include "workers/conversionworkerprotocol.h"

#include <memory>
#include <QMap>

namespace vectorpdf::conversion
{

class VECTORPDF_CONVERSION_EXPORT ConversionRegistry
{
public:
    static ConversionRegistry& instance();

    /// Finds the appropriate worker for an export request from PDF
    std::shared_ptr<IConversionWorker> getExporter(ConversionFormat format) const;

    /// Finds the appropriate worker for a creation request to PDF
    std::shared_ptr<IConversionWorker> getCreator(const QString& sourceExtensionOrFolder) const;

    /// Registers a custom exporter
    void registerExporter(ConversionFormat format, std::shared_ptr<IConversionWorker> exporter);

    /// Registers a custom creator
    void registerCreator(const QString& extension, std::shared_ptr<IConversionWorker> creator);

private:
    ConversionRegistry();
    void registerBuiltInWorkers();

    QMap<ConversionFormat, std::shared_ptr<IConversionWorker>> m_exporters;
    QMap<QString, std::shared_ptr<IConversionWorker>> m_creators;
};

} // namespace vectorpdf::conversion

#endif // VECTORPDF_CONVERSIONREGISTRY_H
