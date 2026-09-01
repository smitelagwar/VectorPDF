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

#include "pdfaconformanceanalyzer.h"
#include <pdfdocument.h>
#include <pdfpage.h>
#include <pdffont.h>

namespace vectorpdf::conversion
{

PdfAAnalysisReport PdfAConformanceAnalyzer::analyze(const pdf::PDFDocument* document, PdfAProfile profile)
{
    PdfAAnalysisReport report;
    report.targetProfile = profile;

    if (!document || !document->getCatalog())
    {
        report.canConvertPreservingVector = false;
        report.requiresRasterFallback = false;
        report.blockers.append(QStringLiteral("Document or Catalog is null / invalid."));
        return report;
    }

    // 1. Check Encryption
    if (document->getStorage().isEncrypted())
    {
        report.isEncrypted = true;
        report.remediations.append(QStringLiteral("Remove PDF encryption (PDF/A prohibits encryption)."));
    }

    // 2. Check OutputIntents
    const pdf::PDFCatalog* catalog = document->getCatalog();
    const pdf::PDFObject* outputIntentsObj = catalog->getObject(pdf::PDFName(QStringLiteral("OutputIntents")));
    report.hasOutputIntent = (outputIntentsObj != nullptr && !outputIntentsObj->isNull());
    if (!report.hasOutputIntent)
    {
        report.remediations.append(QStringLiteral("Inject standard sRGB IEC61966-2.1 OutputIntent dictionary with GTS_PDFA1 subtype."));
    }

    // 3. Check JavaScript / Launch / OpenAction
    const pdf::PDFObject* openActionObj = catalog->getObject(pdf::PDFName(QStringLiteral("OpenAction")));
    if (openActionObj && !openActionObj->isNull())
    {
        report.hasLaunchActions = true;
        report.remediations.append(QStringLiteral("Sanitize active OpenAction operators."));
    }

    const pdf::PDFObject* namesObj = catalog->getObject(pdf::PDFName(QStringLiteral("Names")));
    if (namesObj && namesObj->isDictionary())
    {
        const pdf::PDFDictionary* namesDict = namesObj->getDictionary();
        if (namesDict && namesDict->hasKey(pdf::PDFName(QStringLiteral("JavaScript"))))
        {
            report.hasJavaScript = true;
            report.remediations.append(QStringLiteral("Strip JavaScript names tree (forbidden in all PDF/A profiles)."));
        }
    }

    // 4. Check Attachments (PDF/A-1 forbids attachments; PDF/A-3 allows embedded files with AF relationship)
    const pdf::PDFObject* embeddedFilesObj = catalog->getObject(pdf::PDFName(QStringLiteral("EmbeddedFiles")));
    if (embeddedFilesObj && !embeddedFilesObj->isNull())
    {
        report.hasAttachments = true;
        if (profile == PdfAProfile::PdfA1b)
        {
            report.remediations.append(QStringLiteral("Remove embedded file attachments (forbidden in PDF/A-1)."));
        }
        else
        {
            report.remediations.append(QStringLiteral("Normalize Associated Files (AF) relationship dictionary."));
        }
    }

    // 5. Metadata remediation
    report.remediations.append(QStringLiteral("Generate and embed ISO 19005 compliant XMP metadata packet with pdfaid:part=%1 and pdfaid:conformance=%2.")
                               .arg(pdfAProfilePart(profile))
                               .arg(pdfAProfileConformanceString(profile)));

    report.canConvertPreservingVector = true;
    report.requiresRasterFallback = false;

    return report;
}

} // namespace vectorpdf::conversion
