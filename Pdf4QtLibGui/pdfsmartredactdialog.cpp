// MIT License
#include "pdfsmartredactdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QTableView>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>

PDFSmartRedactDialog::PDFSmartRedactDialog(pdf::PDFDocument* document, QWidget* parent)
    : QDialog(parent)
    , m_document(document)
{
    setWindowTitle(tr("Akıllı Veri Karartma (Smart Redact)"));
    resize(720, 480);
    initUi();
}

PDFSmartRedactDialog::~PDFSmartRedactDialog() = default;

void PDFSmartRedactDialog::initUi()
{
    auto mainLayout = new QVBoxLayout(this);

    auto header = new QLabel(tr("Kişisel ve Hassas Verileri Otomatik Tara ve Karart"), this);
    header->setStyleSheet("font-weight: bold; font-size: 13px;");
    mainLayout->addWidget(header);

    auto filterLayout = new QHBoxLayout();
    m_emailCheck = new QCheckBox(tr("E-posta"), this);
    m_emailCheck->setChecked(true);
    m_phoneCheck = new QCheckBox(tr("Telefon"), this);
    m_phoneCheck->setChecked(true);
    m_tcCheck = new QCheckBox(tr("T.C. Kimlik"), this);
    m_tcCheck->setChecked(true);
    m_ibanCheck = new QCheckBox(tr("IBAN"), this);
    m_ibanCheck->setChecked(true);
    m_cardCheck = new QCheckBox(tr("Kredi Kartı"), this);
    m_cardCheck->setChecked(true);

    filterLayout->addWidget(m_emailCheck);
    filterLayout->addWidget(m_phoneCheck);
    filterLayout->addWidget(m_tcCheck);
    filterLayout->addWidget(m_ibanCheck);
    filterLayout->addWidget(m_cardCheck);
    mainLayout->addLayout(filterLayout);

    auto scanBtn = new QPushButton(tr("Belgeyi Tara"), this);
    mainLayout->addWidget(scanBtn);

    m_tableView = new QTableView(this);
    mainLayout->addWidget(m_tableView);

    auto btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    auto cancelBtn = new QPushButton(tr("Kapat"), this);
    auto applyBtn = new QPushButton(tr("Kalıcı Olarak Karart (Apply Redact)"), this);
    applyBtn->setStyleSheet("background-color: #8a1c2d; color: white; font-weight: bold;");
    btnLayout->addWidget(cancelBtn);
    btnLayout->addWidget(applyBtn);
    mainLayout->addLayout(btnLayout);

    connect(scanBtn, &QPushButton::clicked, this, &PDFSmartRedactDialog::onScanClicked);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    connect(applyBtn, &QPushButton::clicked, this, &PDFSmartRedactDialog::onApplyPermanentlyClicked);
}

void PDFSmartRedactDialog::onScanClicked()
{
}

void PDFSmartRedactDialog::onApplyPermanentlyClicked()
{
    accept();
}
