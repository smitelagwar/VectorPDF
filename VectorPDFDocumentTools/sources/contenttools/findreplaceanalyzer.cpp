// MIT License
// Copyright (c) 2026 VectorPDF Contributors
#include "findreplaceanalyzer.h"
#include "pdfdocument.h"
#include <QRegularExpression>
#include <QLocale>

namespace vectorpdf::tools::content
{

QString FindReplaceAnalyzer::computeReplacementWithPreservedCase(const QString& originalText, const QString& replacementText)
{
    if (originalText.isEmpty() || replacementText.isEmpty())
        return replacementText;

    QLocale trLocale(QLocale::Turkish);

    bool allUpper = (trLocale.toUpper(originalText) == originalText);
    bool allLower = (trLocale.toLower(originalText) == originalText);
    bool titleCase = originalText.at(0).isUpper() && (originalText.length() == 1 || originalText.mid(1) == trLocale.toLower(originalText.mid(1)));

    if (allUpper)
    {
        return trLocale.toUpper(replacementText);
    }
    else if (allLower)
    {
        return trLocale.toLower(replacementText);
    }
    else if (titleCase)
    {
        return trLocale.toUpper(replacementText.left(1)) + trLocale.toLower(replacementText.mid(1));
    }

    return replacementText;
}

bool FindReplaceAnalyzer::isTurkishLocaleAwareMatch(const QString& source, const QString& query, bool caseSensitive)
{
    if (caseSensitive)
    {
        return source.contains(query, Qt::CaseSensitive);
    }

    QLocale trLocale(QLocale::Turkish);
    return trLocale.toLower(source).contains(trLocale.toLower(query));
}

QList<FindReplaceMatch> FindReplaceAnalyzer::analyze(const pdf::PDFDocument* document, const FindReplaceQuery& query)
{
    QList<FindReplaceMatch> matches;
    if (!document || query.pattern.isEmpty())
        return matches;

    int totalPages = static_cast<int>(document->getPageCount());
    int matchCounter = 0;

    for (int p = 0; p < totalPages; ++p)
    {
        // Search simulation on page text
        QString pageText = QStringLiteral("Sample text on page ") + QString::number(p + 1);
        if (isTurkishLocaleAwareMatch(pageText, query.pattern, query.caseSensitive))
        {
            FindReplaceMatch m;
            m.matchIndex = matchCounter++;
            m.pageIndex = p;
            m.boundingRect = QRectF(72, 100, 200, 20);
            m.matchedText = query.pattern;
            m.proposedReplacement = query.preserveCase ? 
                computeReplacementWithPreservedCase(m.matchedText, query.replacement) : query.replacement;
            m.overflowStatus = (m.proposedReplacement.length() > m.matchedText.length() * 1.5) ? 
                ReplaceOverflowStatus::MayOverflow : ReplaceOverflowStatus::Fits;
            m.selected = true;
            matches.append(m);
        }
    }

    return matches;
}

} // namespace vectorpdf::tools::content
