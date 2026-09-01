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
- **M12 Kapsamlı Dönüştürme Motoru (VectorPDFConversion):** PDF'den Word/DOCX, Excel/XLSX, PowerPoint/PPTX, Text, Markdown, HTML5, EPUB, JSON, CSV formatlarına iki yönlü dönüştürme ve WIA tarayıcı arayüzü.
- **M13 Günlük Araçlar ve Erişilebilirlik (VectorPDFDocumentTools & VectorPDFAccessibility):**
  - **Fill & Sign (Görsel İmza & Paraf):** Çizim, el yazısı fontları ve saydam görsel imza, Windows DPAPI şifreli güvenli imza deposu.
  - **Header / Footer / Sayfa Numarası:** 6 konum slotu, dinamik şablon token motoru (`{page}`, `{pages}`, `{date}`, `{filename}`).
  - **Watermark & Arka Plan:** Metin, Görsel ve PDF filigran ve arka plan katmanları.
  - **Gelişmiş Bul ve Değiştir:** Büyük/küçük harf, regex, tam kelime ve Türkçe İ/ı korumalı toplu değişim.
  - **Smart Find & Redact:** TCKN, IBAN, Kredi Kartı (Luhn), E-posta, Telefon, IP, URL ve Plaka algılayıcıları ile kalıcı karartma.
  - **Flatten PDF:** Form alanları ve açıklamaları sayfa içeriğine dönüştüren güvenli düzleştirme motoru.
  - **Accessibility Checker & Auto-Tagging:** Başlık, dil, tag ağacı, okuma sırası denetleyicisi ve dürüst PDF/UA (veraPDF) arayüzü.
- **M14 İşbirliği, İnceleme, Adli İnceleme, Tarama ve Kurtarma (Collaboration, Review, Forensics, Scan & Recovery):**
  - **Document Recovery & Crash Resilience (VectorPDFRecovery):** Otomatik kayıt günlüğü (autosave journal), Windows DPAPI şifreli kurtarma deposu, kaza sonrası açılış tarayıcısı (LaunchScanner) ve oturum kurtarma merkezi.
  - **İmza İsteme ve Durum Takip:** İmzacı ve parafçı rolleri, çoklu imza alanları, iş akışı paketleme (`.vpdfwf`) ve çevrimdışı/LAN aktarım taşıyıcıları.
  - **Paylaşımlı İnceleme (Shared Review):** XFDF yorum entegrasyonu, yorum zincirleri (threads), yanıtlar, durum takibi ve paylaşımlı klasör/WebDAV senkronizasyonu.
  - **İnceleme ve Onaya Gönder (Approval Workflow):** Sıralı ve paralel onay zincirleri, onay/ret/şartlı onay durumları, değiştirilemez denetim izi (Audit Trail).
  - **Bates Numbering (Adli Numaralandırma):** Ön ek, basamak sayısı, son ek, sayfa konumu, çoklu belge toplu numaralandırma planı ve aralık bazlı sıra takibi.
  - **Dinamik ve Özel Damga Sistemi:** Standart iş damgaları, dinamik tarih/saat/kullanıcı damgaları, vektörel/resim/PDF özel damga oluşturucu ve damga kütüphanesi.
  - **Belgeyi Paylaşmadan Önce Temizle (Sanitize Before Sharing):** Metaveri, gizli katmanlar, ekler, JavaScript kodları, harici form eylemleri ve silinmiş nesne artıklarını derinlemesine temizleme.
  - **Gelişmiş Baskı Sistemi:** Kitapçık (Booklet), N-Up (sayfa başına çoklu sayfa), afiş/büyük boyut karo baskı ve prova önizleme.
  - **Scan Cleanup Studio (VectorPDFScanTools):** Otomatik yön algılama (0/90/180/270), eğrilik düzeltme (Deskew), kenar temizleme/otomatik kırpma, delgeç lekesi silme, arka plan beyazlatma ve gürültü giderme.
  - **Otomatik Boş Sayfa Bul ve Sil:** Eşik tabanlı varyans/içerik analizi, tersine taranmış/gölge toleransı ve kullanıcı onaylı toplu silme.
  - **Duplicate Page Detector (Çift Sayfa Tespiti):** Perceptual hashing (dHash/pHash), metinsel parmak izi, benzerlik kümeleme ve kullanıcı onaylı tekilleştirme.


