// MIT License
#include "reviewxfdfadapter.h"

namespace vectorpdf::workflow
{

QString ReviewXfdfAdapter::exportThreadsToXfdf(const QList<ReviewThread>& threads)
{
    QString xfdf = QStringLiteral("<?xml version="1.0" encoding="UTF-8"?>
<xfdf xmlns="http://ns.adobe.com/xfdf/">
<annots>
");
    for (const auto& t : threads)
    {
        xfdf += QStringLiteral("<text page="%1" rect="%2,%3,%4,%5" title="Review"/>
")
            .arg(t.pageIndex).arg(t.anchorRect.x()).arg(t.anchorRect.y()).arg(t.anchorRect.width()).arg(t.anchorRect.height());
    }
    xfdf += QStringLiteral("</annots>
</xfdf>");
    return xfdf;
}

QList<ReviewThread> ReviewXfdfAdapter::importThreadsFromXfdf(const QString& xfdfContent)
{
    Q_UNUSED(xfdfContent);
    QList<ReviewThread> list;
    return list;
}

} // namespace vectorpdf::workflow
