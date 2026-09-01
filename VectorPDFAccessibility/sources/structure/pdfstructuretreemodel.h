// MIT License
#ifndef VECTORPDF_PDFSTRUCTURETREEMODEL_H
#define VECTORPDF_PDFSTRUCTURETREEMODEL_H

#include "../accessibilityglobal.h"
#include "../accessibilitytypes.h"
#include <QAbstractItemModel>
#include <memory>

namespace pdf { class PDFDocument; }

namespace vectorpdf::a11y
{

struct StructureNode
{
    StructureTagType type = StructureTagType::Document;
    QString title;
    QString actualText;
    QString altText;
    QString language;
    int pageIndex = 0;
    int mcid = -1;
    StructureNode* parent = nullptr;
    QList<std::unique_ptr<StructureNode>> children;
};

class VECTORPDF_ACCESSIBILITY_EXPORT PDFStructureTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit PDFStructureTreeModel(QObject* parent = nullptr);
    ~PDFStructureTreeModel() override;

    void setDocument(pdf::PDFDocument* document);

    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& child) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    bool moveUp(const QModelIndex& index);
    bool moveDown(const QModelIndex& index);
    bool changeTagType(const QModelIndex& index, StructureTagType newType);

private:
    std::unique_ptr<StructureNode> m_rootNode;
    pdf::PDFDocument* m_document = nullptr;
};

} // namespace vectorpdf::a11y

#endif // VECTORPDF_PDFSTRUCTURETREEMODEL_H
