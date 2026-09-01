// MIT License
#include "pdfapprovaldialog.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>

PDFApprovalDialog::PDFApprovalDialog(pdf::PDFDocument* document, QWidget* parent)
    : QDialog(parent)
    , m_document(document)
{
    setWindowTitle(tr("Belgeyi Onaya Gönder (Approval Workflow)"));
    resize(520, 380);
    initUi();
}

PDFApprovalDialog::~PDFApprovalDialog() = default;

void PDFApprovalDialog::initUi()
{
    auto mainLayout = new QVBoxLayout(this);
    auto form = new QFormLayout();

    m_titleEdit = new QLineEdit(tr("Sözleşme Final Onayı"), this);
    form->addRow(tr("Onay Başlığı:"), m_titleEdit);

    mainLayout->addLayout(form);

    m_approversList = new QListWidget(this);
    m_approversList->addItem(tr("1. Hukuk Müşaviri (hukuk@sirket.com)"));
    m_approversList->addItem(tr("2. Finans Direktörü (finans@sirket.com)"));
    m_approversList->addItem(tr("3. Genel Müdür (gm@sirket.com)"));
    mainLayout->addWidget(m_approversList);

    auto btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    auto cancelBtn = new QPushButton(tr("İptal"), this);
    auto sendBtn = new QPushButton(tr("Onay Akışını Başlat"), this);
    sendBtn->setDefault(true);
    btnLayout->addWidget(cancelBtn);
    btnLayout->addWidget(sendBtn);
    mainLayout->addLayout(btnLayout);

    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    connect(sendBtn, &QPushButton::clicked, this, &PDFApprovalDialog::onSendApprovalClicked);
}

void PDFApprovalDialog::onSendApprovalClicked()
{
    accept();
}
