// MIT License
// Copyright (c) 2026 VectorPDF Contributors
#ifndef VECTORPDF_PAGEDECORATIONDEFINITION_H
#define VECTORPDF_PAGEDECORATIONDEFINITION_H

#include "../documenttoolsglobal.h"
#include "../documenttooltypes.h"
#include <QString>
#include <QUuid>
#include <QColor>
#include <QFont>
#include <QMarginsF>
#include <QList>

namespace vectorpdf::tools::decorations
{

enum class DecorationType
{
    HeaderFooter,
    Watermark,
    Background
};

enum class PageRangeMode
{
    AllPages,
    OddPagesOnly,
    EvenPagesOnly,
    CustomRange
};

struct VECTORPDF_DOCUMENTTOOLS_EXPORT PageDecorationTargetPages
{
    PageRangeMode rangeMode = PageRangeMode::AllPages;
    QString customRangeString;
    bool excludeFirstPage = false;
    int startPageNumber = 1;
    int pageNumberOffset = 0;

    QList<int> resolvePages(int totalPages) const;
};

class VECTORPDF_DOCUMENTTOOLS_EXPORT PageDecorationDefinition
{
public:
    explicit PageDecorationDefinition(DecorationType type);
    virtual ~PageDecorationDefinition() = default;

    DecorationType decorationType() const { return m_type; }
    QUuid id() const { return m_id; }
    void setId(const QUuid& id) { m_id = id; }

    QString name() const { return m_name; }
    void setName(const QString& name) { m_name = name; }

    PageDecorationTargetPages targetPages() const { return m_targetPages; }
    void setTargetPages(const PageDecorationTargetPages& pages) { m_targetPages = pages; }

    qreal opacity() const { return m_opacity; }
    void setOpacity(qreal opacity) { m_opacity = qBound(0.0, opacity, 1.0); }

    bool isBehindContent() const { return m_behindContent; }
    void setBehindContent(bool behind) { m_behindContent = behind; }

private:
    DecorationType m_type;
    QUuid m_id;
    QString m_name;
    PageDecorationTargetPages m_targetPages;
    qreal m_opacity = 1.0;
    bool m_behindContent = false;
};

} // namespace vectorpdf::tools::decorations

#endif // VECTORPDF_PAGEDECORATIONDEFINITION_H
