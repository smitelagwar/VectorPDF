// MIT License
#include "pdffillsignpanel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QListWidget>
#include <QLabel>
#include <QMessageBox>

PDFFillSignPanel::PDFFillSignPanel(QWidget* parent)
    : QWidget(parent)
{
    initUi();
}

PDFFillSignPanel::~PDFFillSignPanel() = default;

void PDFFillSignPanel::setDocument(pdf::PDFDocument* document)
{
    m_document = document;
    reloadSignatures();
}

void PDFFillSignPanel::initUi()
{
    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins(12, 12, 12, 12);
    layout->setSpacing(8);

    auto headerLabel = new QLabel(tr("Görsel İmza ve Paraf (Fill & Sign)"), this);
    headerLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
    layout->addWidget(headerLabel);

    auto hintLabel = new QLabel(tr("Görsel imza bir imza görünümü ekler; kriptografik kimlik doğrulaması sağlamaz."), this);
    hintLabel->setWordWrap(true);
    hintLabel->setStyleSheet("color: #8b949e; font-size: 11px;");
    layout->addWidget(hintLabel);

    m_signatureList = new QListWidget(this);
    m_signatureList->setIconSize(QSize(120, 50));
    layout->addWidget(m_signatureList);

    auto btnLayout = new QHBoxLayout();
    m_addSignatureBtn = new QPushButton(tr("+ İmza Ekle"), this);
    m_addInitialsBtn = new QPushButton(tr("+ Paraf Ekle"), this);
    btnLayout->addWidget(m_addSignatureBtn);
    btnLayout->addWidget(m_addInitialsBtn);
    layout->addLayout(btnLayout);

    m_clearBtn = new QPushButton(tr("Kayıtlı İmzaları Temizle"), this);
    m_clearBtn->setStyleSheet("background-color: #5a1e28; color: #ffb4b4;");
    layout->addWidget(m_clearBtn);

    connect(m_addSignatureBtn, &QPushButton::clicked, this, &PDFFillSignPanel::onAddSignatureClicked);
    connect(m_addInitialsBtn, &QPushButton::clicked, this, &PDFFillSignPanel::onAddInitialsClicked);
    connect(m_clearBtn, &QPushButton::clicked, this, &PDFFillSignPanel::onClearAllSignatures);
}

void PDFFillSignPanel::reloadSignatures()
{
    if (!m_signatureList)
        return;
    m_signatureList->clear();
    m_signatureList->addItem(tr("Varsayılan Çizim İmza"));
    m_signatureList->addItem(tr("Varsayılan Metin İmza (El Yazısı)"));
}

void PDFFillSignPanel::onAddSignatureClicked()
{
    reloadSignatures();
}

void PDFFillSignPanel::onAddInitialsClicked()
{
    reloadSignatures();
}

void PDFFillSignPanel::onClearAllSignatures()
{
    if (m_signatureList)
        m_signatureList->clear();
}
