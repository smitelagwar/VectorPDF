// MIT License
// Copyright (c) 2026 VectorPDF Contributors
#ifndef VECTORPDF_DECORATIONTEMPLATEENGINE_H
#define VECTORPDF_DECORATIONTEMPLATEENGINE_H

#include "../documenttoolsglobal.h"
#include <QString>
#include <QDateTime>

namespace vectorpdf::tools::decorations
{

struct VECTORPDF_DOCUMENTTOOLS_EXPORT TemplateContext
{
    int pageIndex = 0;
    int totalPages = 1;
    int startPageNumber = 1;
    int pageNumberOffset = 0;
    QString filename;
    QString documentTitle;
    QString author;
    QString subject;
    QDateTime currentDateTime = QDateTime::currentDateTime();
};

class VECTORPDF_DOCUMENTTOOLS_EXPORT DecorationTemplateEngine
{
public:
    static QString evaluate(const QString& templateString, const TemplateContext& context);
};

} // namespace vectorpdf::tools::decorations

#endif // VECTORPDF_DECORATIONTEMPLATEENGINE_H
