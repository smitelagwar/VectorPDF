# Güncel Durum

**Tarih:** 2026-09-01  
**Faz:** M9 — P1 Dönüştürme, Dışa Aktarma ve Standartlar  
**Kod durumu:** VectorPDF C++20 / Qt6 çekirdeği devrede; okuyucu, güvenlik, sayfa işlemleri, formlar, redaction, metin düzenleme, kripto/imza ve OCR tamamlandı; PDF/A standart uyumluluğu ve format dönüştürme yönetiliyor.

## Birincil aktif görev

[TASK-2026-012 — M9 Dönüştürme, Dışa Aktarma ve Standartlar](docs/tasks/TASK-2026-012-m9-conversion-export-and-standards.md)

Yeni bir AI oturumunda `plana devam et` denildiğinde önce bu dosya, sonra yukarıdaki görev kaydının `Devam checkpoint'i` bölümü okunur. Görev bağlantısı değişmedikçe ayrıntılı kaldığım-yer bilgisi görev dosyasındadır; bu bölüm aynı ayrıntıyı tekrar etmez.

## Tamamlananlar

- Beş referans rapor incelendi ve ortak ilkeler/çelişkiler çıkarıldı.
- `ADR-0001`: permissive rota ve root MIT Accepted.
- `ADR-0002`: PDF4QT v1.6.0.0 birincil açık kaynak kod tabanı olarak Accepted.
- `ADR-0003`: Windows 11 x64 ve Store/MSIX-first Accepted.
- `ADR-0004`: ücretsiz/açık çekirdek ve offline/account-free temel kullanım Accepted.
- Gate 1 bake-off tamamlandı (`TASK-2026-002` done).
- 25 dosyalık ortak smoke korpus manifesti `tests/corpus/manifest.yml` içine işlendi.
- M0 VectorPDF kod entegrasyonu, CMake ve rebrand yapılandırması tamamlandı (`TASK-2026-003` done).
- M1 Güvenlik ve Veri Bütünlüğü: `PDFDocumentSession`, imzalı belge koruması ve atomik kayıt hattı tamamlandı (`TASK-2026-004` done).
- M2 Okuyucu ve Arayüz Kalitesi: Sayfa düzenleri, arama motoru, metin seçimi ve Türkçe yerelleştirme tamamlandı (`TASK-2026-005` done).
- M3 Sayfa İşlemleri ve Annotation: Sayfa manipülasyonu (`PDFDocumentManipulator`), 15+ açıklama tipi ve `PDFUndoRedoManager` tamamlandı (`TASK-2026-006` done).
- M4 AcroForm ve Form Tasarımı: Form motoru (`PDFFormField`), widget yöneticisi (`PDFWidgetFormManager`) ve form kontrolleri tamamlandı (`TASK-2026-007` done).
- M5 Redaction, Metadata ve Gizlilik: Gerçek kalıcı Redaction (`PDFRedact`), metaveri temizleme ve ek dosya yönetimi tamamlandı (`TASK-2026-008` done).
- M6 Gelişmiş Düzenleme ve Tipografi: Metin akışı düzenleme (`PDFDocumentTextFlowEditorModel`), FontCache ve görsel optimizasyon tamamlandı (`TASK-2026-009` done).
- M7 Kripto, Güvenlik ve Dijital İmza: AES-256 (Rev 6) şifreleme, OpenSSL PAdES imza doğrulama (`PDFSignatureHandler`) ve sertifika yönetimi tamamlandı (`TASK-2026-010` done).
- M8 OCR ve Taranmış Belge Desteği: Görsel ön işleme, Tesseract Türkçe OCR ve görünmez metin katmanı tamamlandı (`TASK-2026-011` done).

## Kabul edilen teknik rota

1. **PDF4QT v1.6.0.0**, VectorPDF'in açık kaynak çekirdek tabanıdır.
2. Ürün adı **VectorPDF** olarak geliştirilmektedir.

## Sonraki işler

1. PDF/A-1b, PDF/A-2b, PDF/A-3b doğrulama kurallarını (`PDFDocumentValidator`) yapılandır.
2. Raster görsel -> PDF (`PDFDocumentBuilder`) ve PDF -> raster görsel dönüştürme hattını doğrula.
3. Yapılandırılmış metin ve metaveri dışa aktarma araçlarını doğrula.
4. M10 Dağıtım ve Windows Entegrasyonu aşamasına geç.

## Aktif blokajlar

- Yok. Proje M9 dönüştürme ve standartlar geliştirme aşamasındadır.
