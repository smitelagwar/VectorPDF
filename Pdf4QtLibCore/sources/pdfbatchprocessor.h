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

#ifndef PDFBATCHPROCESSOR_H
#define PDFBATCHPROCESSOR_H

#include "pdfglobal.h"

#include <QObject>
#include <QString>
#include <QVariantMap>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonArray>
#include <vector>
#include <memory>

namespace pdf
{

enum class PDFBatchJobType
{
    Convert,
    OCR,
    Optimize,
    Sanitize,
    ExtractPages,
    ExportImages,
    RenderToImage,
    ValidatePDFA
};

enum class PDFBatchJobStatus
{
    Pending,
    Running,
    Succeeded,
    Failed,
    Cancelled
};

struct PDF4QTLIBCORESHARED_EXPORT PDFBatchJob
{
    QString jobId;
    PDFBatchJobType type = PDFBatchJobType::Convert;
    QString inputFilePath;
    QString outputFilePath;
    QVariantMap parameters;
    PDFBatchJobStatus status = PDFBatchJobStatus::Pending;
    int progressPercentage = 0;
    int retryCount = 0;
    int maxRetries = 3;
    QString errorMessage;
    QDateTime startTime;
    QDateTime endTime;

    QJsonObject toJson() const;
    static PDFBatchJob fromJson(const QJsonObject& json);
};

class PDF4QTLIBCORESHARED_EXPORT PDFBatchProcessor : public QObject
{
    Q_OBJECT

public:
    explicit PDFBatchProcessor(QObject* parent = nullptr);
    ~PDFBatchProcessor() override;

    void addJob(const PDFBatchJob& job);
    void addJobs(const std::vector<PDFBatchJob>& jobs);
    bool cancelJob(const QString& jobId);
    bool retryJob(const QString& jobId);
    void clearJobs();

    const std::vector<PDFBatchJob>& getJobs() const;
    PDFBatchJob getJob(const QString& jobId) const;
    
    int getPendingCount() const;
    int getCompletedCount() const;
    int getFailedCount() const;

    void startProcessing();
    void stopProcessing();
    bool isProcessing() const;

    QJsonObject generateReportJson() const;

signals:
    void jobStarted(const QString& jobId);
    void jobProgress(const QString& jobId, int percentage);
    void jobFinished(const QString& jobId, bool success, const QString& errorMessage);
    void allJobsCompleted(int totalSuccess, int totalFailed);

private:
    void processNextJob();

    std::vector<PDFBatchJob> m_jobs;
    bool m_isProcessing;
    int m_currentJobIndex;
};

} // namespace pdf

#endif // PDFBATCHPROCESSOR_H
