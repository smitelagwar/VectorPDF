// MIT License
#include "pdffindreplacedock.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QListWidget>
#include <QPushButton>

PDFFindReplaceDock::PDFFindReplaceDock(QWidget* parent)
    : QDockWidget(tr("Gelişmiş Bul ve Değiştir"), parent)
{
    initUi();
}

PDFFindReplaceDock::~PDFFindReplaceDock() = default;

void PDFFindReplaceDock::setDocument(pdf::PDFDocument* document)
{
    m_document = document;
}

void PDFFindReplaceDock::initUi()
{
    auto container = new QWidget(this);
    auto layout = new QVBoxLayout(container);

    auto grid = new QGridLayout();
    m_findEdit = new QLineEdit(this);
    m_replaceEdit = new QLineEdit(this);
    grid->addWidget(new QLabel(tr("Aranan:")), 0, 0);
    grid->addWidget(m_findEdit, 0, 1);
    grid->addWidget(new QLabel(tr("Yeni Değer:")), 1, 0);
    grid->addWidget(m_replaceEdit, 1, 1);
    layout->addLayout(grid);

    auto opts = new QHBoxLayout();
    m_caseCheck = new QCheckBox(tr("Büyük/Küçük Harf"), this);
    m_wholeWordCheck = new QCheckBox(tr("Tam Kelime"), this);
    m_regexCheck = new QCheckBox(tr("Regex"), this);
    m_preserveCaseCheck = new QCheckBox(tr("Harf Düzenini Koru"), this);
    m_preserveCaseCheck->setChecked(true);
    opts->addWidget(m_caseCheck);
    opts->addWidget(m_wholeWordCheck);
    opts->addWidget(m_regexCheck);
    opts->addWidget(m_preserveCaseCheck);
    layout->addLayout(opts);

    auto searchBtn = new QPushButton(tr("Belgede Ara"), this);
    layout->addWidget(searchBtn);

    m_resultsList = new QListWidget(this);
    layout->addWidget(m_resultsList);

    auto actions = new QHBoxLayout();
    auto replaceSelectedBtn = new QPushButton(tr("Seçileni Değiştir"), this);
    auto replaceAllBtn = new QPushButton(tr("Tümünü Değiştir"), this);
    actions->addWidget(replaceSelectedBtn);
    actions->addWidget(replaceAllBtn);
    layout->addLayout(actions);

    setWidget(container);

    connect(searchBtn, &QPushButton::clicked, this, &PDFFindReplaceDock::onSearchClicked);
    connect(replaceSelectedBtn, &QPushButton::clicked, this, &PDFFindReplaceDock::onReplaceSelectedClicked);
    connect(replaceAllBtn, &QPushButton::clicked, this, &PDFFindReplaceDock::onReplaceAllClicked);
}

void PDFFindReplaceDock::onSearchClicked()
{
    if (m_resultsList)
    {
        m_resultsList->clear();
        m_resultsList->addItem(tr("Eşleşme 1 (Sayfa 1): Örnek metin"));
    }
}

void PDFFindReplaceDock::onReplaceSelectedClicked()
{
}

void PDFFindReplaceDock::onReplaceAllClicked()
{
}
