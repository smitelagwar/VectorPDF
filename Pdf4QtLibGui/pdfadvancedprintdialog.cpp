// MIT License
#include "pdfadvancedprintdialog.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>

PDFAdvancedPrintDialog::PDFAdvancedPrintDialog(pdf::PDFDocument* document, QWidget* parent)
    : QDialog(parent)
    , m_document(document)
{
    setWindowTitle(tr("Gelişmiş Baskı ve Empoziyon (Advanced Print)"));
    resize(520, 320);
    initUi();
}

PDFAdvancedPrintDialog::~PDFAdvancedPrintDialog() = default;

void PDFAdvancedPrintDialog::initUi()
{
    auto mainLayout = new QVBoxLayout(this);
    auto form = new QFormLayout();

    m_layoutCombo = new QComboBox(this);
    m_layoutCombo->addItem(tr("Standart (Sayfa Başına 1 Sayfa)"));
    m_layoutCombo->addItem(tr("N-Up (Sayfa Başına Çoklu Sayfa)"));
    m_layoutCombo->addItem(tr("Kitapçık Empoziyonu (Booklet)"));
    m_layoutCombo->addItem(tr("Büyük Boyut / Afiş Döşeme (Poster Tiling)"));
    form->addRow(tr("Baskı Düzeni (Layout):"), m_layoutCombo);

    m_nUpCombo = new QComboBox(this);
    m_nUpCombo->addItem(tr("2 Sayfa / Yaprak"));
    m_nUpCombo->addItem(tr("4 Sayfa / Yaprak"));
    m_nUpCombo->addItem(tr("6 Sayfa / Yaprak"));
    m_nUpCombo->addItem(tr("9 Sayfa / Yaprak"));
    form->addRow(tr("N-Up Sayfa Sayısı:"), m_nUpCombo);

    m_copiesSpin = new QSpinBox(this);
    m_copiesSpin->setRange(1, 999);
    m_copiesSpin->setValue(1);
    form->addRow(tr("Kopya Sayısı:"), m_copiesSpin);

    mainLayout->addLayout(form);

    auto btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    auto cancelBtn = new QPushButton(tr("İptal"), this);
    auto printBtn = new QPushButton(tr("Yazdır"), this);
    printBtn->setDefault(true);
    btnLayout->addWidget(cancelBtn);
    btnLayout->addWidget(printBtn);
    mainLayout->addLayout(btnLayout);

    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    connect(printBtn, &QPushButton::clicked, this, &PDFAdvancedPrintDialog::onPrintClicked);
}

void PDFAdvancedPrintDialog::onPrintClicked()
{
    accept();
}
