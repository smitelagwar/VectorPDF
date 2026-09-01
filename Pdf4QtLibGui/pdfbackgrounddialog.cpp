// MIT License
#include "pdfbackgrounddialog.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>

PDFBackgroundDialog::PDFBackgroundDialog(pdf::PDFDocument* document, QWidget* parent)
    : QDialog(parent)
    , m_document(document)
{
    setWindowTitle(tr("Sayfa Arka Planı Ekle / Değiştir"));
    resize(480, 260);
    initUi();
}

PDFBackgroundDialog::~PDFBackgroundDialog() = default;

void PDFBackgroundDialog::initUi()
{
    auto mainLayout = new QVBoxLayout(this);
    auto formLayout = new QFormLayout();

    m_typeCombo = new QComboBox(this);
    m_typeCombo->addItem(tr("Düz Renk"));
    m_typeCombo->addItem(tr("Görsel (PNG / JPG / BMP)"));
    m_typeCombo->addItem(tr("PDF Sayfası"));
    formLayout->addRow(tr("Arka Plan Türü:"), m_typeCombo);

    m_placementCombo = new QComboBox(this);
    m_placementCombo->addItem(tr("Sayfaya Sığdır (Fit)"));
    m_placementCombo->addItem(tr("Sayfayı Doldur (Fill)"));
    m_placementCombo->addItem(tr("Uzat (Stretch)"));
    m_placementCombo->addItem(tr("Ortala (Center)"));
    m_placementCombo->addItem(tr("Döşe (Tile)"));
    formLayout->addRow(tr("Yerleşim Modu:"), m_placementCombo);

    mainLayout->addLayout(formLayout);

    auto btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    auto cancelBtn = new QPushButton(tr("İptal"), this);
    auto applyBtn = new QPushButton(tr("Uygula"), this);
    applyBtn->setDefault(true);
    btnLayout->addWidget(cancelBtn);
    btnLayout->addWidget(applyBtn);
    mainLayout->addLayout(btnLayout);

    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    connect(applyBtn, &QPushButton::clicked, this, &PDFBackgroundDialog::onApplyClicked);
}

void PDFBackgroundDialog::onApplyClicked()
{
    accept();
}
