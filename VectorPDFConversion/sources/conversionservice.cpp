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

#include "conversionservice.h"
#include <QMutexLocker>
#include <QThread>

namespace vectorpdf::conversion
{

ConversionService& ConversionService::instance()
{
    static ConversionService s_instance;
    return s_instance;
}

ConversionService::ConversionService(QObject* parent)
    : QObject(parent)
{
    m_threadPool.setMaxThreadCount(qMax(2, QThread::idealThreadCount() - 1));
}

ConversionJob* ConversionService::enqueue(const ConversionRequest& request)
{
    ConversionJob* job = new ConversionJob(request, this);

    {
        QMutexLocker locker(&m_mutex);
        m_activeJobs[request.jobId] = job;
    }

    connect(job, &ConversionJob::stageChanged, this, [this, job](ConversionStage stage, const QString& message) {
        if (stage == ConversionStage::Preparing || stage == ConversionStage::Loading)
        {
            emit jobStarted(job);
        }
        emit jobProgress(job, job->progressPercentage(), message);
    });

    connect(job, &ConversionJob::progressChanged, this, [this, job](int percentage, const QString& message) {
        emit jobProgress(job, percentage, message);
    });

    connect(job, &ConversionJob::finished, this, [this, job](const ConversionResult& result) {
        {
            QMutexLocker locker(&m_mutex);
            m_activeJobs.remove(job->request().jobId);

            ConversionJobSummary summary;
            summary.jobId = job->request().jobId;
            summary.format = job->request().format;
            summary.status = result.status;
            summary.sourcePath = job->request().sourcePath;
            summary.outputPath = result.outputPath;
            summary.elapsedMs = result.elapsedMilliseconds;
            summary.outputSizeBytes = result.outputSizeBytes;

            m_history.append(summary);
            // Cap history to 500 items
            if (m_history.size() > 500)
            {
                m_history.removeFirst();
            }
        }

        emit jobFinished(job, result);
    });

    emit jobEnqueued(job);

    m_threadPool.start(job);
    return job;
}

ConversionResult ConversionService::executeSync(const ConversionRequest& request)
{
    ConversionJob job(request);
    job.run();
    return job.result();
}

void ConversionService::cancelJob(const QString& jobId)
{
    QMutexLocker locker(&m_mutex);
    ConversionJob* job = m_activeJobs.value(jobId, nullptr);
    if (job)
    {
        job->cancel();
    }
}

void ConversionService::cancelAll()
{
    QMutexLocker locker(&m_mutex);
    for (ConversionJob* job : m_activeJobs)
    {
        if (job)
        {
            job->cancel();
        }
    }
}

QList<ConversionJob*> ConversionService::getActiveJobs() const
{
    QMutexLocker locker(&m_mutex);
    return m_activeJobs.values();
}

ConversionJob* ConversionService::getJob(const QString& jobId) const
{
    QMutexLocker locker(&m_mutex);
    return m_activeJobs.value(jobId, nullptr);
}

QList<ConversionJobSummary> ConversionService::getJobHistory() const
{
    QMutexLocker locker(&m_mutex);
    return m_history;
}

} // namespace vectorpdf::conversion
