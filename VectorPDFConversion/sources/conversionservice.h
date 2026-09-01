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

#ifndef VECTORPDF_CONVERSIONSERVICE_H
#define VECTORPDF_CONVERSIONSERVICE_H

#include "conversionglobal.h"
#include "conversiontypes.h"
#include "conversionrequest.h"
#include "conversionresult.h"
#include "conversionjob.h"

#include <QObject>
#include <QThreadPool>
#include <QList>
#include <QMap>
#include <QMutex>

namespace vectorpdf::conversion
{

struct ConversionJobSummary
{
    QString jobId;
    ConversionFormat format = ConversionFormat::Pdf;
    ConversionStatus status = ConversionStatus::Queued;
    QString sourcePath;
    QString outputPath;
    qint64 elapsedMs = 0;
    qint64 outputSizeBytes = 0;
};

class VECTORPDF_CONVERSION_EXPORT ConversionService : public QObject
{
    Q_OBJECT

public:
    static ConversionService& instance();

    /// Enqueues a conversion request to run asynchronously on the background thread pool
    ConversionJob* enqueue(const ConversionRequest& request);

    /// Executes a conversion request synchronously on the current thread
    ConversionResult executeSync(const ConversionRequest& request);

    /// Cancels a running or queued job by ID
    void cancelJob(const QString& jobId);

    /// Cancels all active conversion jobs
    void cancelAll();

    /// Returns list of currently active (queued/running/cancelling) jobs
    QList<ConversionJob*> getActiveJobs() const;

    /// Returns job by ID, or nullptr if not found
    ConversionJob* getJob(const QString& jobId) const;

    /// Returns bounded history of completed jobs
    QList<ConversionJobSummary> getJobHistory() const;

signals:
    void jobEnqueued(ConversionJob* job);
    void jobStarted(ConversionJob* job);
    void jobProgress(ConversionJob* job, int percentage, const QString& message);
    void jobFinished(ConversionJob* job, const ConversionResult& result);

private:
    ConversionService(QObject* parent = nullptr);
    virtual ~ConversionService() override = default;

    QThreadPool m_threadPool;
    mutable QMutex m_mutex;
    QMap<QString, ConversionJob*> m_activeJobs;
    QList<ConversionJobSummary> m_history;
};

} // namespace vectorpdf::conversion

#endif // VECTORPDF_CONVERSIONSERVICE_H
