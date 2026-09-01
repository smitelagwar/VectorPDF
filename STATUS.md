# Güncel Durum

**Tarih:** 2026-09-01  
**Faz:** M3 — P0 Sayfa İşlemleri, Annotation ve Geri Alma  
**Kod durumu:** VectorPDF C++20 / Qt6 çekirdeği devrede; okuyucu ve güvenlik temeli tamamlandı; sayfa işlemleri, açıklamalar ve Undo/Redo yönetiliyor.

## Birincil aktif görev

[TASK-2026-006 — M3 Sayfa İşlemleri, Annotation ve Geri Alma](docs/tasks/TASK-2026-006-m3-page-ops-annotations.md)

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

## Kabul edilen teknik rota

1. **PDF4QT v1.6.0.0**, VectorPDF'in açık kaynak çekirdek tabanıdır.
2. Ürün adı **VectorPDF** olarak geliştirilmektedir.

## Sonraki işler

1. Sayfa manipülasyonu (döndürme, silme, ekleme, bölme, birleştirme) akışlarını yapılandır.
2. Açıklama araçlarını (vurgulama, altı çizili, serbest metin, damga, şekiller) doğrula.
3. Command-based Undo/Redo mekanizmasını ve veri koruma matrisini doğrula.
4. M4 P0/P1 AcroForm ve Form Tasarımı aşamasına geç.

## Aktif blokajlar

- Yok. Proje M3 sayfa işlemleri ve açıklama geliştirme aşamasındadır.
