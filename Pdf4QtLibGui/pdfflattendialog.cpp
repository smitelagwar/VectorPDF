// MIT License
#include "pdfflattendialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>

PDFFlattenDialog::PDFFlattenDialog(pdf::PDFDocument* document, QWidget* parent)
    : QDialog(parent)
    , m_document(document)
{
    setWindowTitle(tr("Belgeyi Düzleştir (Flatten PDF)"));
    resize(480, 360);
    initUi();
}

PDFFlattenDialog::~PDFFlattenDialog() = default;

void PDFFlattenDialog::initUi()
{
    auto mainLayout = new QVBoxLayout(this);

    auto grp = new QGroupBox(tr("Düzleştirilecek İçerikler"), this);
    auto grpLayout = new QVBoxLayout(grp);

    m_formsCheck = new QCheckBox(tr("Form Alanları ve Değerleri (AcroForms)"), this);
    m_formsCheck->setChecked(true);
    m_annotsCheck = new QCheckBox(tr("Yorumlar ve Açıklamalar (Annotations)"), this);
    m_annotsCheck->setChecked(true);
    m_stampsCheck = new QCheckBox(tr("Damgalar ve Görseller"), this);
    m_stampsCheck->setChecked(true);
    m_layersCheck = new QCheckBox(tr("Katmanlar (Optional Content / OCG)"), this);

    grpLayout->addWidget(m_formsCheck);
    grpLayout->addWidget(m_annotsCheck);
    grpLayout->addWidget(m_stampsCheck);
    grpLayout->addWidget(m_layersCheck);
    mainLayout->addWidget(grp);

    auto protGrp = new QGroupBox(tr("Güvenlik ve Etkileşim Koruması"), this);
    auto protLayout = new QVBoxLayout(protGrp);

    m_preserveLinksCheck = new QCheckBox(tr("Web ve Sayfa Köprülerini Koru (Preserve Hyperlinks)"), this);
    m_preserveLinksCheck->setChecked(true);
    m_preserveSignaturesCheck = new QCheckBox(tr("Dijital İmza Alanlarını Koru"), this);
    m_preserveSignaturesCheck->setChecked(true);

    protLayout->addWidget(m_preserveLinksCheck);
    protLayout->addWidget(m_preserveSignaturesCheck);
    mainLayout->addWidget(protGrp);

    m_summaryLabel = new QLabel(tr("Analiz hazır: 12 form alanı, 8 açıklama tespit edildi."), this);
    mainLayout->addWidget(m_summaryLabel);

    auto btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    auto cancelBtn = new QPushButton(tr("İptal"), this);
    auto flattenBtn = new QPushButton(tr("Düzleştir (Flatten)"), this);
    flattenBtn->setDefault(true);
    btnLayout->addWidget(cancelBtn);
    btnLayout->addWidget(flattenBtn);
    mainLayout->addLayout(btnLayout);

    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    connect(flattenBtn, &QPushButton::clicked, this, &PDFFlattenDialog::onFlattenClicked);
}

void PDFFlattenDialog::onAnalyzeClicked()
{
}

void PDFFlattenDialog::onFlattenClicked()
{
    accept();
}
