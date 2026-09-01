// MIT License
#include "pdfscancleanupstudio.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>

PDFScanCleanupStudio::PDFScanCleanupStudio(pdf::PDFDocument* document, QWidget* parent)
    : QDialog(parent)
    , m_document(document)
{
    setWindowTitle(tr("Tarama Temizleme Stüdyosu (Scan Cleanup Studio)"));
    resize(720, 480);
    initUi();
}

PDFScanCleanupStudio::~PDFScanCleanupStudio() = default;

void PDFScanCleanupStudio::initUi()
{
    auto mainLayout = new QHBoxLayout(this);

    auto leftPanel = new QWidget(this);
    leftPanel->setFixedWidth(280);
    auto leftLayout = new QVBoxLayout(leftPanel);

    auto grp = new QGroupBox(tr("Temizleme & İyileştirme"), this);
    auto grpLayout = new QVBoxLayout(grp);

    m_deskewCheck = new QCheckBox(tr("Eğrilik Düzeltme (Deskew)"), this);
    m_deskewCheck->setChecked(true);
    m_rotateCheck = new QCheckBox(tr("Otomatik Yön Döndürme (Orientation)"), this);
    m_rotateCheck->setChecked(true);
    m_punchCheck = new QCheckBox(tr("Delgeç İzlerini Sil (Punch Holes)"), this);
    m_punchCheck->setChecked(true);
    m_shadowCheck = new QCheckBox(tr("Kenar Gölgelerini Temizle"), this);
    m_shadowCheck->setChecked(true);
    m_bgNormCheck = new QCheckBox(tr("Arka Plan Beyazlatma & Netleştirme"), this);
    m_bgNormCheck->setChecked(true);

    grpLayout->addWidget(m_deskewCheck);
    grpLayout->addWidget(m_rotateCheck);
    grpLayout->addWidget(m_punchCheck);
    grpLayout->addWidget(m_shadowCheck);
    grpLayout->addWidget(m_bgNormCheck);
    leftLayout->addWidget(grp);

    auto applyBtn = new QPushButton(tr("Tüm Sayfaları Temizle"), this);
    applyBtn->setDefault(true);
    leftLayout->addWidget(applyBtn);
    leftLayout->addStretch();
    mainLayout->addWidget(leftPanel);

    m_previewLabel = new QLabel(tr("Tarama Önizleme ve Temizleme Sonucu"), this);
    m_previewLabel->setAlignment(Qt::AlignCenter);
    m_previewLabel->setStyleSheet("background-color: #1a1a24; border: 1px solid #333; border-radius: 8px; color: #aaa;");
    mainLayout->addWidget(m_previewLabel);

    connect(applyBtn, &QPushButton::clicked, this, &PDFScanCleanupStudio::onProcessClicked);
}

void PDFScanCleanupStudio::onProcessClicked()
{
    accept();
}
