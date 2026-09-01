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

#ifndef VECTORPDF_CONVERSIONJOB_H
#define VECTORPDF_CONVERSIONJOB_H

#include "conversionglobal.h"
#include "conversiontypes.h"
#include "conversionrequest.h"
#include "conversionresult.h"

#include <QObject>
#include <QRunnable>
#include <atomic>
#include <memory>

namespace vectorpdf::conversion
{

class IConversionWorker;

class VECTORPDF_CONVERSION_EXPORT ConversionJob : public QObject, public QRunnable
{
    Q_OBJECT

public:
    explicit ConversionJob(const ConversionRequest& request, QObject* parent = nullptr);
    virtual ~ConversionJob() override = default;

    const ConversionRequest& request() const;
    ConversionResult result() const;
    ConversionStatus status() const;
    ConversionStage stage() const;
    int progressPercentage() const;
    QString currentMessage() const;

    void cancel();
    bool isCancelled() const;

    virtual void run() override;

signals:
    void stageChanged(ConversionStage stage, const QString& message);
    void progressChanged(int percentage, const QString& message);
    void finished(const ConversionResult& result);

private:
    ConversionRequest m_request;
    ConversionResult m_result;
    std::atomic<ConversionStatus> m_status{ConversionStatus::Queued};
    std::atomic<ConversionStage> m_stage{ConversionStage::Idle};
    std::atomic<int> m_progress{0};
    std::atomic<bool> m_cancelRequested{false};
    QString m_currentMessage;
};

} // namespace vectorpdf::conversion

#endif // VECTORPDF_CONVERSIONJOB_H
