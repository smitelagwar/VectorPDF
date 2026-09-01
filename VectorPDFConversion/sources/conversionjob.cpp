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

#include "conversionjob.h"
#include "conversionregistry.h"
#include "utilities/mimedetector.h"
#include <QFileInfo>
#include <QDir>

namespace vectorpdf::conversion
{

ConversionJob::ConversionJob(const ConversionRequest& request, QObject* parent)
    : QObject(parent)
    , m_request(request)
{
    setAutoDelete(false);
}

const ConversionRequest& ConversionJob::request() const
{
    return m_request;
}

ConversionResult ConversionJob::result() const
{
    return m_result;
}

ConversionStatus ConversionJob::status() const
{
    return m_status.load();
}

ConversionStage ConversionJob::stage() const
{
    return m_stage.load();
}

int ConversionJob::progressPercentage() const
{
    return m_progress.load();
}

QString ConversionJob::currentMessage() const
{
    return m_currentMessage;
}

void ConversionJob::cancel()
{
    m_cancelRequested.store(true);
}

bool ConversionJob::isCancelled() const
{
    return m_cancelRequested.load();
}

void ConversionJob::run()
{
    m_status.store(ConversionStatus::Running);
    m_stage.store(ConversionStage::Preparing);
    emit stageChanged(ConversionStage::Preparing, QStringLiteral("Starting conversion job..."));

    std::shared_ptr<IConversionWorker> worker;

    if (m_request.format != ConversionFormat::Pdf)
    {
        // Export from PDF to another format
        worker = ConversionRegistry::instance().getExporter(m_request.format);
    }
    else
    {
        // Create PDF from input
        if (QFileInfo(m_request.sourcePath).isDir())
        {
            worker = ConversionRegistry::instance().getCreator(QStringLiteral("folder"));
        }
        else if (!m_request.sourcePaths.isEmpty())
        {
            worker = ConversionRegistry::instance().getCreator(QStringLiteral("png")); // Image creator handles multi-image
        }
        else
        {
            const QString ext = QFileInfo(m_request.sourcePath).suffix().trimmed().toLower();
            worker = ConversionRegistry::instance().getCreator(ext);
        }
    }

    if (!worker)
    {
        m_result = ConversionResult::unsupported(QStringLiteral("No suitable converter found for requested operation."));
        m_status.store(ConversionStatus::Unsupported);
        m_stage.store(ConversionStage::Failed);
        emit finished(m_result);
        return;
    }

    auto progressCallback = [this](ConversionStage stage, int percentage, const QString& message) {
        m_stage.store(stage);
        m_progress.store(percentage);
        m_currentMessage = message;
        emit stageChanged(stage, message);
        emit progressChanged(percentage, message);
    };

    m_result = worker->execute(m_request, progressCallback, &m_cancelRequested);
    m_result.jobId = m_request.jobId;

    m_status.store(m_result.status);
    m_stage.store(m_result.isSuccess() ? ConversionStage::Completed : ConversionStage::Failed);

    emit finished(m_result);
}

} // namespace vectorpdf::conversion
