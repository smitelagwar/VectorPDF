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

#ifndef VECTORPDF_CONVERSIONCAPABILITY_H
#define VECTORPDF_CONVERSIONCAPABILITY_H

#include "conversiontypes.h"
#include "conversionrequest.h"
#include <QString>
#include <QList>
#include <QMap>

namespace vectorpdf::conversion
{

struct VECTORPDF_CONVERSION_EXPORT CapabilityInfo
{
    bool isAvailable = true;
    bool isLossy = false;
    QString engineName;
    QString note;
    QString toolExecutablePath;
};

class VECTORPDF_CONVERSION_EXPORT ConversionCapability
{
public:
    static ConversionCapability& instance();

    bool isExportSupported(ConversionFormat format) const;
    bool isCreationSupported(const QString& extension) const;

    CapabilityInfo getExportCapability(ConversionFormat format) const;
    CapabilityInfo getCreationCapability(const QString& extension) const;

    bool isLibreOfficeAvailable() const;
    QString getLibreOfficePath() const;
    void setLibreOfficePath(const QString& path);

    bool isVeraPdfAvailable() const;
    QString getVeraPdfPath() const;
    void setVeraPdfPath(const QString& path);

    bool isWiaScannerAvailable() const;

    QList<ConversionFormat> getSupportedExportFormats() const;
    QStringList getSupportedCreationExtensions() const;

    void refreshCapabilities();

private:
    ConversionCapability();

    void detectLibreOffice();
    void detectVeraPdf();
    void detectWia();

    QString m_libreOfficePath;
    bool m_libreOfficeAvailable = false;

    QString m_veraPdfPath;
    bool m_veraPdfAvailable = false;

    bool m_wiaAvailable = false;
};

} // namespace vectorpdf::conversion

#endif // VECTORPDF_CONVERSIONCAPABILITY_H
