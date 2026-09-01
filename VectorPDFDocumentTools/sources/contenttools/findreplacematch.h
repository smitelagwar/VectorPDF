// MIT License
// Copyright (c) 2026 VectorPDF Contributors
#ifndef VECTORPDF_FINDREPLACEMATCH_H
#define VECTORPDF_FINDREPLACEMATCH_H

#include "../documenttoolsglobal.h"
#include <QString>
#include <QRectF>

namespace vectorpdf::tools::content
{

enum class ReplaceOverflowStatus
{
    Fits,
    MayOverflow,
    WillOverflow,
    UnsupportedEncoding
};

struct VECTORPDF_DOCUMENTTOOLS_EXPORT FindReplaceMatch
{
    int matchIndex = 0;
    int pageIndex = 0;
    QRectF boundingRect;
    QString matchedText;
    QString proposedReplacement;
    ReplaceOverflowStatus overflowStatus = ReplaceOverflowStatus::Fits;
    bool isOcrInvisibleLayer = false;
    bool selected = true;
};

} // namespace vectorpdf::tools::content

#endif // VECTORPDF_FINDREPLACEMATCH_H
