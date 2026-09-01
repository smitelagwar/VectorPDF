# Güncel Durum

**Tarih:** 2026-09-01  
**Faz:** M1 — P0 Güvenlik ve Veri Bütünlüğü  
**Kod durumu:** VectorPDF C++20 / Qt6 çekirdeği entegre edildi; DocumentSession, atomik kayıt ve güvenlik kontrolleri geliştiriliyor.

## Birincil aktif görev

[TASK-2026-004 — M1 Güvenlik ve Veri Bütünlüğü](docs/tasks/TASK-2026-004-m1-security-and-data-integrity.md)

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

## Kabul edilen teknik rota

1. **PDF4QT v1.6.0.0**, VectorPDF'in açık kaynak çekirdek tabanıdır.
2. Ürün adı **VectorPDF** olarak geliştirilmektedir.

## Sonraki işler

1. `DocumentSession` sınıfı: belge açık/kirli/imzalı durum yönetimini kur.
2. Güvenli atomik kayıt hattını (`QSaveFile` + flush + reopen validation) sertleştir.
3. Tehlikeli action (JavaScript, Launch) varsayılan deny güvenlik politikasını uygula.
4. M2 P0 Okuyucu ve Arayüz Kalitesi aşamasına geç.

## Aktif blokajlar

- Yok. Proje M1 güvenlik ve veri bütünlüğü geliştirme aşamasındadır.
