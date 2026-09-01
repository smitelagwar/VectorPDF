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

#include "pdfaconversionreport.h"

namespace vectorpdf::conversion
{

QString PdfAAnalysisReport::toString() const
{
    QStringList lines;
    lines.append(QStringLiteral("PDF/A Target Profile: %1").arg(pdfAProfileToString(targetProfile)));
    lines.append(QStringLiteral("Encrypted: %1").arg(isEncrypted ? QStringLiteral("Yes") : QStringLiteral("No")));
    lines.append(QStringLiteral("Contains JavaScript: %1").arg(hasJavaScript ? QStringLiteral("Yes") : QStringLiteral("No")));
    lines.append(QStringLiteral("Contains Launch Actions: %1").arg(hasLaunchActions ? QStringLiteral("Yes") : QStringLiteral("No")));
    lines.append(QStringLiteral("Contains Non-Embedded Fonts: %1").arg(hasNonEmbeddedFonts ? QStringLiteral("Yes") : QStringLiteral("No")));
    lines.append(QStringLiteral("Strategy: %1").arg(canConvertPreservingVector ? QStringLiteral("Vector-Preserving (Mode A)") : QStringLiteral("Archival Raster Fallback (Mode B)")));
    if (!blockers.isEmpty())
    {
        lines.append(QStringLiteral("Blockers: %1").arg(blockers.join(QStringLiteral("; "))));
    }
    if (!remediations.isEmpty())
    {
        lines.append(QStringLiteral("Planned Remediations: %1").arg(remediations.join(QStringLiteral("; "))));
    }
    return lines.join(QStringLiteral("\n"));
}

QString PdfAConversionReport::formatSummary() const
{
    QStringList lines;
    lines.append(QStringLiteral("=== PDF/A Conformance Report ==="));
    lines.append(QStringLiteral("Profile: %1").arg(pdfAProfileToString(profile)));
    lines.append(QStringLiteral("Mode: %1").arg(mode == PdfATransformationMode::PreserveVector ? QStringLiteral("Vector-Preserving") : QStringLiteral("Archival Raster Fallback")));
    lines.append(QStringLiteral("Conversion Status: %1").arg(conversionSuccessful ? QStringLiteral("Success") : QStringLiteral("Failed")));

    if (isValidatedByVeraPdf)
    {
        lines.append(QStringLiteral("veraPDF Certification: %1").arg(isVeraPdfCompliant ? QStringLiteral("PASS (Strict Conformance Verified)") : QStringLiteral("FAIL (Non-Compliant)")));
        if (!veraPdfStatement.isEmpty())
        {
            lines.append(QStringLiteral("veraPDF Statement: %1").arg(veraPdfStatement));
        }
    }
    else
    {
        lines.append(QStringLiteral("veraPDF Certification: UNVALIDATED (veraPDF CLI is not installed or available)"));
    }

    if (!appliedRemediations.isEmpty())
    {
        lines.append(QStringLiteral("Applied Remediations:"));
        for (const QString& rem : appliedRemediations)
        {
            lines.append(QStringLiteral("  - %1").arg(rem));
        }
    }

    if (!warnings.isEmpty())
    {
        lines.append(QStringLiteral("Warnings:"));
        for (const QString& w : warnings)
        {
            lines.append(QStringLiteral("  - %1").arg(w));
        }
    }

    if (!validationErrors.isEmpty())
    {
        lines.append(QStringLiteral("Validation Errors:"));
        for (const QString& err : validationErrors)
        {
            lines.append(QStringLiteral("  - %1").arg(err));
        }
    }

    return lines.join(QStringLiteral("\n"));
}

} // namespace vectorpdf::conversion
