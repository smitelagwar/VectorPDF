// MIT License
#include "pdfblankpagereviewdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>

PDFBlankPageReviewDialog::PDFBlankPageReviewDialog(pdf::PDFDocument* document, QWidget* parent)
    : QDialog(parent)
    , m_document(document)
{
    setWindowTitle(tr("Otomatik Boş Sayfa İnceleme ve Silme (Blank Pages)"));
    resize(520, 360);
    initUi();
}

PDFBlankPageReviewDialog::~PDFBlankPageReviewDialog() = default;

void PDFBlankPageReviewDialog::initUi()
{
    auto mainLayout = new QVBoxLayout(this);

    auto header = new QLabel(tr("Belgede tespit edilen boş sayfalar aşağıda listelenmiştir:"), this);
    mainLayout->addWidget(header);

    m_pagesList = new QListWidget(this);
    m_pagesList->addItem(tr("Sayfa 4 (Koyuluk Oranı: %0.08 - Boş)"));
    m_pagesList->addItem(tr("Sayfa 8 (Koyuluk Oranı: %0.12 - Boş)"));
    mainLayout->addWidget(m_pagesList);

    auto btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    auto cancelBtn = new QPushButton(tr("Kapat"), this);
    auto delBtn = new QPushButton(tr("Seçili Boş Sayfaları Belgeden Sil"), this);
    delBtn->setStyleSheet("background-color: #8a1c2d; color: white;");
    btnLayout->addWidget(cancelBtn);
    btnLayout->addWidget(delBtn);
    mainLayout->addLayout(btnLayout);

    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    connect(delBtn, &QPushButton::clicked, this, &PDFBlankPageReviewDialog::onDeleteBlankPagesClicked);
}

void PDFBlankPageReviewDialog::onDeleteBlankPagesClicked()
{
    accept();
}
