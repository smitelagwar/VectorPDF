// MIT License
#include "pdfsanitizedialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>

PDFSanitizeDialog::PDFSanitizeDialog(pdf::PDFDocument* document, QWidget* parent)
    : QDialog(parent)
    , m_document(document)
{
    setWindowTitle(tr("Belgeyi Paylaşmadan Önce Temizle (Sanitize Before Sharing)"));
    resize(520, 360);
    initUi();
}

PDFSanitizeDialog::~PDFSanitizeDialog() = default;

void PDFSanitizeDialog::initUi()
{
    auto mainLayout = new QVBoxLayout(this);

    auto header = new QLabel(tr("Gizli metaverileri, harici eylemleri ve ekleri temizleyerek belgeyi güvenle paylaşın."), this);
    header->setWordWrap(true);
    mainLayout->addWidget(header);

    auto grp = new QGroupBox(tr("Temizleme Seçenekleri"), this);
    auto grpLayout = new QVBoxLayout(grp);

    m_metaCheck = new QCheckBox(tr("Tüm Belge Metaverilerini Temizle (Başlık, Yazar, Konu, Anahtar Kelimeler)"), this);
    m_metaCheck->setChecked(true);
    m_attachCheck = new QCheckBox(tr("Gömülü Ek Dosyaları Sil (Embedded Attachments)"), this);
    m_attachCheck->setChecked(true);
    m_jsCheck = new QCheckBox(tr("Tüm JavaScript Kodlarını ve Eylemlerini Kaldır"), this);
    m_jsCheck->setChecked(true);
    m_actionsCheck = new QCheckBox(tr("Harici Bağlantı ve Başlatma (Launch) Eylemlerini Temizle"), this);
    m_actionsCheck->setChecked(true);
    m_layersCheck = new QCheckBox(tr("Gizli Katmanları (Hidden Layers) Sil"), this);
    m_layersCheck->setChecked(true);

    grpLayout->addWidget(m_metaCheck);
    grpLayout->addWidget(m_attachCheck);
    grpLayout->addWidget(m_jsCheck);
    grpLayout->addWidget(m_actionsCheck);
    grpLayout->addWidget(m_layersCheck);
    mainLayout->addWidget(grp);

    auto btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    auto cancelBtn = new QPushButton(tr("İptal"), this);
    auto sanitizeBtn = new QPushButton(tr("Temizle ve Yeni Kopya Olarak Kaydet"), this);
    sanitizeBtn->setDefault(true);
    btnLayout->addWidget(cancelBtn);
    btnLayout->addWidget(sanitizeBtn);
    mainLayout->addLayout(btnLayout);

    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    connect(sanitizeBtn, &QPushButton::clicked, this, &PDFSanitizeDialog::onSanitizeClicked);
}

void PDFSanitizeDialog::onSanitizeClicked()
{
    accept();
}
