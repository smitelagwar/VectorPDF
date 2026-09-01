// MIT License
// Copyright (c) 2026 VectorPDF Contributors
#ifndef VECTORPDF_FINDREPLACEANALYZER_H
#define VECTORPDF_FINDREPLACEANALYZER_H

#include "../documenttoolsglobal.h"
#include "findreplacequery.h"
#include "findreplacematch.h"
#include <QList>

namespace pdf
{
class PDFDocument;
}

namespace vectorpdf::tools::content
{

class VECTORPDF_DOCUMENTTOOLS_EXPORT FindReplaceAnalyzer
{
public:
    static QList<FindReplaceMatch> analyze(const pdf::PDFDocument* document, const FindReplaceQuery& query);
    static QString computeReplacementWithPreservedCase(const QString& originalText, const QString& replacementText);
    static bool isTurkishLocaleAwareMatch(const QString& source, const QString& query, bool caseSensitive);
};

} // namespace vectorpdf::tools::content

#endif // VECTORPDF_FINDREPLACEANALYZER_H
