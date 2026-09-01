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

#include "libreofficeworker.h"
#include "../conversioncapability.h"
#include "../utilities/tempfileguard.h"
#include "../utilities/outputverification.h"
#include <QProcess>
#include <QFileInfo>
#include <QDir>
#include <QElapsedTimer>
#include <QThread>

namespace vectorpdf::conversion
{

LibreOfficeWorker::LibreOfficeWorker(const QString& executablePath)
    : m_executablePath(executablePath)
{
    if (m_executablePath.isEmpty())
    {
        m_executablePath = ConversionCapability::instance().getLibreOfficePath();
    }
}

bool LibreOfficeWorker::isAvailable() const
{
    return !m_executablePath.isEmpty() && QFileInfo::exists(m_executablePath);
}

QString LibreOfficeWorker::executablePath() const
{
    return m_executablePath;
}

ConversionResult LibreOfficeWorker::execute(const ConversionRequest& request,
                                            const ProgressCallback& progress,
                                            CancelToken* cancelToken)
{
    if (!isAvailable())
    {
        return ConversionResult::unsupported(QStringLiteral("LibreOffice executable not found. Please install LibreOffice to convert Office files."));
    }

    if (!QFile::exists(request.sourcePath))
    {
        return ConversionResult::failure(QStringLiteral("FileNotFound"), QStringLiteral("Source file does not exist: '%1'").arg(request.sourcePath));
    }

    if (progress) progress(ConversionStage::Preparing, 10, QStringLiteral("Preparing isolated conversion sandbox..."));

    TempFileGuard tempGuard(QStringLiteral("vpdf_lo_"));
    if (!tempGuard.isValid())
    {
        return ConversionResult::failure(QStringLiteral("TempDirError"), QStringLiteral("Failed to create temporary directory for LibreOffice."));
    }

    const QString profileDir = tempGuard.dirPath() + QStringLiteral("/profile");
    const QString outDir = tempGuard.dirPath() + QStringLiteral("/out");
    QDir().mkpath(profileDir);
    QDir().mkpath(outDir);

    // Profile URL in standard file:/// format
    const QString profileUrl = QStringLiteral("-env:UserInstallation=file:///%1").arg(QDir::toNativeSeparators(profileDir).replace(QLatin1Char('\\'), QLatin1Char('/')));

    QStringList arguments;
    arguments << profileUrl;
    arguments << QStringLiteral("--headless");
    arguments << QStringLiteral("--convert-to");
    arguments << QStringLiteral("pdf");
    arguments << QStringLiteral("--outdir");
    arguments << outDir;
    arguments << request.sourcePath;

    if (progress) progress(ConversionStage::Rendering, 30, QStringLiteral("Running LibreOffice converter in background..."));

    QProcess process;
    process.setProgram(m_executablePath);
    process.setArguments(arguments);
    process.start();

    if (!process.waitForStarted(10000))
    {
        return ConversionResult::failure(QStringLiteral("ProcessStartFailed"), QStringLiteral("Failed to start LibreOffice worker process."));
    }

    QElapsedTimer timer;
    timer.start();
    const int timeoutMs = 120000; // 2 minutes max

    while (!process.waitForFinished(500))
    {
        if (cancelToken && cancelToken->load())
        {
            process.kill();
            process.waitForFinished(3000);
            return ConversionResult::cancelled();
        }

        if (timer.elapsed() > timeoutMs)
        {
            process.kill();
            process.waitForFinished(3000);
            return ConversionResult::failure(QStringLiteral("Timeout"), QStringLiteral("LibreOffice conversion timed out after 2 minutes."));
        }

        if (progress)
        {
            int p = 30 + static_cast<int>((timer.elapsed() * 40) / timeoutMs);
            if (p > 70) p = 70;
            progress(ConversionStage::Rendering, p, QStringLiteral("Converting document..."));
        }
    }

    if (process.exitStatus() != QProcess::NormalExit || process.exitCode() != 0)
    {
        const QString errStr = QString::fromUtf8(process.readAllStandardError());
        return ConversionResult::failure(QStringLiteral("ConversionFailed"),
                                         QStringLiteral("LibreOffice failed with exit code %1: %2").arg(process.exitCode()).arg(errStr.trimmed()));
    }

    if (progress) progress(ConversionStage::Validating, 80, QStringLiteral("Validating generated PDF..."));

    QFileInfo srcInfo(request.sourcePath);
    const QString expectedPdfName = srcInfo.completeBaseName() + QStringLiteral(".pdf");
    const QString generatedPdfPath = QDir(outDir).filePath(expectedPdfName);

    if (!QFile::exists(generatedPdfPath))
    {
        return ConversionResult::failure(QStringLiteral("OutputNotFound"), QStringLiteral("LibreOffice completed but output PDF was not found."));
    }

    QString verifyError;
    if (!OutputVerification::verifyPdf(generatedPdfPath, 0, &verifyError))
    {
        return ConversionResult::failure(QStringLiteral("ValidationFailed"), QStringLiteral("Generated PDF validation failed: %1").arg(verifyError));
    }

    if (progress) progress(ConversionStage::Committing, 95, QStringLiteral("Committing output file..."));

    QString commitError;
    if (!TempFileGuard::atomicCommit(generatedPdfPath, request.outputPath, &commitError))
    {
        return ConversionResult::failure(QStringLiteral("CommitFailed"), QStringLiteral("Failed to commit final PDF: %1").arg(commitError));
    }

    if (progress) progress(ConversionStage::Completed, 100, QStringLiteral("Conversion complete."));

    ConversionResult result = ConversionResult::success(request.outputPath);
    result.totalPagesProcessed = 1;
    result.elapsedMilliseconds = timer.elapsed();
    result.outputSizeBytes = QFileInfo(request.outputPath).size();
    return result;
}

} // namespace vectorpdf::conversion
