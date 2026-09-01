// MIT License
#include "pdfwatermarkdialog.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSlider>
#include <QSpinBox>
#include <QComboBox>
#include <QPushButton>

PDFWatermarkDialog::PDFWatermarkDialog(pdf::PDFDocument* document, QWidget* parent)
    : QDialog(parent)
    , m_document(document)
{
    setWindowTitle(tr("Filigran Ekle (Watermark)"));
    resize(480, 320);
    initUi();
}

PDFWatermarkDialog::~PDFWatermarkDialog() = default;

void PDFWatermarkDialog::initUi()
{
    auto mainLayout = new QVBoxLayout(this);
    auto formLayout = new QFormLayout();

    m_textEdit = new QLineEdit(tr("GİZLİ"), this);
    formLayout->addRow(tr("Filigran Metni:"), m_textEdit);

    m_rotationSpin = new QSpinBox(this);
    m_rotationSpin->setRange(-180, 180);
    m_rotationSpin->setValue(-45);
    formLayout->addRow(tr("Döndürme Açısı (°):"), m_rotationSpin);

    m_opacitySlider = new QSlider(Qt::Horizontal, this);
    m_opacitySlider->setRange(10, 100);
    m_opacitySlider->setValue(30);
    formLayout->addRow(tr("Şeffaflık (%):"), m_opacitySlider);

    m_layerCombo = new QComboBox(this);
    m_layerCombo->addItem(tr("İçeriğin Önünde (Overlay)"));
    m_layerCombo->addItem(tr("İçeriğin Arkasında (Background)"));
    formLayout->addRow(tr("Konum:"), m_layerCombo);

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
    connect(applyBtn, &QPushButton::clicked, this, &PDFWatermarkDialog::onApplyClicked);
}

void PDFWatermarkDialog::onApplyClicked()
{
    accept();
}
