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

#ifndef VECTORPDF_PDFACONFORMANCEPROFILE_H
#define VECTORPDF_PDFACONFORMANCEPROFILE_H

#include "../conversionglobal.h"
#include "../conversiontypes.h"
#include <QString>

namespace vectorpdf::conversion
{

enum class PdfAProfile
{
    PdfA1b,
    PdfA2b,
    PdfA3b,
    PdfA4
};

inline PdfAProfile conversionFormatToPdfAProfile(ConversionFormat format)
{
    switch (format)
    {
    case ConversionFormat::PdfA1: return PdfAProfile::PdfA1b;
    case ConversionFormat::PdfA2: return PdfAProfile::PdfA2b;
    case ConversionFormat::PdfA3: return PdfAProfile::PdfA3b;
    case ConversionFormat::PdfA4: return PdfAProfile::PdfA4;
    default:                      return PdfAProfile::PdfA2b;
    }
}

inline ConversionFormat pdfAProfileToConversionFormat(PdfAProfile profile)
{
    switch (profile)
    {
    case PdfAProfile::PdfA1b: return ConversionFormat::PdfA1;
    case PdfAProfile::PdfA2b: return ConversionFormat::PdfA2;
    case PdfAProfile::PdfA3b: return ConversionFormat::PdfA3;
    case PdfAProfile::PdfA4:  return ConversionFormat::PdfA4;
    }
    return ConversionFormat::PdfA2;
}

inline QString pdfAProfileToString(PdfAProfile profile)
{
    switch (profile)
    {
    case PdfAProfile::PdfA1b: return QStringLiteral("PDF/A-1b");
    case PdfAProfile::PdfA2b: return QStringLiteral("PDF/A-2b");
    case PdfAProfile::PdfA3b: return QStringLiteral("PDF/A-3b");
    case PdfAProfile::PdfA4:  return QStringLiteral("PDF/A-4");
    }
    return QStringLiteral("PDF/A-2b");
}

inline int pdfAProfilePart(PdfAProfile profile)
{
    switch (profile)
    {
    case PdfAProfile::PdfA1b: return 1;
    case PdfAProfile::PdfA2b: return 2;
    case PdfAProfile::PdfA3b: return 3;
    case PdfAProfile::PdfA4:  return 4;
    }
    return 2;
}

inline QString pdfAProfileConformanceString(PdfAProfile profile)
{
    switch (profile)
    {
    case PdfAProfile::PdfA1b: return QStringLiteral("B");
    case PdfAProfile::PdfA2b: return QStringLiteral("B");
    case PdfAProfile::PdfA3b: return QStringLiteral("B");
    case PdfAProfile::PdfA4:  return QStringLiteral("F");
    }
    return QStringLiteral("B");
}

} // namespace vectorpdf::conversion

#endif // VECTORPDF_PDFACONFORMANCEPROFILE_H
