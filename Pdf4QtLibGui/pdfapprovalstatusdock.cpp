// MIT License
#include "pdfapprovalstatusdock.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QTreeWidget>
#include <QLabel>

PDFApprovalStatusDock::PDFApprovalStatusDock(QWidget* parent)
    : QDockWidget(tr("Onay Akış Durumu & Denetim İzi (Audit Trail)"), parent)
{
    initUi();
}

PDFApprovalStatusDock::~PDFApprovalStatusDock() = default;

void PDFApprovalStatusDock::setDocument(pdf::PDFDocument* document)
{
    m_document = document;
}

void PDFApprovalStatusDock::initUi()
{
    auto container = new QWidget(this);
    auto layout = new QVBoxLayout(container);

    m_statusLabel = new QLabel(tr("Onay Durumu: Adım 2/3 (Finans Direktörü Bekleniyor)"), this);
    layout->addWidget(m_statusLabel);

    m_stepsTree = new QTreeWidget(this);
    m_stepsTree->setHeaderLabels({tr("Adım / Yetkili"), tr("Karar"), tr("Tarih")});
    auto item1 = new QTreeWidgetItem(m_stepsTree, {tr("1. Hukuk Müşaviri"), tr("ONAYLANDI"), tr("01.09.2026 09:30")});
    item1->setForeground(1, QBrush(QColor(0, 150, 0)));
    auto item2 = new QTreeWidgetItem(m_stepsTree, {tr("2. Finans Direktörü"), tr("BEKLİYOR"), tr("-")});
    item2->setForeground(1, QBrush(QColor(180, 100, 0)));
    layout->addWidget(m_stepsTree);

    setWidget(container);
}
