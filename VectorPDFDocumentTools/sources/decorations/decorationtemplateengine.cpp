// MIT License
// Copyright (c) 2026 VectorPDF Contributors
#include "decorationtemplateengine.h"
#include <QRegularExpression>

namespace vectorpdf::tools::decorations
{

QString DecorationTemplateEngine::evaluate(const QString& templateString, const TemplateContext& context)
{
    QString result = templateString;
    int displayPageNum = context.pageIndex + context.startPageNumber + context.pageNumberOffset;

    result.replace(QStringLiteral("{page}"), QString::number(displayPageNum));

    QRegularExpression padRe(QStringLiteral(R"(\{page:(0+)\})"));
    QRegularExpressionMatch match = padRe.match(result);
    while (match.hasMatch())
    {
        int digits = match.captured(1).length();
        QString padded = QString("%1").arg(displayPageNum, digits, 10, QChar('0'));
        result.replace(match.captured(0), padded);
        match = padRe.match(result);
    }

    result.replace(QStringLiteral("{pages}"), QString::number(context.totalPages));
    result.replace(QStringLiteral("{date}"), context.currentDateTime.toString(QStringLiteral("dd.MM.yyyy")));
    result.replace(QStringLiteral("{time}"), context.currentDateTime.toString(QStringLiteral("hh:mm")));
    result.replace(QStringLiteral("{filename}"), context.filename);
    result.replace(QStringLiteral("{title}"), context.documentTitle);
    result.replace(QStringLiteral("{author}"), context.author);
    result.replace(QStringLiteral("{subject}"), context.subject);

    return result;
}

} // namespace vectorpdf::tools::decorations
