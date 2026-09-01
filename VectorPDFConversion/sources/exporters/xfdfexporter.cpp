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

#include "xfdfexporter.h"
#include "../utilities/tempfileguard.h"
#include "../utilities/filenamepolicy.h"
#include "../utilities/outputverification.h"

#include <pdfdocument.h>
#include <pdfdocumentreader.h>
#include <pdfform.h>

#include <QFile>
#include <QFileInfo>
#include <QXmlStreamWriter>
#include <QElapsedTimer>

namespace vectorpdf::conversion
{

ConversionResult XfdfExporter::execute(const ConversionRequest& request,
                                       const ProgressCallback& progress,
                                       CancelToken* cancelToken)
{
    QElapsedTimer timer;
    timer.start();

    if (!QFile::exists(request.sourcePath))
    {
        return ConversionResult::failure(QStringLiteral("FileNotFound"),
                                         QStringLiteral("Source PDF does not exist: '%1'").arg(request.sourcePath));
    }

    if (progress) progress(ConversionStage::Loading, 10, QStringLiteral("Reading PDF document and form fields..."));

    QFile file(request.sourcePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        return ConversionResult::failure(QStringLiteral("OpenFailed"),
                                         QStringLiteral("Cannot open PDF file for reading: '%1'").arg(request.sourcePath));
    }

    QByteArray fileData = file.readAll();
    file.close();

    pdf::PDFDocument document;
    pdf::PDFDocumentReader reader(&document);
    auto readRes = reader.read(fileData);
    if (!readRes.isSuccessful() || !document.getCatalog())
    {
        return ConversionResult::failure(QStringLiteral("ParseFailed"),
                                         QStringLiteral("Failed to parse PDF document: %1").arg(readRes.getMessage()));
    }

    if (cancelToken && cancelToken->load()) return ConversionResult::cancelled();

    if (progress) progress(ConversionStage::Analyzing, 30, QStringLiteral("Extracting AcroForm data..."));

    TempFileGuard tempGuard(QStringLiteral("vpdf_xfdf_"));
    if (!tempGuard.isValid())
    {
        return ConversionResult::failure(QStringLiteral("TempDirError"), QStringLiteral("Failed to create temporary directory."));
    }

    const QString tempXfdfPath = tempGuard.createTempFilePath(QStringLiteral("xfdf"));

    QFile xfdfFile(tempXfdfPath);
    if (!xfdfFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return ConversionResult::failure(QStringLiteral("CreateFailed"), QStringLiteral("Cannot create temporary XFDF file."));
    }

    QXmlStreamWriter xml(&xfdfFile);
    xml.setAutoFormatting(true);
    xml.writeStartDocument(QStringLiteral("1.0"), true);

    xml.writeStartElement(QStringLiteral("xfdf"));
    xml.writeAttribute(QStringLiteral("xmlns"), QStringLiteral("http://ns.adobe.com/xfdf/"));
    xml.writeAttribute(QStringLiteral("xml:space"), QStringLiteral("preserve"));

    // Reference to original PDF
    xml.writeStartElement(QStringLiteral("f"));
    xml.writeAttribute(QStringLiteral("href"), QFileInfo(request.sourcePath).fileName());
    xml.writeEndElement(); // f

    xml.writeStartElement(QStringLiteral("fields"));

    int fieldsExported = 0;
    const pdf::PDFForm* form = document.getCatalog()->getForm();
    if (form)
    {
        for (const auto& fieldPtr : form->getFields())
        {
            if (!fieldPtr) continue;

            const QString name = fieldPtr->getName(pdf::PDFFormField::NameType::FullyQualified);
            const QString val = fieldPtr->getStringValue();

            if (!name.isEmpty())
            {
                xml.writeStartElement(QStringLiteral("field"));
                xml.writeAttribute(QStringLiteral("name"), name);
                xml.writeTextElement(QStringLiteral("value"), val);
                xml.writeEndElement(); // field
                fieldsExported++;
            }
        }
    }

    xml.writeEndElement(); // fields

    // Annotations block
    xml.writeStartElement(QStringLiteral("annots"));
    xml.writeEndElement(); // annots

    xml.writeEndElement(); // xfdf
    xml.writeEndDocument();
    xfdfFile.close();

    if (progress) progress(ConversionStage::Validating, 85, QStringLiteral("Validating XFDF output..."));

    QString verifyErr;
    if (!OutputVerification::verifyXfdf(tempXfdfPath, &verifyErr))
    {
        return ConversionResult::failure(QStringLiteral("ValidationFailed"), QStringLiteral("XFDF validation failed: %1").arg(verifyErr));
    }

    QString finalOutPath = request.outputPath.isEmpty()
                               ? FilenamePolicy::computeDefaultOutputPath(request.sourcePath, ConversionFormat::Xfdf, request.outputDirectory)
                               : request.outputPath;
    finalOutPath = FilenamePolicy::resolveCollision(finalOutPath, request.overwriteExisting);

    if (progress) progress(ConversionStage::Committing, 95, QStringLiteral("Committing XFDF file..."));

    QString commitErr;
    if (!TempFileGuard::atomicCommit(tempXfdfPath, finalOutPath, &commitErr))
    {
        return ConversionResult::failure(QStringLiteral("CommitFailed"), QStringLiteral("Failed to commit final XFDF: %1").arg(commitErr));
    }

    if (progress) progress(ConversionStage::Completed, 100, QStringLiteral("XFDF export completed."));

    ConversionResult res = ConversionResult::success(finalOutPath);
    res.metrics[QStringLiteral("FieldsExported")] = fieldsExported;
    res.elapsedMilliseconds = timer.elapsed();
    res.outputSizeBytes = QFileInfo(finalOutPath).size();
    return res;
}

} // namespace vectorpdf::conversion
