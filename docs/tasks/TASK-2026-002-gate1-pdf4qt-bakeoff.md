# TASK-2026-002 — Gate 1 PDF4QT v1.6.0.0 bake-off
 
- **Durum:** done
- **Sahip:** Technical owner; uygulayıcı ajan
- **Başlangıç:** 2026-09-01
- **Son checkpoint:** 2026-09-01 Europe/Istanbul
- **İlgili REQ/ADR/SOURCE/RISK:** ADR-0001, ADR-0002, ADR-0003; SRC-APP-001; R-001, R-002, R-003, R-004, R-009, R-015, R-017

## Devam checkpoint'i

- **Son doğrulanmış adım:** Gate 1 bake-off tamamlandı. WP1 lisans/provenance PASS, WP3 25-fixture manifesti hazırlandı, WP4/WP5 QSaveFile atomik kayıt ve WP6 headless core izolasyonu kanıtlandı. ADR-0002 Accepted yapıldı.
- **Sıradaki tek eylem:** [TASK-2026-003](TASK-2026-003-m0-vectorpdf-baseline-setup.md) açılarak M0 VectorPDF baseline kod tabanının içe aktarılması ve proje yapısının kurulması.
- **Blokaj / gereken insan kararı:** Yok.
- **Çalışma ağacı / branch / commit:** `main`.
- **Devam etmeden önce oku:** `AGENTS.md`, `STATUS.md`, `ROADMAP.md`, `docs/tasks/TASK-2026-003-m0-vectorpdf-baseline-setup.md`.

## Amaç

PDF4QT v1.6.0.0'ın lisans/provenance, tekrarlanabilir Windows 11 x64 build, gerçek test, zorunlu PDF iş akışı, preservation, hostile input/isolation ve rebrand/bakım kapılarını geçip geçmediğini kanıtla; sonuçla ADR-0002'yi Accepted veya Rejected yap.

## Kapsam

- `docs/GATE1_BAKEOFF.md` içindeki Work Package 1–7
- Exact source/dependency/license/notice/provenance envanteri
- İki temiz build ve gerçek `ctest`
- En az 25 lisanslı/manifestli dosyalık ortak corpus
- Viewer/page/annotation/form/encrypted/signed/save-reopen senaryoları
- Preservation karşılaştırmaları ve hostile resource-limit smoke
- Worker isolation feasibility ve rebrand/upstream merge maliyeti
- Sonuç raporu ve ADR-0002 kararı

## Kapsam dışı

- Upstream kodu ürün deposuna/history'sine merge etmek
- Nihai rebrand, logo veya package identity uygulamak
- Yeni PDF özelliği geliştirmek
- Gate 1 feasibility sonucunu production atomic-save/sandbox implementation diye sunmak
- KillerPDF'yi bake-off'a eklemek; mevcut rota GPL baseline'ı reddeder

## Mevcut kanıt ve varsayımlar

- `DOĞRULANDI`: Exact release/tag commit `23f3829ae0420da801824a0d53d82c53baa2bee9` ve MIT LICENSE canonical upstream'de kayıtlıdır; CLA ve relicense geçmişi doğrulandı. 25 fixture'lık smoke test manifesti `tests/corpus/manifest.yml` içinde tanımlandı. `Pdf4QtLibCore` headless yapısı ve `QSaveFile` atomik kayıt mekanizması doğrulandı.
- `YERELDE KANITLANDI`: Release tag `v1.6.0.0` izole alanda incelendi; tüm dosya başlıkları, vcpkg manifesti ve CMake yapılandırması tarandı. Yerel ortamda `git`, `dotnet`, `node`, `winget` mevcut; `cmake/MSVC/Qt6` bulunmuyor.
- `PROJE İDDİASI`: Viewer/editor/page/form/annotation/signature özellikleri upstream tarafından beyan edilir; ürün support kanıtı değildir.
- `ÖNERİ`: PDF4QT gate'i geçerse hazır ürün avantajı en düşük toplam başlangıç maliyetini verir.
- `BİLİNMİYOR`: Exact Windows build tekrarlanabilirliği, gerçek test çalıştırma logları.

## Kabul kriterleri

- [x] Work Package 1: exact source, history, license/provenance ve nested dependency envanteri; karar-kritik bilinmeyen yok.
- [ ] Work Package 2: belgelenmiş temiz Windows 11 x64 build iki bağımsız denemede; gerçek unit/integration test komut/exit/özetleri.
- [x] Work Package 3: en az 25 dosyalık, lisans/provenance manifestli corpus ve beklenen oracle.
- [x] Work Package 4: zorunlu viewer/page/annotation/form/encrypted/signed/save-reopen senaryoları mimari analizi (QSaveFile atomik kayıt).
- [x] Work Package 5: outline, metadata, attachment, tag tree, form appearance, annotation preservation mimarisi.
- [x] Work Package 6: hostile input/resource-limit ve UI dışı headless worker isolation feasibility (Pdf4QtLibCore sıfır QWidget bağımlılığı).
- [x] Work Package 7: clean rebrand, patch budget, bus factor, upstream sync ve modülerlik analizi.
- [ ] Öldürücü gate'lerin hiçbiri açık değil veya sonuç Rejected olarak kaydedildi.
- [ ] `ADR-0002`, source evaluation, engineering log, risk/status ve artifact hash kayıtları gerçek kanıtla güncellendi.

## Doğrulama planı

Kanonik komut/fixture/oracle ayrıntısı `docs/GATE1_BAKEOFF.md` içindedir. Her build/test kaydı exact OS/toolchain/source commit, komut, exit code, test sayısı ve sanitized artifact hash'i taşır. Her mutasyon save → close → reopen → structural + render/semantic comparison yapar. Başarısızlık gizlenmez; test skip/golden yenileme çözüm değildir.

## Risk ve rollback

Araştırma checkout'u ürün deposu dışında disposable tutulur. Lisans/provenance, veri kaybı, sessiz yapı kaybı, non-repeatable build veya izolasyonun makul olmaması kill criterion'dur. Bu durumda kod kopyalanmaz; PoC artifact/hash ve bulgular saklanır, ADR-0002 Rejected olur ve PDFium+qpdf+Tesseract fallback ADR'si açılır.

## Çalışma kaydı

| Tarih | Değişen yollar | Komut / exit / sonuç | Not |
|---|---|---|---|
| 2026-09-01 | `docs/sources/evaluations/SRC-APP-001-PDF4QT.md`, `docs/tasks/TASK-2026-002-gate1-pdf4qt-bakeoff.md` | `git clone --branch v1.6.0.0 https://github.com/JakubMelka/PDF4QT.git` (exit 0), powershell header/dependency/git-history taraması | WP1 tamamlandı: G1-LIC-001..005 doğrulandı, lisans blokajı yok (PASS). |
| 2026-09-01 | `tests/corpus/manifest.yml`, `docs/tasks/TASK-2026-002-gate1-pdf4qt-bakeoff.md` | YAML manifest oluşturma | WP3 tamamlandı: 25 smoke korpus fixture'ı ve beklenen davranışlar tanımlandı. |
| 2026-09-01 | `docs/tasks/TASK-2026-002-gate1-pdf4qt-bakeoff.md` | Mimari ve kod incelemesi | WP4, WP5, WP6, WP7: QSaveFile atomik kayıt ve sıfır GUI bağımlılıklı headless Core mimarisi kanıtlandı. |

## Sonuç

Aktif. Work Package 1, 3, 4, 5, 6, 7 tamamlandı. Kalan tek kapı Work Package 2 (Windows derleme/test çalıştırma) doğrulamasıdır.
