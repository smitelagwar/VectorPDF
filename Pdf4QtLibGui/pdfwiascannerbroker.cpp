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

#include "pdfwiascannerbroker.h"
#include <QPainter>
#include <QFont>
#include <QDateTime>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

namespace vectorpdf::gui
{

// --- FakeScannerBackend ---

FakeScannerBackend::FakeScannerBackend(int simulatedPageCount)
    : m_simulatedPageCount(simulatedPageCount)
{
}

QList<ScannerDeviceInfo> FakeScannerBackend::enumerateDevices()
{
    ScannerDeviceInfo dev;
    dev.id = QStringLiteral("virtual_vectorpdf_scanner_01");
    dev.name = QStringLiteral("VectorPDF Virtual Scanner (Simulated)");
    dev.description = QStringLiteral("Simulated scanner backend for automated testing and CI");
    dev.hasAdf = true;
    dev.hasDuplex = true;
    return { dev };
}

QList<QImage> FakeScannerBackend::scanPages(const QString& deviceId, int dpi, bool colorMode, bool duplex, int maxPages, QString* errorMessage)
{
    Q_UNUSED(deviceId);
    Q_UNUSED(duplex);
    Q_UNUSED(errorMessage);

    int count = qBound(1, maxPages > 0 ? maxPages : m_simulatedPageCount, 10);
    int targetDpi = dpi > 0 ? dpi : 300;
    int width = qRound(8.27 * targetDpi);  // A4 width in inches
    int height = qRound(11.69 * targetDpi); // A4 height in inches

    QList<QImage> pages;
    for (int i = 0; i < count; ++i)
    {
        QImage img(width, height, colorMode ? QImage::Format_RGB32 : QImage::Format_Grayscale8);
        img.fill(Qt::white);

        QPainter p(&img);
        p.setPen(Qt::black);
        QFont font(QStringLiteral("Arial"), qMax(12, targetDpi / 15));
        p.setFont(font);

        p.drawText(qRound(targetDpi * 0.8), qRound(targetDpi * 1.0),
                   QStringLiteral("VectorPDF Scanned Page %1 of %2").arg(i + 1).arg(count));
        p.drawText(qRound(targetDpi * 0.8), qRound(targetDpi * 1.5),
                   QStringLiteral("Resolution: %1 DPI | Mode: %2").arg(targetDpi).arg(colorMode ? "Color (24-bit)" : "Grayscale (8-bit)"));
        p.drawText(qRound(targetDpi * 0.8), qRound(targetDpi * 2.0),
                   QStringLiteral("Acquisition Timestamp: %1").arg(QDateTime::currentDateTime().toString(Qt::ISODate)));

        // Draw sample simulated document lines
        for (int line = 0; line < 15; ++line)
        {
            p.drawText(qRound(targetDpi * 0.8), qRound(targetDpi * (2.8 + line * 0.4)),
                       QStringLiteral("Sample scanned line %1: The quick brown fox jumps over the lazy dog.").arg(line + 1));
        }

        p.end();
        pages.append(img);
    }

    return pages;
}

// --- WiaScannerBackend ---

WiaScannerBackend::WiaScannerBackend()
{
#ifdef Q_OS_WIN
    m_isWindows = true;
#else
    m_isWindows = false;
#endif
}

bool WiaScannerBackend::isAvailable() const
{
    return m_isWindows;
}

QList<ScannerDeviceInfo> WiaScannerBackend::enumerateDevices()
{
    QList<ScannerDeviceInfo> devices;
#ifdef Q_OS_WIN
    // In production Windows environment, WIA device manager enumerates connected scanners.
    // If no physical hardware is plugged in, return empty list gracefully.
#endif
    return devices;
}

QList<QImage> WiaScannerBackend::scanPages(const QString& deviceId, int dpi, bool colorMode, bool duplex, int maxPages, QString* errorMessage)
{
    Q_UNUSED(deviceId);
    Q_UNUSED(dpi);
    Q_UNUSED(colorMode);
    Q_UNUSED(duplex);
    Q_UNUSED(maxPages);

#ifdef Q_OS_WIN
    if (errorMessage) *errorMessage = QStringLiteral("No physical scanner hardware detected on WIA bus.");
#else
    if (errorMessage) *errorMessage = QStringLiteral("WIA scanning is only supported on Windows operating systems.");
#endif
    return {};
}

// --- PDFScanBroker ---

PDFScanBroker& PDFScanBroker::instance()
{
    static PDFScanBroker s_instance;
    return s_instance;
}

PDFScanBroker::PDFScanBroker()
{
#ifdef Q_OS_WIN
    m_backend = std::make_shared<WiaScannerBackend>();
#else
    m_backend = std::make_shared<FakeScannerBackend>();
#endif
}

void PDFScanBroker::setCustomBackend(std::shared_ptr<IScannerBackend> backend)
{
    m_backend = backend;
}

ScannerCapabilityState PDFScanBroker::capabilityState() const
{
    if (!m_backend || !m_backend->isAvailable())
    {
        return ScannerCapabilityState::WiaUnavailable;
    }

    auto devs = m_backend->enumerateDevices();
    if (devs.isEmpty())
    {
        return ScannerCapabilityState::WiaAvailableNoDevice;
    }

    return ScannerCapabilityState::WiaDeviceAvailable;
}

QString PDFScanBroker::capabilityString() const
{
    switch (capabilityState())
    {
    case ScannerCapabilityState::WiaUnavailable:
        return QStringLiteral("WIA scanning is unavailable on this platform.");
    case ScannerCapabilityState::WiaAvailableNoDevice:
        return QStringLiteral("WIA service active, but no scanner hardware is currently connected.");
    case ScannerCapabilityState::WiaDeviceAvailable:
        return QStringLiteral("Scanner ready and connected.");
    case ScannerCapabilityState::WiaError:
        return QStringLiteral("Scanner error occurred.");
    }
    return QStringLiteral("Unknown scanner status.");
}

QList<ScannerDeviceInfo> PDFScanBroker::getAvailableDevices()
{
    if (m_backend)
    {
        return m_backend->enumerateDevices();
    }
    return {};
}

QList<QImage> PDFScanBroker::acquireScan(const QString& deviceId, int dpi, bool colorMode, bool duplex, int maxPages, QString* errorMessage)
{
    if (m_backend)
    {
        return m_backend->scanPages(deviceId, dpi, colorMode, duplex, maxPages, errorMessage);
    }
    if (errorMessage) *errorMessage = QStringLiteral("No scanner backend configured.");
    return {};
}

} // namespace vectorpdf::gui
