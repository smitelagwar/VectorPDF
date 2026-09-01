# ADR-0002: Upstream baseline ve kabul kapısı

- **Durum:** Accepted
- **Tarih:** 2026-09-01
- **Karar sahibi:** Product + technical owner
- **Bağımlılık:** ADR-0001
- **İlgili riskler:** R-002, R-003, R-004, R-008, R-010, R-015, R-017

## Bağlam

Sıfırdan PDF motoru/uygulaması yazmak yerine çalışan açık kaynak taban istenmektedir. Gate 1 bake-off sürecinde PDF4QT v1.6.0.0 exact commit `23f3829ae0420da801824a0d53d82c53baa2bee9` üzerinde lisans/provenance (WP1), 25 fixture smoke korpusu (WP3), `QSaveFile` atomik kayıt mekanizması (WP4/WP5), sıfır QWidget GUI bağımlılıklı headless Core mimarisi (WP6) ve modüler CMake yapısı (WP7) incelenmiş ve doğrulanmıştır.

## Değerlendirilen seçenekler

### A. PDF4QT v1.6.0.0 (Seçildi)

- C++20 / Qt6 tabanlı hazır çalışan açık kaynak sistem.
- Viewer, Editor, PageMaster, Diff, CLI ve core/gui/widgets mimarisi.
- Lisans (MIT) ve Qt6 (LGPLv3) uyumluluğu kanıtlandı.
- Headless `Pdf4QtLibCore` yapısı sayesinde izole worker süreçlerine ayrıştırılabilirliği doğrulandı.

### B. KillerPDF v1.8.2

- .NET 10/WPF tabanlı açık kaynak PDF motoru ve davranış/test referansı.

### C. Custom shell + PDFium/qpdf/Tesseract

- İhtiyaç duyulduğunda ek motor ve OCR bileşenleri.

## Karar

1. **PDF4QT v1.6.0.0**, Gate 1 teknik ve hukuki kapılarını geçmiş olup VectorPDF ürününün çekirdek açık kaynak tabanı olarak **Accepted** edilmiştir.
2. Ürün adı **VectorPDF** olarak belirlenmiştir.
3. Proje fazı **M0 (VectorPDF Baseline Entegrasyonu ve Yapılandırması)** aşamasına geçirilmiştir.

## Gate 1 acceptance

- Exact tag/commit/dependency/license/provenance envanteri
- Clean Windows build iki kez
- Upstream unit/integration testleri gerçekten çalışır
- 25 dosyalık ortak smoke corpus
- Save/reopen/preservation ve signed-file policy
- Hostile resource-limit smoke
- Worker isolation feasibility spike
- Rebrand/install/update identity audit
- Upstream merge/patch budget değerlendirmesi

Öldürücü failure: veri kaybı, sessiz önemli yapı kaybı, license block, repeatable build yokluğu, unbounded hostile crash/hang veya worker separation'ın makul biçimde yapılamaması.

## Uygulama sonucu

Geçerse:

- `upstream` remote ve immutable baseline tag
- temiz rebrand ayrı commit
- yeni özellikten önce M0/M1
- dependency update olarak kontrollü upstream sync

Kalırsa:

- PoC sonucu ve artifact hash'leri kaydedilir;
- kod ürün history'sine alınmaz veya deneysel branch arşivlenir;
- fallback shell için ayrı ADR açılır.

## Rollback

M0/M1 sırasında baseline kabul varsayımı bozulursa marka/application katmanındaki taşınabilir gereksinimler ve corpus korunur; engine-specific kod adapter arkasından kaldırılır. Kullanıcı dosya formatı veya plugin ABI 1.0 öncesi sabitlenmediği için migration borcu sınırlanır.

## Yeniden değerlendirme tetikleyicileri

- Critical upstream CVE ve kabul edilemez response
- Lisans veya contributor provenance değişimi
- İki ardışık upstream sync'in patch bütçesini aşması
- Corpus'ta sistematik preservation/conformance failure
- Windows packaging/toolchain sürdürülemezliği

## Kaynaklar

- [PDF4QT repo](https://github.com/JakubMelka/PDF4QT)
- [PDF4QT notes](https://github.com/JakubMelka/PDF4QT/blob/master/NOTES.txt)
- [KillerPDF repo](https://github.com/SteveTheKiller/KillerPDF)
- [PDFium](https://pdfium.googlesource.com/pdfium/)
- [qpdf](https://github.com/qpdf/qpdf)
- [Tesseract](https://github.com/tesseract-ocr/tesseract)
