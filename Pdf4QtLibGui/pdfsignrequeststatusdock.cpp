// MIT License
#include "pdfsignrequeststatusdock.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QTreeWidget>
#include <QLabel>

PDFSignRequestStatusDock::PDFSignRequestStatusDock(QWidget* parent)
    : QDockWidget(tr("İmza Takip Durumu (Sign Tracking)"), parent)
{
    initUi();
}

PDFSignRequestStatusDock::~PDFSignRequestStatusDock() = default;

void PDFSignRequestStatusDock::setDocument(pdf::PDFDocument* document)
{
    m_document = document;
}

void PDFSignRequestStatusDock::initUi()
{
    auto container = new QWidget(this);
    auto layout = new QVBoxLayout(container);

    m_progressLabel = new QLabel(tr("İmza Durumu: %50 Tamamlandı (1/2)"), this);
    m_progressLabel->setStyleSheet("font-weight: bold;");
    layout->addWidget(m_progressLabel);

    m_statusTree = new QTreeWidget(this);
    m_statusTree->setHeaderLabels({tr("İmzacı"), tr("Durum"), tr("Tarih")});
    auto item1 = new QTreeWidgetItem(m_statusTree, {tr("Ahmet Yılmaz"), tr("İMZALANDI"), tr("01.09.2026 10:15")});
    item1->setForeground(1, QBrush(QColor(0, 150, 0)));
    auto item2 = new QTreeWidgetItem(m_statusTree, {tr("Mehmet Demir"), tr("BEKLİYOR"), tr("-")});
    item2->setForeground(1, QBrush(QColor(180, 100, 0)));
    layout->addWidget(m_statusTree);

    setWidget(container);
}
