// MIT License
#include "pdfreviewdock.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTreeWidget>
#include <QPushButton>

PDFReviewDock::PDFReviewDock(QWidget* parent)
    : QDockWidget(tr("Paylaşımlı Yorumlar ve İnceleme (Shared Review)"), parent)
{
    initUi();
}

PDFReviewDock::~PDFReviewDock() = default;

void PDFReviewDock::setDocument(pdf::PDFDocument* document)
{
    m_document = document;
}

void PDFReviewDock::initUi()
{
    auto container = new QWidget(this);
    auto layout = new QVBoxLayout(container);

    auto btnLayout = new QHBoxLayout();
    auto syncBtn = new QPushButton(tr("🔄 Yorumları Eşitle"), this);
    auto resolveBtn = new QPushButton(tr("✅ Çözüldü İşaretle"), this);
    btnLayout->addWidget(syncBtn);
    btnLayout->addWidget(resolveBtn);
    layout->addLayout(btnLayout);

    m_commentsTree = new QTreeWidget(this);
    m_commentsTree->setHeaderLabels({tr("Yorum / Katılımcı"), tr("Tarih / Durum")});
    auto t1 = new QTreeWidgetItem(m_commentsTree, {tr("1. Paragraf revizyonu gerekli"), tr("AÇIK")});
    new QTreeWidgetItem(t1, {tr("Ahmet: Bu cümlenin ifade tarzı değiştirilmeli."), tr("10:20")});
    m_commentsTree->expandAll();
    layout->addWidget(m_commentsTree);

    setWidget(container);

    connect(syncBtn, &QPushButton::clicked, this, &PDFReviewDock::onSyncCommentsClicked);
    connect(resolveBtn, &QPushButton::clicked, this, &PDFReviewDock::onResolveSelectedClicked);
}

void PDFReviewDock::onSyncCommentsClicked()
{
}

void PDFReviewDock::onResolveSelectedClicked()
{
}
