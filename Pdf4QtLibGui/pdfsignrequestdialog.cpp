// MIT License
#include "pdfsignrequestdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>

PDFSignRequestDialog::PDFSignRequestDialog(pdf::PDFDocument* document, QWidget* parent)
    : QDialog(parent)
    , m_document(document)
{
    setWindowTitle(tr("İmza İsteme ve İş Akışı Başlat (Request Signatures)"));
    resize(540, 420);
    initUi();
}

PDFSignRequestDialog::~PDFSignRequestDialog() = default;

void PDFSignRequestDialog::initUi()
{
    auto mainLayout = new QVBoxLayout(this);
    auto form = new QFormLayout();

    m_titleEdit = new QLineEdit(tr("Belge İmza Talebi"), this);
    form->addRow(tr("İş Akışı Başlığı:"), m_titleEdit);

    auto addLayout = new QHBoxLayout();
    m_nameEdit = new QLineEdit(this);
    m_nameEdit->setPlaceholderText(tr("Ad Soyad"));
    m_emailEdit = new QLineEdit(this);
    m_emailEdit->setPlaceholderText(tr("E-posta"));
    auto addBtn = new QPushButton(tr("+ Ekle"), this);
    addLayout->addWidget(m_nameEdit);
    addLayout->addWidget(m_emailEdit);
    addLayout->addWidget(addBtn);
    form->addRow(tr("İmzacılar:"), addLayout);

    mainLayout->addLayout(form);

    m_signersList = new QListWidget(this);
    m_signersList->addItem(tr("[1] Ahmet Yılmaz (ahmet@ornek.com) - İmza"));
    mainLayout->addWidget(m_signersList);

    auto btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    auto cancelBtn = new QPushButton(tr("İptal"), this);
    auto sendBtn = new QPushButton(tr("İmza Paketini Oluştur ve Gönder (.vpdfwf)"), this);
    sendBtn->setDefault(true);
    btnLayout->addWidget(cancelBtn);
    btnLayout->addWidget(sendBtn);
    mainLayout->addLayout(btnLayout);

    connect(addBtn, &QPushButton::clicked, this, &PDFSignRequestDialog::onAddSignerClicked);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    connect(sendBtn, &QPushButton::clicked, this, &PDFSignRequestDialog::onSendRequestClicked);
}

void PDFSignRequestDialog::onAddSignerClicked()
{
    if (!m_emailEdit->text().isEmpty())
    {
        m_signersList->addItem(QStringLiteral("[%1] %2 (%3) - İmza").arg(m_signersList->count() + 1).arg(m_nameEdit->text(), m_emailEdit->text()));
        m_nameEdit->clear();
        m_emailEdit->clear();
    }
}

void PDFSignRequestDialog::onSendRequestClicked()
{
    accept();
}
