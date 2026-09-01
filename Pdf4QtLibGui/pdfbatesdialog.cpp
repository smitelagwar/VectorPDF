// MIT License
#include "pdfbatesdialog.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QPushButton>

PDFBatesDialog::PDFBatesDialog(pdf::PDFDocument* document, QWidget* parent)
    : QDialog(parent)
    , m_document(document)
{
    setWindowTitle(tr("Bates Numaralandırma (Bates Numbering)"));
    resize(480, 320);
    initUi();
}

PDFBatesDialog::~PDFBatesDialog() = default;

void PDFBatesDialog::initUi()
{
    auto mainLayout = new QVBoxLayout(this);
    auto form = new QFormLayout();

    m_prefixEdit = new QLineEdit(QStringLiteral("CASE-2026-"), this);
    form->addRow(tr("Ön Ek (Prefix):"), m_prefixEdit);

    m_startNumberSpin = new QSpinBox(this);
    m_startNumberSpin->setRange(1, 9999999);
    m_startNumberSpin->setValue(1);
    form->addRow(tr("Başlangıç Numarası:"), m_startNumberSpin);

    m_digitsSpin = new QSpinBox(this);
    m_digitsSpin->setRange(3, 12);
    m_digitsSpin->setValue(6);
    form->addRow(tr("Basamak Sayısı (Digits):"), m_digitsSpin);

    m_suffixEdit = new QLineEdit(this);
    form->addRow(tr("Son Ek (Suffix):"), m_suffixEdit);

    m_positionCombo = new QComboBox(this);
    m_positionCombo->addItem(tr("Alt Sağ (Önerilen)"));
    m_positionCombo->addItem(tr("Alt Orta"));
    m_positionCombo->addItem(tr("Alt Sol"));
    m_positionCombo->addItem(tr("Üst Sağ"));
    m_positionCombo->addItem(tr("Üst Orta"));
    m_positionCombo->addItem(tr("Üst Sol"));
    form->addRow(tr("Sayfa Konumu:"), m_positionCombo);

    mainLayout->addLayout(form);

    auto btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    auto cancelBtn = new QPushButton(tr("İptal"), this);
    auto applyBtn = new QPushButton(tr("Numaralandırmayı Uygula"), this);
    applyBtn->setDefault(true);
    btnLayout->addWidget(cancelBtn);
    btnLayout->addWidget(applyBtn);
    mainLayout->addLayout(btnLayout);

    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    connect(applyBtn, &QPushButton::clicked, this, &PDFBatesDialog::onApplyClicked);
}

void PDFBatesDialog::onApplyClicked()
{
    accept();
}
