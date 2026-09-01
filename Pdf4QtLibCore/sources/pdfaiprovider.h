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

#ifndef PDFAIPROVIDER_H
#define PDFAIPROVIDER_H

#include "pdfglobal.h"

#include <QString>
#include <QStringList>
#include <QDateTime>
#include <QJsonObject>
#include <memory>
#include <functional>

namespace pdf
{

enum class AITaskType
{
    SummarizeDocument,
    ExtractKeyPhrases,
    SemanticSearch,
    SuggestTags,
    TableStructureExtraction
};

struct PDF4QTLIBCORESHARED_EXPORT AITaskConsent
{
    QString consentId;
    QString documentId;
    AITaskType taskType = AITaskType::SummarizeDocument;
    bool userConsentGranted = false;
    bool enforceOfflineExecution = true;
    QDateTime grantedAt;

    bool isValid() const;
};

struct PDF4QTLIBCORESHARED_EXPORT AIExecutionResult
{
    bool success = false;
    QString resultText;
    QStringList items;
    QString errorMessage;
    double confidence = 0.0;
    QString modelIdentifier;
    bool executedOffline = true;
};

class PDF4QTLIBCORESHARED_EXPORT IAIProvider
{
public:
    virtual ~IAIProvider() = default;

    virtual QString getProviderName() const = 0;
    virtual QString getProviderVersion() const = 0;
    virtual bool isOfflineOnly() const = 0;
    virtual bool isAvailable() const = 0;

    virtual AIExecutionResult summarizeText(const QString& text, const AITaskConsent& consent, int maxSentences = 5) = 0;
    virtual AIExecutionResult extractKeyPhrases(const QString& text, const AITaskConsent& consent, int maxPhrases = 10) = 0;
    virtual AIExecutionResult suggestTags(const QString& text, const AITaskConsent& consent) = 0;
};

class PDF4QTLIBCORESHARED_EXPORT PDFAIOfflineProvider : public IAIProvider
{
public:
    explicit PDFAIOfflineProvider();
    ~PDFAIOfflineProvider() override = default;

    QString getProviderName() const override;
    QString getProviderVersion() const override;
    bool isOfflineOnly() const override;
    bool isAvailable() const override;

    AIExecutionResult summarizeText(const QString& text, const AITaskConsent& consent, int maxSentences = 5) override;
    AIExecutionResult extractKeyPhrases(const QString& text, const AITaskConsent& consent, int maxPhrases = 10) override;
    AIExecutionResult suggestTags(const QString& text, const AITaskConsent& consent) override;

    static QString sanitizeInputPrompt(const QString& input);
};

} // namespace pdf

#endif // PDFAIPROVIDER_H
