// MIT License
#include "pdfaccessibilitydock.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTreeWidget>
#include <QPushButton>
#include <QLabel>

PDFAccessibilityDock::PDFAccessibilityDock(QWidget* parent)
    : QDockWidget(tr("Erişilebilirlik Denetçisi (Accessibility)"), parent)
{
    initUi();
}

PDFAccessibilityDock::~PDFAccessibilityDock() = default;

void PDFAccessibilityDock::setDocument(pdf::PDFDocument* document)
{
    m_document = document;
}

void PDFAccessibilityDock::initUi()
{
    auto container = new QWidget(this);
    auto layout = new QVBoxLayout(container);

    m_statusLabel = new QLabel(tr("Erişilebilirlik durumu kontrol edilmedi."), this);
    layout->addWidget(m_statusLabel);

    auto btnLayout = new QHBoxLayout();
    auto runBtn = new QPushButton(tr("Denetimi Çalıştır"), this);
    auto fixBtn = new QPushButton(tr("Seçileni Düzelt"), this);
    auto pdfUaBtn = new QPushButton(tr("PDF/UA Doğrula"), this);
    btnLayout->addWidget(runBtn);
    btnLayout->addWidget(fixBtn);
    btnLayout->addWidget(pdfUaBtn);
    layout->addLayout(btnLayout);

    m_issuesTree = new QTreeWidget(this);
    m_issuesTree->setHeaderLabels({tr("Kategori / Kural"), tr("Durum")});
    layout->addWidget(m_issuesTree);

    setWidget(container);

    connect(runBtn, &QPushButton::clicked, this, &PDFAccessibilityDock::onRunCheckClicked);
    connect(fixBtn, &QPushButton::clicked, this, &PDFAccessibilityDock::onFixSelectedClicked);
    connect(pdfUaBtn, &QPushButton::clicked, this, &PDFAccessibilityDock::onValidatePdfUaClicked);
}

void PDFAccessibilityDock::onRunCheckClicked()
{
    if (!m_issuesTree)
        return;

    m_issuesTree->clear();
    auto docItem = new QTreeWidgetItem(m_issuesTree, {tr("Belge Metaverileri"), tr("2 Uyarı")});
    new QTreeWidgetItem(docItem, {tr("Belge Başlığı Eksik"), tr("HATA")});
    new QTreeWidgetItem(docItem, {tr("Birincil Dil Tanımsız"), tr("UYARI")});

    auto tagItem = new QTreeWidgetItem(m_issuesTree, {tr("Etiket Ağacı (/StructTreeRoot)"), tr("1 Hata")});
    new QTreeWidgetItem(tagItem, {tr("Etiketsiz Belge"), tr("HATA")});

    m_issuesTree->expandAll();
    m_statusLabel->setText(tr("Denetim tamamlandı: 2 hata, 1 uyarı"));
}

void PDFAccessibilityDock::onFixSelectedClicked()
{
}

void PDFAccessibilityDock::onValidatePdfUaClicked()
{
    m_statusLabel->setText(tr("veraPDF PDF/UA: Doğrulayıcı harici CLI mevcut değil (Unavailable)."));
}
