// MIT License
#include "pdfstampdock.h"
#include "pdfcustomstampdialog.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QListWidget>
#include <QPushButton>

PDFStampDock::PDFStampDock(QWidget* parent)
    : QDockWidget(tr("Damgalar & Mühürler (Stamps)"), parent)
{
    initUi();
}

PDFStampDock::~PDFStampDock() = default;

void PDFStampDock::setDocument(pdf::PDFDocument* document)
{
    m_document = document;
}

void PDFStampDock::initUi()
{
    auto container = new QWidget(this);
    auto layout = new QVBoxLayout(container);

    m_stampList = new QListWidget(this);
    m_stampList->setIconSize(QSize(120, 50));
    m_stampList->addItem(tr("✅ ONAYLANDI (Standart)"));
    m_stampList->addItem(tr("🔒 GİZLİ (Standart)"));
    m_stampList->addItem(tr("🕒 DİNAMİK ONAY (Tarih & Saatli)"));
    layout->addWidget(m_stampList);

    auto addCustomBtn = new QPushButton(tr("+ Özel Damga Oluştur"), this);
    layout->addWidget(addCustomBtn);

    setWidget(container);

    connect(addCustomBtn, &QPushButton::clicked, this, &PDFStampDock::onAddCustomStampClicked);
}

void PDFStampDock::onAddCustomStampClicked()
{
    PDFCustomStampDialog dlg(this);
    dlg.exec();
}
