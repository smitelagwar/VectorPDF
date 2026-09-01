// MIT License
#include "pdfheaderfooterdialog.h"
#include <QVBoxLayout>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QGroupBox>

PDFHeaderFooterDialog::PDFHeaderFooterDialog(pdf::PDFDocument* document, QWidget* parent)
    : QDialog(parent)
    , m_document(document)
{
    setWindowTitle(tr("Üst Bilgi ve Alt Bilgi Ekle / Düzenle"));
    resize(580, 420);
    initUi();
}

PDFHeaderFooterDialog::~PDFHeaderFooterDialog() = default;

void PDFHeaderFooterDialog::initUi()
{
    auto mainLayout = new QVBoxLayout(this);

    auto headerGroup = new QGroupBox(tr("Üst Bilgi (Header)"), this);
    auto headerLayout = new QGridLayout(headerGroup);

    m_topLeftEdit = new QLineEdit(this);
    m_topCenterEdit = new QLineEdit(this);
    m_topRightEdit = new QLineEdit(this);

    headerLayout->addWidget(new QLabel(tr("Sol:")), 0, 0);
    headerLayout->addWidget(m_topLeftEdit, 0, 1);
    headerLayout->addWidget(new QLabel(tr("Orta:")), 0, 2);
    headerLayout->addWidget(m_topCenterEdit, 0, 3);
    headerLayout->addWidget(new QLabel(tr("Sağ:")), 0, 4);
    headerLayout->addWidget(m_topRightEdit, 0, 5);
    mainLayout->addWidget(headerGroup);

    auto footerGroup = new QGroupBox(tr("Alt Bilgi (Footer)"), this);
    auto footerLayout = new QGridLayout(footerGroup);

    m_bottomLeftEdit = new QLineEdit(this);
    m_bottomCenterEdit = new QLineEdit(QStringLiteral("{page} / {pages}"), this);
    m_bottomRightEdit = new QLineEdit(this);

    footerLayout->addWidget(new QLabel(tr("Sol:")), 0, 0);
    footerLayout->addWidget(m_bottomLeftEdit, 0, 1);
    footerLayout->addWidget(new QLabel(tr("Orta:")), 0, 2);
    footerLayout->addWidget(m_bottomCenterEdit, 0, 3);
    footerLayout->addWidget(new QLabel(tr("Sağ:")), 0, 4);
    footerLayout->addWidget(m_bottomRightEdit, 0, 5);
    mainLayout->addWidget(footerGroup);

    auto btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    auto cancelBtn = new QPushButton(tr("İptal"), this);
    auto applyBtn = new QPushButton(tr("Uygula"), this);
    applyBtn->setDefault(true);
    btnLayout->addWidget(cancelBtn);
    btnLayout->addWidget(applyBtn);
    mainLayout->addLayout(btnLayout);

    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    connect(applyBtn, &QPushButton::clicked, this, &PDFHeaderFooterDialog::onApplyClicked);
}

void PDFHeaderFooterDialog::onApplyClicked()
{
    accept();
}
