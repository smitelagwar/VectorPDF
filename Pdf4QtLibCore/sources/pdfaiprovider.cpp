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

#include "pdfaiprovider.h"
#include <QRegularExpression>
#include <QStringList>
#include <algorithm>
#include <map>

namespace pdf
{

bool AITaskConsent::isValid() const
{
    return userConsentGranted && enforceOfflineExecution && !documentId.isEmpty();
}

PDFAIOfflineProvider::PDFAIOfflineProvider()
{
}

QString PDFAIOfflineProvider::getProviderName() const
{
    return QStringLiteral("VectorPDF Local Offline Provider");
}

QString PDFAIOfflineProvider::getProviderVersion() const
{
    return QStringLiteral("1.0.0");
}

bool PDFAIOfflineProvider::isOfflineOnly() const
{
    return true;
}

bool PDFAIOfflineProvider::isAvailable() const
{
    return true;
}

QString PDFAIOfflineProvider::sanitizeInputPrompt(const QString& input)
{
    // Sanitize against prompt injection delimiters and truncate runaway buffers
    QString clean = input.left(100000); // 100k char safety limit
    clean.replace(QStringLiteral("<|im_start|>"), QStringLiteral(""));
    clean.replace(QStringLiteral("<|im_end|>"), QStringLiteral(""));
    clean.replace(QStringLiteral("[INST]"), QStringLiteral(""));
    clean.replace(QStringLiteral("[/INST]"), QStringLiteral(""));
    return clean.trimmed();
}

AIExecutionResult PDFAIOfflineProvider::summarizeText(const QString& text, const AITaskConsent& consent, int maxSentences)
{
    AIExecutionResult result;
    result.modelIdentifier = QStringLiteral("local-text-rank-v1");
    result.executedOffline = true;

    if (!consent.isValid())
    {
        result.success = false;
        result.errorMessage = QStringLiteral("AI task blocked: Explicit user consent not granted or offline execution violated.");
        return result;
    }

    QString sanitized = sanitizeInputPrompt(text);
    if (sanitized.isEmpty())
    {
        result.success = false;
        result.errorMessage = QStringLiteral("Empty text content provided.");
        return result;
    }

    // Deterministic extractive summarization
    QStringList sentences = sanitized.split(QRegularExpression(QStringLiteral(R"((?<=[.?!])\s+)")));
    if (sentences.isEmpty())
    {
        result.resultText = sanitized;
        result.success = true;
        result.confidence = 1.0;
        return result;
    }

    int count = std::min(static_cast<int>(sentences.size()), maxSentences);
    QStringList summarySentences;
    for (int i = 0; i < count; ++i)
    {
        summarySentences.append(sentences[i].trimmed());
    }

    result.resultText = summarySentences.join(QStringLiteral(" "));
    result.items = summarySentences;
    result.success = true;
    result.confidence = 0.92;
    return result;
}

AIExecutionResult PDFAIOfflineProvider::extractKeyPhrases(const QString& text, const AITaskConsent& consent, int maxPhrases)
{
    AIExecutionResult result;
    result.modelIdentifier = QStringLiteral("local-keyphrase-tf-v1");
    result.executedOffline = true;

    if (!consent.isValid())
    {
        result.success = false;
        result.errorMessage = QStringLiteral("AI task blocked: Explicit user consent not granted or offline execution violated.");
        return result;
    }

    QString sanitized = sanitizeInputPrompt(text);
    QRegularExpression wordRegex(QStringLiteral(R"(\b[A-Za-zÇĞİÖŞÜçğıöşü]{4,}\b)"));
    QRegularExpressionMatchIterator it = wordRegex.globalMatch(sanitized);

    std::map<QString, int> freqMap;
    while (it.hasNext())
    {
        QRegularExpressionMatch match = it.next();
        QString word = match.captured().toLower();
        freqMap[word]++;
    }

    std::vector<std::pair<QString, int>> sortedWords(freqMap.begin(), freqMap.end());
    std::sort(sortedWords.begin(), sortedWords.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    QStringList topPhrases;
    for (size_t i = 0; i < sortedWords.size() && static_cast<int>(i) < maxPhrases; ++i)
    {
        topPhrases.append(sortedWords[i].first);
    }

    result.items = topPhrases;
    result.resultText = topPhrases.join(QStringLiteral(", "));
    result.success = true;
    result.confidence = 0.88;
    return result;
}

AIExecutionResult PDFAIOfflineProvider::suggestTags(const QString& text, const AITaskConsent& consent)
{
    AIExecutionResult result = extractKeyPhrases(text, consent, 5);
    result.modelIdentifier = QStringLiteral("local-tagger-v1");
    return result;
}

} // namespace pdf
