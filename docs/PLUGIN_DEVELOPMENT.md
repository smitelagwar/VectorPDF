# VectorPDF — Eklenti Geliştirme Kılavuzu (Plugin SDK)

Bu belge, VectorPDF için güvenli, izin tabanlı ve modüler eklentiler (plugins) geliştirmek isteyen mühendisler için teknik standartları ve yönergeleri içerir.

---

## 1. Mimari Genel Bakış

VectorPDF eklenti mimarisi, çekirdek güvenliği ve kullanıcı verisi gizliliğini korumak amacıyla **en az ayrıcalık (least privilege)** ilkesine dayanır.

Tüm eklentiler:
1. Bir **Manifest** (`plugin.json`) dosyası ile yetki alanlarını (`Capabilities`) açıkça deklare etmelidir.
2. `pdf::PDFPlugin` temel sınıfından türetilmelidir.
3. Yetkisiz dosya veya sistem eylemleri `PDFPluginSecurityGuard` tarafından otomatik olarak engellenir.

---

## 2. Yetki İzinleri (`PDFPluginCapability`)

Eklenti manifestinde talep edilebilecek izinler şunlardır:

| Yetki | Açıklama | Güvenlik Riski |
|---|---|---|
| `ReadDocument` | Açık olan PDF belgesinin sayfalarını ve metinlerini okur. | Düşük |
| `ModifyDocument` | Belgeye sayfa, açıklama (annotation) veya metin ekler/değiştirir. | Orta |
| `ExecuteExternalProcess` | Harici CLI araçlarını veya yardımcı programları çalıştırır. | Yüksek (Özel kullanıcı onayı gerektirir) |
| `AccessSystemInfo` | Ekran DPI, dil ve temel ortam bilgilerini sorgular. | Düşük |
| `NetworkAccessBlocked` | **Varsayılan kural.** Eklentilerin dış ağlara bağlanmasını kesin olarak engeller. | Yok (Gizlilik güvencesi) |

---

## 3. Örnek Eklenti Manifesti (`plugin.json`)

```json
{
  "Id": "com.ornek.metinsayaci",
  "Name": "Gelişmiş Metin Sayacı Eklentisi",
  "Author": "Geliştirici Adı",
  "Version": "1.0.0",
  "MinAppVersion": "1.1.0",
  "MaxAppVersion": "2.0.0",
  "License": "MIT",
  "Description": "PDF belgesindeki toplam kelime, karakter ve paragraf sayısını analiz eder.",
  "Capabilities": [
    "ReadDocument"
  ]
}
```

---

## 4. Örnek C++ Eklenti Kod Şablonu

```cpp
#include "pdfplugin.h"
#include <QAction>
#include <QMessageBox>

class MetinSayaciPlugin : public pdf::PDFPlugin
{
    Q_OBJECT

public:
    explicit MetinSayaciPlugin(QObject* parent = nullptr)
        : pdf::PDFPlugin(parent)
    {
    }

    QString getPluginMenuName() const override
    {
        return QStringLiteral("Metin Sayacı");
    }

    std::vector<QAction*> getActions() const override
    {
        std::vector<QAction*> actions;
        QAction* sayacAction = new QAction(QStringLiteral("Kelime Sayısını Hesapla"), nullptr);
        
        connect(sayacAction, &QAction::triggered, this, [this]() {
            if (!m_document) {
                return;
            }
            
            // Güvenlik kontrolü
            QString denialReason;
            if (!pdf::PDFPluginSecurityGuard::isActionPermitted(m_manifest, pdf::PDFPluginCapability::ReadDocument, &denialReason)) {
                return;
            }

            // Sayfa metinlerini tara ve say
            size_t pageCount = m_document->getPageCount();
            QMessageBox::information(nullptr, "Sonuç", QString("Belgedeki toplam sayfa sayısı: %1").arg(pageCount));
        });

        actions.push_back(sayacAction);
        return actions;
    }
};
```

---

## 5. Eklentilerin Dağıtımı ve Kurulumu

- Eklenti ikili dosyaları (`.dll`) ve karşılık gelen `plugin.json` manifesti, VectorPDF `plugins/` dizinine yerleştirilir.
- VectorPDF başlatıldığında manifest dosyalarını tarar, yetkileri doğrular ve eklentileri arayüzün "Eklentiler" menüsüne dinamik olarak ekler.
