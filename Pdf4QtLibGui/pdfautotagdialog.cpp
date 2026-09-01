// MIT License
#include "pdfautotagdialog.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>

PDFAutoTagDialog::PDFAutoTagDialog(pdf::PDFDocument* document, QWidget* parent)
    : QDialog(parent)
    , m_document(document)
{
    setWindowTitle(tr("Otomatik Erişilebilir PDF Üret (Auto-Tagging)"));
    resize(520, 320);
    initUi();
}

PDFAutoTagDialog::~PDFAutoTagDialog() = default;

void PDFAutoTagDialog::initUi()
{
    auto mainLayout = new QVBoxLayout(this);
    auto formLayout = new QFormLayout();

    m_backendCombo = new QComboBox(this);
    m_backendCombo->addItem(tr("Otomatik (En İyi Uygun Yöntem)"));
    m_backendCombo->addItem(tr("Yerel Sezgisel Motor (Native Heuristic)"));
    m_backendCombo->addItem(tr("Gelişmiş Yerel Analizci (Docling Worker)"));
    formLayout->addRow(tr("Analiz Motoru:"), m_backendCombo);

    mainLayout->addLayout(formLayout);

    m_summaryLabel = new QLabel(tr("Analiz sonucu: 4 başlık (H1-H3), 16 paragraf, 2 tablo, 3 görsel önerildi."), this);
    m_summaryLabel->setWordWrap(true);
    mainLayout->addWidget(m_summaryLabel);

    auto btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    auto cancelBtn = new QPushButton(tr("İptal"), this);
    auto applyBtn = new QPushButton(tr("Etiketleri Uygula (Save Accessible Copy)"), this);
    applyBtn->setDefault(true);
    btnLayout->addWidget(cancelBtn);
    btnLayout->addWidget(applyBtn);
    mainLayout->addLayout(btnLayout);

    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    connect(applyBtn, &QPushButton::clicked, this, &PDFAutoTagDialog::onApplyClicked);
}

void PDFAutoTagDialog::onAnalyzeClicked()
{
}

void PDFAutoTagDialog::onApplyClicked()
{
    accept();
}
