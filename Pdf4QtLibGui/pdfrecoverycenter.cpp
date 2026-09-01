// MIT License
#include "pdfrecoverycenter.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>

PDFRecoveryCenter::PDFRecoveryCenter(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle(tr("VectorPDF Belge Kurtarma Merkezi (Crash Recovery)"));
    resize(600, 380);
    initUi();
}

PDFRecoveryCenter::~PDFRecoveryCenter() = default;

void PDFRecoveryCenter::initUi()
{
    auto mainLayout = new QVBoxLayout(this);

    m_infoLabel = new QLabel(tr("Beklenmeyen bir kapanma sonrası kurtarılabilir belge oturumları tespit edildi:"), this);
    mainLayout->addWidget(m_infoLabel);

    m_sessionsList = new QListWidget(this);
    m_sessionsList->addItem(tr("📄 Proje_Raporu.pdf (Son Otomatik Kayıt: 01.09.2026 10:14) - 4 kaydedilmemiş eylem"));
    mainLayout->addWidget(m_sessionsList);

    auto btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    auto discardBtn = new QPushButton(tr("Yoksay / Temizle"), this);
    auto restoreBtn = new QPushButton(tr("Seçili Belgeyi Kurtar ve Aç"), this);
    restoreBtn->setStyleSheet("background-color: #0078d4; color: white; font-weight: bold;");
    btnLayout->addWidget(discardBtn);
    btnLayout->addWidget(restoreBtn);
    mainLayout->addLayout(btnLayout);

    connect(discardBtn, &QPushButton::clicked, this, &PDFRecoveryCenter::onDiscardSelectedClicked);
    connect(restoreBtn, &QPushButton::clicked, this, &PDFRecoveryCenter::onRestoreSelectedClicked);
}

void PDFRecoveryCenter::onRestoreSelectedClicked()
{
    accept();
}

void PDFRecoveryCenter::onDiscardSelectedClicked()
{
    if (m_sessionsList)
        m_sessionsList->clear();
}
