// MIT License
#include "pdfreadingorderdock.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>

PDFReadingOrderDock::PDFReadingOrderDock(QWidget* parent)
    : QDockWidget(tr("Okuma Sırası ve Tag Ağacı (Reading Order)"), parent)
{
    initUi();
}

PDFReadingOrderDock::~PDFReadingOrderDock() = default;

void PDFReadingOrderDock::setDocument(pdf::PDFDocument* document)
{
    m_document = document;
    if (m_blockList)
    {
        m_blockList->clear();
        m_blockList->addItem(tr("[1] <H1> Başlık Alanı"));
        m_blockList->addItem(tr("[2] <P> Giriş Paragrafı"));
        m_blockList->addItem(tr("[3] <Figure> Şirket Logosu"));
        m_blockList->addItem(tr("[4] <Table> Veri Tablosu"));
    }
}

void PDFReadingOrderDock::initUi()
{
    auto container = new QWidget(this);
    auto layout = new QVBoxLayout(container);

    layout->addWidget(new QLabel(tr("Sayfa Üzerindeki Okuma Blokları:"), this));

    m_blockList = new QListWidget(this);
    layout->addWidget(m_blockList);

    auto btnLayout = new QHBoxLayout();
    auto upBtn = new QPushButton(tr("Yukarı Taşı"), this);
    auto downBtn = new QPushButton(tr("Aşağı Taşı"), this);
    btnLayout->addWidget(upBtn);
    btnLayout->addWidget(downBtn);
    layout->addLayout(btnLayout);

    setWidget(container);

    connect(upBtn, &QPushButton::clicked, this, &PDFReadingOrderDock::onMoveUpClicked);
    connect(downBtn, &QPushButton::clicked, this, &PDFReadingOrderDock::onMoveDownClicked);
}

void PDFReadingOrderDock::onMoveUpClicked()
{
}

void PDFReadingOrderDock::onMoveDownClicked()
{
}
