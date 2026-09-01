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

#include "pdfbatchprocessor.h"
#include <QTimer>
#include <QUuid>

namespace pdf
{

static QString jobTypeToString(PDFBatchJobType type)
{
    switch (type)
    {
    case PDFBatchJobType::Convert: return "Convert";
    case PDFBatchJobType::OCR: return "OCR";
    case PDFBatchJobType::Optimize: return "Optimize";
    case PDFBatchJobType::Sanitize: return "Sanitize";
    case PDFBatchJobType::ExtractPages: return "ExtractPages";
    case PDFBatchJobType::ExportImages: return "ExportImages";
    case PDFBatchJobType::RenderToImage: return "RenderToImage";
    case PDFBatchJobType::ValidatePDFA: return "ValidatePDFA";
    }
    return "Unknown";
}

static PDFBatchJobType stringToJobType(const QString& str)
{
    if (str == "OCR") return PDFBatchJobType::OCR;
    if (str == "Optimize") return PDFBatchJobType::Optimize;
    if (str == "Sanitize") return PDFBatchJobType::Sanitize;
    if (str == "ExtractPages") return PDFBatchJobType::ExtractPages;
    if (str == "ExportImages") return PDFBatchJobType::ExportImages;
    if (str == "RenderToImage") return PDFBatchJobType::RenderToImage;
    if (str == "ValidatePDFA") return PDFBatchJobType::ValidatePDFA;
    return PDFBatchJobType::Convert;
}

static QString jobStatusToString(PDFBatchJobStatus status)
{
    switch (status)
    {
    case PDFBatchJobStatus::Pending: return "Pending";
    case PDFBatchJobStatus::Running: return "Running";
    case PDFBatchJobStatus::Succeeded: return "Succeeded";
    case PDFBatchJobStatus::Failed: return "Failed";
    case PDFBatchJobStatus::Cancelled: return "Cancelled";
    }
    return "Unknown";
}

QJsonObject PDFBatchJob::toJson() const
{
    QJsonObject json;
    json["jobId"] = jobId;
    json["type"] = jobTypeToString(type);
    json["inputFilePath"] = inputFilePath;
    json["outputFilePath"] = outputFilePath;
    json["status"] = jobStatusToString(status);
    json["progressPercentage"] = progressPercentage;
    json["retryCount"] = retryCount;
    json["maxRetries"] = maxRetries;
    json["errorMessage"] = errorMessage;
    if (startTime.isValid())
    {
        json["startTime"] = startTime.toString(Qt::ISODate);
    }
    if (endTime.isValid())
    {
        json["endTime"] = endTime.toString(Qt::ISODate);
    }
    return json;
}

PDFBatchJob PDFBatchJob::fromJson(const QJsonObject& json)
{
    PDFBatchJob job;
    job.jobId = json["jobId"].toString();
    job.type = stringToJobType(json["type"].toString());
    job.inputFilePath = json["inputFilePath"].toString();
    job.outputFilePath = json["outputFilePath"].toString();
    job.retryCount = json["retryCount"].toInt(0);
    job.maxRetries = json["maxRetries"].toInt(3);
    job.errorMessage = json["errorMessage"].toString();
    return job;
}

PDFBatchProcessor::PDFBatchProcessor(QObject* parent)
    : QObject(parent)
    , m_isProcessing(false)
    , m_currentJobIndex(-1)
{
}

PDFBatchProcessor::~PDFBatchProcessor()
{
    stopProcessing();
}

void PDFBatchProcessor::addJob(const PDFBatchJob& job)
{
    PDFBatchJob newJob = job;
    if (newJob.jobId.isEmpty())
    {
        newJob.jobId = QUuid::createUuid().toString(QUuid::WithoutBraces);
    }
    newJob.status = PDFBatchJobStatus::Pending;
    m_jobs.push_back(newJob);
}

void PDFBatchProcessor::addJobs(const std::vector<PDFBatchJob>& jobs)
{
    for (const auto& job : jobs)
    {
        addJob(job);
    }
}

bool PDFBatchProcessor::cancelJob(const QString& jobId)
{
    for (auto& job : m_jobs)
    {
        if (job.jobId == jobId)
        {
            if (job.status == PDFBatchJobStatus::Pending || job.status == PDFBatchJobStatus::Running)
            {
                job.status = PDFBatchJobStatus::Cancelled;
                job.endTime = QDateTime::currentDateTime();
                emit jobFinished(jobId, false, "Cancelled by user");
                return true;
            }
        }
    }
    return false;
}

bool PDFBatchProcessor::retryJob(const QString& jobId)
{
    for (auto& job : m_jobs)
    {
        if (job.jobId == jobId && (job.status == PDFBatchJobStatus::Failed || job.status == PDFBatchJobStatus::Cancelled))
        {
            job.status = PDFBatchJobStatus::Pending;
            job.progressPercentage = 0;
            job.errorMessage.clear();
            job.retryCount++;
            return true;
        }
    }
    return false;
}

void PDFBatchProcessor::clearJobs()
{
    stopProcessing();
    m_jobs.clear();
    m_currentJobIndex = -1;
}

const std::vector<PDFBatchJob>& PDFBatchProcessor::getJobs() const
{
    return m_jobs;
}

PDFBatchJob PDFBatchProcessor::getJob(const QString& jobId) const
{
    for (const auto& job : m_jobs)
    {
        if (job.jobId == jobId)
        {
            return job;
        }
    }
    return PDFBatchJob();
}

int PDFBatchProcessor::getPendingCount() const
{
    int count = 0;
    for (const auto& job : m_jobs)
    {
        if (job.status == PDFBatchJobStatus::Pending)
        {
            count++;
        }
    }
    return count;
}

int PDFBatchProcessor::getCompletedCount() const
{
    int count = 0;
    for (const auto& job : m_jobs)
    {
        if (job.status == PDFBatchJobStatus::Succeeded)
        {
            count++;
        }
    }
    return count;
}

int PDFBatchProcessor::getFailedCount() const
{
    int count = 0;
    for (const auto& job : m_jobs)
    {
        if (job.status == PDFBatchJobStatus::Failed)
        {
            count++;
        }
    }
    return count;
}

void PDFBatchProcessor::startProcessing()
{
    if (m_isProcessing)
    {
        return;
    }

    m_isProcessing = true;
    processNextJob();
}

void PDFBatchProcessor::stopProcessing()
{
    m_isProcessing = false;
}

bool PDFBatchProcessor::isProcessing() const
{
    return m_isProcessing;
}

void PDFBatchProcessor::processNextJob()
{
    if (!m_isProcessing)
    {
        return;
    }

    int nextIndex = -1;
    for (size_t i = 0; i < m_jobs.size(); ++i)
    {
        if (m_jobs[i].status == PDFBatchJobStatus::Pending)
        {
            nextIndex = static_cast<int>(i);
            break;
        }
    }

    if (nextIndex == -1)
    {
        m_isProcessing = false;
        m_currentJobIndex = -1;
        emit allJobsCompleted(getCompletedCount(), getFailedCount());
        return;
    }

    m_currentJobIndex = nextIndex;
    auto& job = m_jobs[m_currentJobIndex];
    job.status = PDFBatchJobStatus::Running;
    job.startTime = QDateTime::currentDateTime();
    emit jobStarted(job.jobId);
    emit jobProgress(job.jobId, 25);

    // Asynchronous completion simulation via timer
    QTimer::singleShot(50, this, [this, nextIndex]() {
        if (nextIndex < static_cast<int>(m_jobs.size()) && m_jobs[nextIndex].status == PDFBatchJobStatus::Running)
        {
            auto& currentJob = m_jobs[nextIndex];
            currentJob.progressPercentage = 100;
            currentJob.status = PDFBatchJobStatus::Succeeded;
            currentJob.endTime = QDateTime::currentDateTime();
            emit jobProgress(currentJob.jobId, 100);
            emit jobFinished(currentJob.jobId, true, QString());
        }

        if (m_isProcessing)
        {
            processNextJob();
        }
    });
}

QJsonObject PDFBatchProcessor::generateReportJson() const
{
    QJsonObject report;
    report["totalJobs"] = static_cast<int>(m_jobs.size());
    report["completedJobs"] = getCompletedCount();
    report["failedJobs"] = getFailedCount();
    report["pendingJobs"] = getPendingCount();
    report["generatedAt"] = QDateTime::currentDateTime().toString(Qt::ISODate);

    QJsonArray jobsArray;
    for (const auto& job : m_jobs)
    {
        jobsArray.append(job.toJson());
    }
    report["jobs"] = jobsArray;

    return report;
}

} // namespace pdf
