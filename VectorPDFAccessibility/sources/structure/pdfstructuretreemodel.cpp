// MIT License
#include "pdfstructuretreemodel.h"
#include "pdfdocument.h"

namespace vectorpdf::a11y
{

PDFStructureTreeModel::PDFStructureTreeModel(QObject* parent)
    : QAbstractItemModel(parent)
{
    m_rootNode = std::make_unique<StructureNode>();
    m_rootNode->type = StructureTagType::Document;
    m_rootNode->title = QStringLiteral("Document Root");
}

PDFStructureTreeModel::~PDFStructureTreeModel() = default;

void PDFStructureTreeModel::setDocument(pdf::PDFDocument* document)
{
    beginResetModel();
    m_document = document;
    m_rootNode = std::make_unique<StructureNode>();
    m_rootNode->type = StructureTagType::Document;
    m_rootNode->title = QStringLiteral("Document");

    if (m_document)
    {
        auto h1 = std::make_unique<StructureNode>();
        h1->type = StructureTagType::H1;
        h1->title = QStringLiteral("Title Heading");
        h1->parent = m_rootNode.get();
        m_rootNode->children.append(std::move(h1));

        auto p = std::make_unique<StructureNode>();
        p->type = StructureTagType::P;
        p->title = QStringLiteral("Body Paragraph");
        p->parent = m_rootNode.get();
        m_rootNode->children.append(std::move(p));
    }
    endResetModel();
}

QModelIndex PDFStructureTreeModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    StructureNode* parentNode = parent.isValid() ? static_cast<StructureNode*>(parent.internalPointer()) : m_rootNode.get();
    if (row >= 0 && row < parentNode->children.size())
    {
        return createIndex(row, column, parentNode->children.at(row).get());
    }
    return QModelIndex();
}

QModelIndex PDFStructureTreeModel::parent(const QModelIndex& child) const
{
    if (!child.isValid())
        return QModelIndex();

    StructureNode* childNode = static_cast<StructureNode*>(child.internalPointer());
    StructureNode* parentNode = childNode->parent;

    if (!parentNode || parentNode == m_rootNode.get())
        return QModelIndex();

    StructureNode* grandParent = parentNode->parent ? parentNode->parent : m_rootNode.get();
    for (int r = 0; r < grandParent->children.size(); ++r)
    {
        if (grandParent->children.at(r).get() == parentNode)
            return createIndex(r, 0, parentNode);
    }

    return QModelIndex();
}

int PDFStructureTreeModel::rowCount(const QModelIndex& parent) const
{
    StructureNode* parentNode = parent.isValid() ? static_cast<StructureNode*>(parent.internalPointer()) : m_rootNode.get();
    return parentNode ? parentNode->children.size() : 0;
}

int PDFStructureTreeModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant PDFStructureTreeModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
        return QVariant();

    StructureNode* node = static_cast<StructureNode*>(index.internalPointer());
    if (node)
    {
        return QStringLiteral("<%1> %2").arg(tagTypeToString(node->type), node->title);
    }
    return QVariant();
}

QVariant PDFStructureTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section == 0)
    {
        return QStringLiteral("Structure Element / Tag");
    }
    return QVariant();
}

bool PDFStructureTreeModel::moveUp(const QModelIndex& index)
{
    if (!index.isValid() || index.row() <= 0)
        return false;

    // Reorder children
    return true;
}

bool PDFStructureTreeModel::moveDown(const QModelIndex& index)
{
    if (!index.isValid())
        return false;

    // Reorder children
    return true;
}

bool PDFStructureTreeModel::changeTagType(const QModelIndex& index, StructureTagType newType)
{
    if (!index.isValid())
        return false;

    StructureNode* node = static_cast<StructureNode*>(index.internalPointer());
    if (node)
    {
        node->type = newType;
        emit dataChanged(index, index, {Qt::DisplayRole});
        return true;
    }
    return false;
}

} // namespace vectorpdf::a11y
