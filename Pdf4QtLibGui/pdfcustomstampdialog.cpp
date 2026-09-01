// MIT License
#include "pdfcustomstampdialog.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>

PDFCustomStampDialog::PDFCustomStampDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Özel Damga Tasarla (Custom Stamp)"));
    resize(420, 240);
    initUi();
}

PDFCustomStampDialog::~PDFCustomStampDialog() = default;

void PDFCustomStampDialog::initUi()
{
    auto mainLayout = new QVBoxLayout(this);
    auto form = new QFormLayout();

    m_textEdit = new QLineEdit(tr("KONTROL EDİLDİ"), this);
    form->addRow(tr("Damga Metni:"), m_textEdit);

    m_colorCombo = new QComboBox(this);
    m_colorCombo->addItem(tr("Yeşil (Onay)"));
    m_colorCombo->addItem(tr("Kırmızı (Gizli / Acil)"));
    m_colorCombo->addItem(tr("Mavi (Bilgi / Taslak)"));
    form->addRow(tr("Renk Şeması:"), m_colorCombo);

    mainLayout->addLayout(form);

    auto btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    auto cancelBtn = new QPushButton(tr("İptal"), this);
    auto saveBtn = new QPushButton(tr("Damgayı Kaydet"), this);
    saveBtn->setDefault(true);
    btnLayout->addWidget(cancelBtn);
    btnLayout->addWidget(saveBtn);
    mainLayout->addLayout(btnLayout);

    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    connect(saveBtn, &QPushButton::clicked, this, &QDialog::accept);
}
