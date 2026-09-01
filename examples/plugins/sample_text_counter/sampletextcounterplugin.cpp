// MIT License
//
// Copyright (c) 2026 VectorPDF Contributors

#include "sampletextcounterplugin.h"
#include <QAction>
#include <QMessageBox>

SampleTextCounterPlugin::SampleTextCounterPlugin(QObject* parent)
    : pdf::PDFPlugin(parent)
{
}

QString SampleTextCounterPlugin::getPluginMenuName() const
{
    return QStringLiteral("Metin Analizi");
}

std::vector<QAction*> SampleTextCounterPlugin::getActions() const
{
    std::vector<QAction*> actions;
    QAction* analyzeAction = new QAction(QStringLiteral("Sayfa ve Metin İstatistiklerini Göster"), nullptr);
    
    connect(analyzeAction, &QAction::triggered, this, [this]() {
        calculateStatistics();
    });

    actions.push_back(analyzeAction);
    return actions;
}

void SampleTextCounterPlugin::calculateStatistics()
{
    if (!m_document)
    {
        QMessageBox::warning(nullptr, QStringLiteral("Uyarı"), QStringLiteral("Lütfen önce bir PDF belgesi açın."));
        return;
    }

    // Yetki kontrolü (Least privilege security guard)
    QString denialReason;
    if (!pdf::PDFPluginSecurityGuard::isActionPermitted(m_manifest, pdf::PDFPluginCapability::ReadDocument, &denialReason))
    {
        QMessageBox::critical(nullptr, QStringLiteral("Güvenlik Engeli"), denialReason);
        return;
    }

    size_t pageCount = m_document->getPageCount();
    QString report = QStringLiteral("Belge İstatistikleri:\n- Toplam Sayfa Sayısı: %1\n- Eklenti Durumu: Güvenli ve İzinli").arg(pageCount);
    
    QMessageBox::information(nullptr, QStringLiteral("Metin ve Belge Analizi"), report);
}
