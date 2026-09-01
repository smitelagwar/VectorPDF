// MIT License
// Copyright (c) 2026 VectorPDF Contributors
#ifndef VECTORPDF_FINDREPLACEQUERY_H
#define VECTORPDF_FINDREPLACEQUERY_H

#include "../documenttoolsglobal.h"
#include <QString>

namespace vectorpdf::tools::content
{

struct VECTORPDF_DOCUMENTTOOLS_EXPORT FindReplaceQuery
{
    QString pattern;
    QString replacement;
    bool caseSensitive = false;
    bool wholeWord = false;
    bool regex = false;
    bool preserveCase = true;
    QString pageRange; // Empty means all pages
};

} // namespace vectorpdf::tools::content

#endif // VECTORPDF_FINDREPLACEQUERY_H
