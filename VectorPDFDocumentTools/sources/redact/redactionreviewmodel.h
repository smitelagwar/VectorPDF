// MIT License
#ifndef VECTORPDF_REDACTIONREVIEWMODEL_H
#define VECTORPDF_REDACTIONREVIEWMODEL_H

#include "../documenttoolsglobal.h"
#include "../documenttoolresult.h"
#include "redactioncandidate.h"
#include <QAbstractTableModel>
#include <QList>

namespace pdf
{
class PDFDocument;
}

namespace vectorpdf::tools::redact
{

class VECTORPDF_DOCUMENTTOOLS_EXPORT RedactionReviewModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum Column
    {
        ColSelected = 0,
        ColPage,
        ColType,
        ColText,
        ColConfidence,
        ColReason,
        ColCount
    };

    explicit RedactionReviewModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    void setCandidates(const QList<RedactionCandidate>& candidates);
    QList<RedactionCandidate> candidates() const { return m_candidates; }
    QList<RedactionCandidate> selectedCandidates() const;

    void setSelectAll(bool select);
    void selectByDetector(const QString& detectorId, bool select);

    DocumentToolResult applyPermanently(pdf::PDFDocument* targetDocument);

private:
    QList<RedactionCandidate> m_candidates;
};

} // namespace vectorpdf::tools::redact

#endif // VECTORPDF_REDACTIONREVIEWMODEL_H
