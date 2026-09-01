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

#ifndef VECTORPDF_PDFWIASCANNERBROKER_H
#define VECTORPDF_PDFWIASCANNERBROKER_H

#include <QString>
#include <QStringList>
#include <QImage>
#include <QList>
#include <memory>

namespace vectorpdf::gui
{

enum class ScannerCapabilityState
{
    WiaUnavailable,
    WiaAvailableNoDevice,
    WiaDeviceAvailable,
    WiaError
};

struct ScannerDeviceInfo
{
    QString id;
    QString name;
    QString description;
    bool hasAdf = false;
    bool hasDuplex = false;
};

class IScannerBackend
{
public:
    virtual ~IScannerBackend() = default;
    virtual bool isAvailable() const = 0;
    virtual QList<ScannerDeviceInfo> enumerateDevices() = 0;
    virtual QList<QImage> scanPages(const QString& deviceId, int dpi, bool colorMode, bool duplex, int maxPages = 1, QString* errorMessage = nullptr) = 0;
};

class FakeScannerBackend : public IScannerBackend
{
public:
    FakeScannerBackend(int simulatedPageCount = 1);
    virtual bool isAvailable() const override { return true; }
    virtual QList<ScannerDeviceInfo> enumerateDevices() override;
    virtual QList<QImage> scanPages(const QString& deviceId, int dpi, bool colorMode, bool duplex, int maxPages = 1, QString* errorMessage = nullptr) override;

private:
    int m_simulatedPageCount = 1;
};

class WiaScannerBackend : public IScannerBackend
{
public:
    WiaScannerBackend();
    virtual ~WiaScannerBackend() override = default;

    virtual bool isAvailable() const override;
    virtual QList<ScannerDeviceInfo> enumerateDevices() override;
    virtual QList<QImage> scanPages(const QString& deviceId, int dpi, bool colorMode, bool duplex, int maxPages = 1, QString* errorMessage = nullptr) override;

private:
    bool m_isWindows = false;
};

class PDFScanBroker
{
public:
    static PDFScanBroker& instance();

    ScannerCapabilityState capabilityState() const;
    QString capabilityString() const;

    QList<ScannerDeviceInfo> getAvailableDevices();
    QList<QImage> acquireScan(const QString& deviceId, int dpi = 300, bool colorMode = true, bool duplex = false, int maxPages = 1, QString* errorMessage = nullptr);

    void setCustomBackend(std::shared_ptr<IScannerBackend> backend);

private:
    PDFScanBroker();
    std::shared_ptr<IScannerBackend> m_backend;
};

} // namespace vectorpdf::gui

#endif // VECTORPDF_PDFWIASCANNERBROKER_H
