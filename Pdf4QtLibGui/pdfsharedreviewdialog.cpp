// MIT License
#include "pdfsharedreviewdialog.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>

PDFSharedReviewDialog::PDFSharedReviewDialog(pdf::PDFDocument* document, QWidget* parent)
    : QDialog(parent)
    , m_document(document)
{
    setWindowTitle(tr("Paylaşımlı İnceleme Başlat (Shared Review)"));
    resize(520, 320);
    initUi();
}

PDFSharedReviewDialog::~PDFSharedReviewDialog() = default;

void PDFSharedReviewDialog::initUi()
{
    auto mainLayout = new QVBoxLayout(this);
    auto form = new QFormLayout();

    m_titleEdit = new QLineEdit(tr("Belge Taslak İncelemesi"), this);
    form->addRow(tr("İnceleme Başlığı:"), m_titleEdit);

    m_storageCombo = new QComboBox(this);
    m_storageCombo->addItem(tr("Ağ Paylaşım Klasörü (Shared Folder / SMB)"));
    m_storageCombo->addItem(tr("Yerel Dosya Sistemi (Local Folder)"));
    m_storageCombo->addItem(tr("WebDAV Sunucusu"));
    form->addRow(tr("Yorum Depolama Yeri:"), m_storageCombo);

    m_pathEdit = new QLineEdit(QStringLiteral(R"(\sunucu\paylasim\incelemeler\)"), this);
    form->addRow(tr("Depolama Yolu:"), m_pathEdit);

    mainLayout->addLayout(form);

    auto btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    auto cancelBtn = new QPushButton(tr("İptal"), this);
    auto startBtn = new QPushButton(tr("İncelemeyi Başlat"), this);
    startBtn->setDefault(true);
    btnLayout->addWidget(cancelBtn);
    btnLayout->addWidget(startBtn);
    mainLayout->addLayout(btnLayout);

    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    connect(startBtn, &QPushButton::clicked, this, &PDFSharedReviewDialog::onStartReviewClicked);
}

void PDFSharedReviewDialog::onStartReviewClicked()
{
    accept();
}
