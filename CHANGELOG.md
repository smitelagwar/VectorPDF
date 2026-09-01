# Değişiklik Günlüğü

Bu dosya yalnız kullanıcıya görünen ürün değişikliklerini [Keep a Changelog](https://keepachangelog.com/en/1.1.0/) yaklaşımıyla tutar. Araştırma ve iç süreç değişiklikleri `docs/ENGINEERING_LOG.md` içindedir.

# Değişiklik Günlüğü

Bu dosya yalnız kullanıcıya görünen ürün değişikliklerini [Keep a Changelog](https://keepachangelog.com/en/1.1.0/) yaklaşımıyla tutar. Araştırma ve iç süreç değişiklikleri `docs/ENGINEERING_LOG.md` içindedir.

## [1.0.0.0] - 2026-09-01

### Eklendi (Added)
- **VectorPDF Uygulama Paketi:** VectorPDF Viewer (Hızlı Görüntüleyici), VectorPDF Editor (Tam PDF Düzenleyici), VectorPDF PageMaster (Sayfa Yöneticisi), VectorPDF Diff (Görsel ve Metinsel Belge Karşılaştırma) ve VectorPDF LaunchPad.
- **P0 Güvenlik ve Veri Bütünlüğü:** `PDFDocumentSession` oturum yönetimi, `QSaveFile` atomik kayıt hattı, dijital imzalı belgeleri üzerine yazılmaktan koruyan güvenlik bariyeri ve tehlikeli eylemler (JavaScript/Launch) için varsayılan deny politikası.
- **Okuyucu Deneyimi:** Tek sayfa, sürekli kaydırma, iki sayfa ve iki sütun düzenleri; fit-page / fit-width yakınlaştırma; regex destekli gelişmiş metin arama ve pano metin kopyalama.
- **Sayfa İşlemleri:** Sayfa döndürme, silme, ekleme, ayırma, birleştirme ve `PDFDocumentManipulator` optimizasyonu.
- **Açıklama (Annotation) Araçları:** Metin vurgulama, altı/üstü çizili, serbest metin kutuları, kalem/mürekkep çizimi, damgalar, geometrik şekiller ve yapışkan notlar.
- **Komut Tabanlı Geri Al / Yinele (Undo / Redo):** `PDFUndoRedoManager` ile tüm belge düzenleme işlemlerinde güvenli geçmiş yönetimi.
- **AcroForm ve Form Tasarımı:** Etkileşimli metin kutuları, onay kutuları, radyo düğmeleri, açılır listeler, sekme sırası (tab order) ve appearance stream üretimi.
- **Kalıcı Karartma (True Redaction):** İçerik akışındaki metin gliflerini ve görsel piksellerini kalıcı silen gerçek redaction motoru; Info, XMP ve PieceInfo metaveri temizleme.
- **Gelişmiş Metin ve Tipografi Düzenleme:** `PDFDocumentTextFlowEditorModel` metin akış düzenleyici, FreeType font önbelleği ve akıllı görsel boyutu optimize edici (`PDFImageOptimizer`).
- **Kripto ve Dijital İmza:** ISO 32000-2 Rev 6 uyumlu AES-256 şifreleme ve OpenSSL tabanlı PAdES / PKCS#7 dijital imza doğrulama (`PDFSignatureHandler`).
- **OCR ve Taranmış Belge Desteği:** Otomatik eğrilik düzeltme (deskew), Otsu binarizasyonu, Tesseract Türkçe OCR (`tur.traineddata`) ve görünmez aranabilir metin katmanı.
- **Dönüştürme ve Standartlar:** PDF/A arşiv profili kontrolleri, JPEG/PNG/TIFF/WebP formatlarından doğrudan PDF üretimi ve yüksek çözünürlüklü dışa aktarım.
- **Windows 11 Entegrasyonu:** MSIX paketleme manifesti (`AppxManifest.xml.in`), `.pdf` dosya ilişkilendirmeleri ve bağımsız taşınabilir (portable) dağıtım desteği.
- **Otomasyon ve Toplu İşlem (Batch Queue):** `PDFBatchProcessor` ile toplu dönüştürme, OCR, optimizasyon, sayfa ayıklama, ilerleme takibi ve hata toleranslı yeniden deneme/iptal kuyruğu.
- **Güvenli Eklenti Mimarisi:** İzin tabanlı `PDFPluginManifest` ve `PDFPluginSecurityGuard` ile yetki kısıtlama (`ReadDocument`, `ModifyDocument`, `ExecuteExternalProcess`) ve dijital imza doğrulaması.
- **Çevrimdışı Yerel Yapay Zekâ Adaptörü:** `IAIProvider` ve `PDFAIOfflineProvider` ile görev bazlı açık izin (`AITaskConsent`), prompt injection filtreleme, yerel metin özetleme ve anahtar kavram çıkarma.
- **Türkçe Dil Desteği:** 15.000'den fazla çevrilmiş UI dizesiyle eksiksiz Türkçe arayüz (`translations/PDF4QT_tr.ts`).

