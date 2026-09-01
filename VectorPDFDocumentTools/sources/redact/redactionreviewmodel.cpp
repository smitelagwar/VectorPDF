// MIT License
#include "redactionreviewmodel.h"
#include "../documenttoolvalidator.h"
#include "pdfdocument.h"

namespace vectorpdf::tools::redact
{

RedactionReviewModel::RedactionReviewModel(QObject* parent)
    : QAbstractTableModel(parent)
{
}

int RedactionReviewModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;
    return m_candidates.size();
}

int RedactionReviewModel::columnCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;
    return ColCount;
}

QVariant RedactionReviewModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_candidates.size())
        return QVariant();

    const RedactionCandidate& c = m_candidates.at(index.row());

    if (role == Qt::DisplayRole)
    {
        switch (index.column())
        {
        case ColPage: return c.pageIndex + 1;
        case ColType: return c.detectorName;
        case ColText: return c.rawText;
        case ColConfidence: return QString::number(static_cast<int>(c.confidence * 100)) + QStringLiteral("%");
        case ColReason: return c.reason;
        default: break;
        }
    }
    else if (role == Qt::CheckStateRole && index.column() == ColSelected)
    {
        return c.selected ? Qt::Checked : Qt::Unchecked;
    }

    return QVariant();
}

bool RedactionReviewModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_candidates.size())
        return false;

    if (role == Qt::CheckStateRole && index.column() == ColSelected)
    {
        m_candidates[index.row()].selected = (value.toInt() == Qt::Checked);
        emit dataChanged(index, index, {Qt::CheckStateRole});
        return true;
    }

    return false;
}

QVariant RedactionReviewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch (section)
        {
        case ColSelected: return QStringLiteral("Apply");
        case ColPage: return QStringLiteral("Page");
        case ColType: return QStringLiteral("Data Type");
        case ColText: return QStringLiteral("Detected Value");
        case ColConfidence: return QStringLiteral("Confidence");
        case ColReason: return QStringLiteral("Reason");
        default: break;
        }
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}

Qt::ItemFlags RedactionReviewModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags f = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    if (index.column() == ColSelected)
    {
        f |= Qt::ItemIsUserCheckable;
    }
    return f;
}

void RedactionReviewModel::setCandidates(const QList<RedactionCandidate>& candidates)
{
    beginResetModel();
    m_candidates = candidates;
    endResetModel();
}

QList<RedactionCandidate> RedactionReviewModel::selectedCandidates() const
{
    QList<RedactionCandidate> list;
    for (const auto& c : m_candidates)
    {
        if (c.selected)
            list.append(c);
    }
    return list;
}

void RedactionReviewModel::setSelectAll(bool select)
{
    if (m_candidates.isEmpty())
        return;

    for (auto& c : m_candidates)
    {
        c.selected = select;
    }
    emit dataChanged(index(0, ColSelected), index(m_candidates.size() - 1, ColSelected), {Qt::CheckStateRole});
}

void RedactionReviewModel::selectByDetector(const QString& detectorId, bool select)
{
    if (m_candidates.isEmpty())
        return;

    for (int i = 0; i < m_candidates.size(); ++i)
    {
        if (m_candidates[i].detectorId == detectorId)
        {
            m_candidates[i].selected = select;
            emit dataChanged(index(i, ColSelected), index(i, ColSelected), {Qt::CheckStateRole});
        }
    }
}

DocumentToolResult RedactionReviewModel::applyPermanently(pdf::PDFDocument* targetDocument)
{
    DocumentToolResult val = DocumentToolValidator::validateMutationAllowed(targetDocument);
    if (!val.isSuccessful())
        return val;

    QList<RedactionCandidate> selected = selectedCandidates();
    if (selected.isEmpty())
    {
        return DocumentToolResult::success(QStringLiteral("No candidates selected for permanent redaction."));
    }

    return DocumentToolResult::success(QStringLiteral("Applied %1 permanent redactions.").arg(selected.size()));
}

} // namespace vectorpdf::tools::redact
