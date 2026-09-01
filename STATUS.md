# Güncel Durum

**Tarih:** 2026-09-01  
**Faz:** M0 — VectorPDF baseline entegrasyonu ve yapılandırması  
**Kod durumu:** C++20 / Qt6 açık kaynak PDF çekirdeği VectorPDF olarak depoya aktarıldı.

## Birincil aktif görev

[TASK-2026-003 — M0 VectorPDF baseline entegrasyonu ve yapılandırması](docs/tasks/TASK-2026-003-m0-vectorpdf-baseline-setup.md)

Yeni bir AI oturumunda `plana devam et` denildiğinde önce bu dosya, sonra yukarıdaki görev kaydının `Devam checkpoint'i` bölümü okunur. Görev bağlantısı değişmedikçe ayrıntılı kaldığım-yer bilgisi görev dosyasındadır; bu bölüm aynı ayrıntıyı tekrar etmez.

## Tamamlananlar

- Beş referans rapor incelendi ve ortak ilkeler/çelişkiler çıkarıldı.
- `ADR-0001`: permissive rota ve root MIT Accepted.
- `ADR-0002`: PDF4QT v1.6.0.0 birincil açık kaynak kod tabanı olarak Accepted.
- `ADR-0003`: Windows 11 x64 ve Store/MSIX-first Accepted.
- `ADR-0004`: ücretsiz/açık çekirdek ve offline/account-free temel kullanım Accepted.
- Gate 1 bake-off tamamlandı (`TASK-2026-002` done).
- 25 dosyalık ortak smoke korpus manifesti `tests/corpus/manifest.yml` içine işlendi.
- C++20 / Qt6 açık kaynak kod tabanı depoya aktarıldı (`TASK-2026-003` active).

## Kabul edilen teknik rota

1. **PDF4QT v1.6.0.0**, VectorPDF'in açık kaynak çekirdek tabanıdır.
2. Ürün adı **VectorPDF** olarak geliştirilmektedir.

## Sonraki işler

1. CMakeLists.txt üzerinde VectorPDF proje tanımlarını ve hedeflerini yapılandır.
2. Windows 11 x64 derleme hattını (CMake/MSVC/Qt6) ve derleme rehberini hazırla.
3. M1 Güvenlik ve Veri Bütünlüğü (DocumentSession, atomik kayıt sertleştirme, izole worker mimarisi) aşamasına geç.

## Aktif blokajlar

- Yok. Proje M0 baseline geliştirme aşamasındadır.
