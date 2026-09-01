// MIT License
#ifndef VECTORPDF_READINGORDERMODEL_H
#define VECTORPDF_READINGORDERMODEL_H

#include "../accessibilityglobal.h"
#include <QList>
#include <QRectF>
#include <QString>

namespace vectorpdf::a11y
{

struct ReadingBlock
{
    int order = 1;
    int pageIndex = 0;
    QRectF bounds;
    QString textSnippet;
    QString tagTypeName = QStringLiteral("P");
};

class VECTORPDF_ACCESSIBILITY_EXPORT ReadingOrderModel
{
public:
    void setBlocks(const QList<ReadingBlock>& blocks);
    QList<ReadingBlock> blocksForPage(int pageIndex) const;
    QList<ReadingBlock> allBlocks() const { return m_blocks; }

    bool reorderBlock(int fromOrder, int toOrder, int pageIndex);

private:
    QList<ReadingBlock> m_blocks;
};

} // namespace vectorpdf::a11y

#endif // VECTORPDF_READINGORDERMODEL_H
