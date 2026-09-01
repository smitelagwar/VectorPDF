# VectorPDF — Çekirdek API Referansı (Core API Reference)

Bu belge, `Pdf4QtLibCore` kütüphanesinin sunduğu temel C++20 API sınıflarını, veri modellerini ve güvenlik mekanizmalarını açıklar.

---

## 1. Belge Oturumu (`PDFDocumentSession`)

Dosya: `Pdf4QtLibCore/sources/pdfdocumentsession.h`

Belgenin çalışma zamanı durumunu, salt okunurluk güvencelerini ve atomik kayıt hattını yöneten merkezi sınıftır.

### Metotlar:
- `bool isOpen() const`: Belgenin bellekte açık olup olmadığını döndürür.
- `bool isModified() const`: Belgede kaydedilmemiş değişiklik olup olmadığını döndürür.
- `bool isSigned() const`: Belgenin dijital imza içerip içermediğini sorgular.
- `bool isReadOnly() const`: Belgenin salt okunur kilitte olup olmadığını bildirir.
- `bool canSaveInPlace() const`: Orijinal dosyanın üzerine güvenle yazılıp yazılamayacağını denetler. (İmzalı belgelerde `false` döner).
- `void markModified()`: Oturumu değiştirilmiş (dirty) olarak işaretler.
- `void markClean()`: Oturumu temiz (clean) durumuna geçirir.

---

## 2. Toplu İşlem Motoru (`PDFBatchProcessor`)

Dosya: `Pdf4QtLibCore/sources/pdfbatchprocessor.h`

Birden fazla PDF belgesi üzerinde asenkron dönüştürme, OCR, optimizasyon ve PDF/A doğrulama işlemlerini sıralayan kuyruk yöneticisidir.

### Metotlar:
- `void addJob(const PDFBatchJob& job)`: Kuyruğa yeni bir toplu işlem ekler.
- `bool cancelJob(const QString& jobId)`: Belirtilen görevi iptal eder.
- `bool retryJob(const QString& jobId)`: Başarısız olmuş bir görevi yeniden kuyruğa alır.
- `void startProcessing()`: Kuyruktaki bekleyen işlerin işlenmesini başlatır.
- `void stopProcessing()`: İşlem kuyruğunu duraklatır.
- `QJsonObject generateReportJson() const`: Tüm işlerin sonuçlarını yapılandırılmış JSON formatında raporlar.

### Sinyaller (Signals):
- `jobStarted(QString jobId)`: Görev başladığında tetiklenir.
- `jobProgress(QString jobId, int percentage)`: Görev ilerleme yüzdesi (0-100) güncellendiğinde tetiklenir.
- `jobFinished(QString jobId, bool success, QString errorMessage)`: Görev bittiğinde tetiklenir.
- `allJobsCompleted(int totalSuccess, int totalFailed)`: Tüm kuyruk tamamlandığında tetiklenir.

---

## 3. Güvenli Eklenti Mimarisi (`PDFPluginManifest` & `PDFPluginSecurityGuard`)

Dosya: `Pdf4QtLibCore/sources/pdfplugin.h`

Eklentilerin yetki alanlarını sınırlayan ve izin denetimlerini gerçekleştiren güvenlik altyapısıdır.

### Sınıflar:
- `PDFPluginManifest`: Eklentinin adı, kimliği, sürümü ve talep ettiği yetkileri (`PDFPluginCapability`) içeren yapı.
- `PDFPluginSecurityGuard`: Belirli bir eylemin manifest yetkileri dahilinde olup olmadığını denetler (`isActionPermitted`).

---

## 4. Çevrimdışı Yapay Zekâ Sağlayıcı Adaptörü (`IAIProvider` & `PDFAIOfflineProvider`)

Dosya: `Pdf4QtLibCore/sources/pdfaiprovider.h`

Kullanıcı izni (`AITaskConsent`) doğrulanmadan hiçbir işlem yapmayan, prompt injection filtrelerine sahip ve %100 çevrimdışı çalışan AI yardımcı arayüzüdür.

### Metotlar:
- `AIExecutionResult summarizeText(const QString& text, const AITaskConsent& consent, int maxSentences = 5)`: Metin özetleme.
- `AIExecutionResult extractKeyPhrases(const QString& text, const AITaskConsent& consent, int maxPhrases = 10)`: Anahtar kavram çıkarma.
- `AIExecutionResult suggestTags(const QString& text, const AITaskConsent& consent)`: Belge etiket önerisi.
- `static QString sanitizeInputPrompt(const QString& input)`: Özel prompt belirteçlerini temizler ve güvenli hale getirir.
