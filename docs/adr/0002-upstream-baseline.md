# ADR-0002: Upstream baseline ve kabul kapısı

- **Durum:** Proposed — Gate 1 kanıtı gerekli
- **Tarih:** 2026-09-01
- **Karar sahibi:** Product + technical owner
- **Bağımlılık:** ADR-0001
- **İlgili riskler:** R-002, R-003, R-004, R-008, R-010, R-015, R-017

## Bağlam

Sıfırdan PDF motoru/uygulaması yazmak yerine çalışan açık kaynak taban isteniyor. Araştırmada en yakın permissive ürün PDF4QT, GPL rotasında ikinci aday KillerPDF; hazır ürünler elenirse en sağlam permissive motor bileşimi PDFium + qpdf + Tesseract olarak bulundu.

## Değerlendirilen seçenekler

### A. PDF4QT v1.6.0.0 fork

- MIT beyanı; Windows/Linux, C++20/Qt6.
- Viewer, Editor, PageMaster, Diff, CLI ve core/gui/widgets modülleri.
- Form, annotation, edit, encryption ve signature özellik iddiaları.
- Risk: tek geliştirici yoğunluğu, native parser saldırı yüzeyi, CI'da `ctest` kanıtı yok, known issues ve historical relicensing provenance.

### B. KillerPDF v1.8.2 fork

- .NET 10/WPF + ayrı GPLv3 engine.
- Yerelde current-main'de 1.716 test geçti.
- Risk: Nisan 2026'da doğmuş çok genç repo, GPL ürün kararı, büyük UI dosyaları, upstream testlerinin conformance/security garantisi olmaması.

### C. Custom shell + PDFium/qpdf/Tesseract

- Permissive, dar adapter ve sandbox için temiz sınırlar.
- Risk: çalışan ürün yerine ciddi UI/application/save orchestration geliştirme maliyeti.

### D. Open PDF Studio / Stirling / ONLYOFFICE / Sumatra / Okular

Lisans/provenance, kapsam veya mimari borç nedeniyle baseline değil; `SOURCES.md`de reference/watch/reject sınıfında.

## Önerilen karar

1. ADR-0001 permissive ise **PDF4QT v1.6.0.0 Gate 1'e tek birincil aday** olarak girer.
2. ADR-0001 permissive/MIT olarak Accepted olduğundan KillerPDF mevcut Gate 1 kapsamına girmez; davranış/test referansı olarak kalır.
3. PDF4QT öldürücü gate'i geçmezse C rotasına geçilir; Open PDF Studio/Stirling gibi kolay görünen ama lisansı sorunlu kodla boşluk kapatılmaz.
4. Gate 1 tamamlanmadan bu ADR `Accepted` yapılmaz ve upstream kod ana depoya merge edilmez.

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
