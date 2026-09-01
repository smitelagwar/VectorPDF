# Güncel Durum

**Tarih:** 2026-09-01  
**Faz:** M8 — P1 OCR ve Taranmış Belge Desteği  
**Kod durumu:** VectorPDF C++20 / Qt6 çekirdeği devrede; okuyucu, güvenlik, sayfa işlemleri, formlar, redaction, metin düzenleme ve kripto/imza tamamlandı; OCR motoru ve Türkçe metin katmanı yönetiliyor.

## Birincil aktif görev

[TASK-2026-011 — M8 OCR ve Taranmış Belge Desteği](docs/tasks/TASK-2026-011-m8-ocr-and-scanned-documents.md)

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

## Kabul edilen teknik rota

1. **PDF4QT v1.6.0.0**, VectorPDF'in açık kaynak çekirdek tabanıdır.
2. Ürün adı **VectorPDF** olarak geliştirilmektedir.

## Sonraki işler

1. Görsel ön işleme filtrelerini (Deskew, Otsu binarization, kontrast) yapılandır.
2. Tesseract OCR ve Türkçe dil modeli (`tur.traineddata`) entegrasyonunu doğrula.
3. Görünmez aranabilir metin katmanı (invisible searchable text overlay) yerleşimini doğrula.
4. M9 Dönüştürme, Dışa Aktarma ve Standartlar aşamasına geç.

## Aktif blokajlar

- Yok. Proje M8 OCR ve taranmış belge desteği geliştirme aşamasındadır.
