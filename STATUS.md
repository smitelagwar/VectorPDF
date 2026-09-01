# Güncel Durum

**Tarih:** 2026-09-01  
**Faz:** M5 — P1 Redaction, Metadata ve Gizlilik  
**Kod durumu:** VectorPDF C++20 / Qt6 çekirdeği devrede; okuyucu, güvenlik, sayfa işlemleri ve formlar tamamlandı; kalıcı redaction ve metaveri temizleme yönetiliyor.

## Birincil aktif görev

[TASK-2026-008 — M5 Redaction, Metadata ve Gizlilik](docs/tasks/TASK-2026-008-m5-redaction-and-metadata.md)

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

## Kabul edilen teknik rota

1. **PDF4QT v1.6.0.0**, VectorPDF'in açık kaynak çekirdek tabanıdır.
2. Ürün adı **VectorPDF** olarak geliştirilmektedir.

## Sonraki işler

1. Gerçek kalıcı Redaction (glif/piksel düzeyinde kalıcı silme) motorunu yapılandır.
2. Metaveri temizleme (Info, XMP, PieceInfo scrub) araçlarını doğrula.
3. Ek dosya (attachment) yönetimi ve taramasını doğrula.
4. M6 Gelişmiş Düzenleme ve Tipografi aşamasına geç.

## Aktif blokajlar

- Yok. Proje M5 redaction, metadata ve gizlilik geliştirme aşamasındadır.
