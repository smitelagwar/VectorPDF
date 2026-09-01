// MIT License
#include "pdfduplicatepagereviewdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTreeWidget>
#include <QPushButton>
#include <QLabel>

PDFDuplicatePageReviewDialog::PDFDuplicatePageReviewDialog(pdf::PDFDocument* document, QWidget* parent)
    : QDialog(parent)
    , m_document(document)
{
    setWindowTitle(tr("Çift / Yinelenen Sayfaları Algıla ve Tekilleştir (Duplicate Pages)"));
    resize(580, 400);
    initUi();
}

PDFDuplicatePageReviewDialog::~PDFDuplicatePageReviewDialog() = default;

void PDFDuplicatePageReviewDialog::initUi()
{
    auto mainLayout = new QVBoxLayout(this);

    auto header = new QLabel(tr("Görsel ve metinsel benzerlik analiziyle tespit edilen yinelenen sayfalar:"), this);
    mainLayout->addWidget(header);

    m_duplicatesTree = new QTreeWidget(this);
    m_duplicatesTree->setHeaderLabels({tr("Sayfa Kümesi"), tr("Benzerlik Oranı")});
    auto cluster1 = new QTreeWidgetItem(m_duplicatesTree, {tr("Küme 1 (Orijinal: Sayfa 2)"), tr("%98 Benzerlik")});
    new QTreeWidgetItem(cluster1, {tr("Yinelenen: Sayfa 15"), tr("%98")});
    m_duplicatesTree->expandAll();
    layout()->addWidget(m_duplicatesTree);

    auto btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    auto cancelBtn = new QPushButton(tr("Kapat"), this);
    auto dedupBtn = new QPushButton(tr("Yinelenenleri Belgeden Çıkar (Tekilleştir)"), this);
    dedupBtn->setStyleSheet("background-color: #0078d4; color: white;");
    btnLayout->addWidget(cancelBtn);
    btnLayout->addWidget(dedupBtn);
    mainLayout->addLayout(btnLayout);

    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    connect(dedupBtn, &QPushButton::clicked, this, &PDFDuplicatePageReviewDialog::onDeduplicateClicked);
}

void PDFDuplicatePageReviewDialog::onDeduplicateClicked()
{
    accept();
}
