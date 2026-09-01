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

#ifndef VECTORPDF_PDFACONVERSIONREPORT_H
#define VECTORPDF_PDFACONVERSIONREPORT_H

#include "../conversionglobal.h"
#include "pdfaconformanceprofile.h"
#include <QStringList>
#include <QMap>

namespace vectorpdf::conversion
{

enum class PdfATransformationMode
{
    PreserveVector,
    ArchivalRasterFallback
};

struct VECTORPDF_CONVERSION_EXPORT PdfAAnalysisReport
{
    PdfAProfile targetProfile = PdfAProfile::PdfA2b;
    bool isEncrypted = false;
    bool hasJavaScript = false;
    bool hasLaunchActions = false;
    bool hasExternalLinks = false;
    bool hasNonEmbeddedFonts = false;
    bool hasTransparency = false;
    bool hasAttachments = false;
    bool hasOutputIntent = false;

    bool canConvertPreservingVector = true;
    bool requiresRasterFallback = false;

    QStringList blockers;
    QStringList remediations;

    QString toString() const;
};

struct VECTORPDF_CONVERSION_EXPORT PdfAConversionReport
{
    PdfAProfile profile = PdfAProfile::PdfA2b;
    PdfATransformationMode mode = PdfATransformationMode::PreserveVector;
    bool conversionSuccessful = false;
    bool isValidatedByVeraPdf = false;
    bool isVeraPdfCompliant = false;

    QString outputFilePath;
    QStringList appliedRemediations;
    QStringList warnings;
    QStringList validationErrors;
    QString veraPdfStatement;

    QString formatSummary() const;
};

} // namespace vectorpdf::conversion

#endif // VECTORPDF_PDFACONVERSIONREPORT_H
