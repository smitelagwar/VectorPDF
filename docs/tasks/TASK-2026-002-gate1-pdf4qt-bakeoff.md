# TASK-2026-002 — Gate 1 PDF4QT v1.6.0.0 bake-off

- **Durum:** active
- **Sahip:** Technical owner; uygulayıcı ajan değişebilir
- **Başlangıç:** 2026-09-01
- **Son checkpoint:** 2026-09-01 Europe/Istanbul
- **İlgili REQ/ADR/SOURCE/RISK:** ADR-0001, ADR-0002, ADR-0003; SRC-APP-001; R-001, R-002, R-003, R-004, R-009, R-015, R-017

## Devam checkpoint'i

- **Son doğrulanmış adım:** Gate 0 permissive/MIT, Windows 11 x64, Store/MSIX-first ve ücretsiz/offline çekirdek kararları Accepted ADR'lerle sabitlendi. PDF4QT v1.6.0.0 tek Gate 1 adayıdır; ürün kodu henüz alınmadı.
- **Sıradaki tek eylem:** `docs/GATE1_BAKEOFF.md` Work Package 1'i uygula: PDF4QT exact `v1.6.0.0` / `23f3829ae0420da801824a0d53d82c53baa2bee9` için license/provenance ve bütün doğrudan/nested dependency envanterini ürün deposu dışında salt araştırma checkout'unda çıkar; kalıcı değerlendirmeyi `docs/sources/evaluations/SRC-APP-001-PDF4QT.md` içinde güncelle.
- **Blokaj / gereken insan kararı:** Yok. Lisans/provenance kırmızı bulgusu çıkarsa teknik PoC'u durdur ve karar iste; lisans riski puanla geçilemez.
- **Çalışma ağacı / branch / commit:** `main`; upstream ürün kodu yok. Önce başlangıç plan commit'ini doğrula. Araştırma clone'u bu ürün ağacı/history'si dışında olmalı.
- **Devam etmeden önce oku:** `AGENTS.md`, `STATUS.md`, `.agents/skills/pdf-dependency-audit/SKILL.md`, `docs/GATE1_BAKEOFF.md`, `docs/adr/0001-product-license-model.md`, `docs/adr/0002-upstream-baseline.md`, `docs/sources/evaluations/SRC-APP-001-PDF4QT.md`, `docs/LICENSING.md`, `docs/SECURITY_AND_PRIVACY.md`.

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

- `DOĞRULANDI`: Exact release/tag commit ve MIT LICENSE canonical upstream'de kayıtlıdır.
- `YERELDE KANITLANDI`: Önceki kaynak incelemesinde PDF4QT current-main tarandı; yerel CMake bulunmadığı için build/test çalıştırılmadı.
- `PROJE İDDİASI`: Viewer/editor/page/form/annotation/signature özellikleri upstream tarafından beyan edilir; ürün support kanıtı değildir.
- `ÖNERİ`: PDF4QT gate'i geçerse hazır ürün avantajı en düşük toplam başlangıç maliyetini verir.
- `BİLİNMİYOR`: Exact Windows build tekrarlanabilirliği, gerçek test sonucu, historical relicense provenance, preservation ve isolation uygulanabilirliği.

## Kabul kriterleri

- [ ] Work Package 1: exact source, history, license/provenance ve nested dependency envanteri; karar-kritik bilinmeyen yok.
- [ ] Work Package 2: belgelenmiş temiz Windows 11 x64 build iki bağımsız denemede; gerçek unit/integration test komut/exit/özetleri.
- [ ] Work Package 3: en az 25 dosyalık, lisans/provenance manifestli corpus ve beklenen oracle.
- [ ] Work Package 4: zorunlu viewer/page/annotation/form/encrypted/signed/save-reopen senaryoları ölçüldü.
- [ ] Work Package 5: outline, metadata, attachment, tag tree, form appearance, annotation, unknown objects ve signature revision preservation matrisi.
- [ ] Work Package 6: hostile input/resource-limit sonucu ve UI dışı worker isolation feasibility spike.
- [ ] Work Package 7: clean rebrand, patch budget, bus factor, upstream sync ve replacement maliyeti.
- [ ] Öldürücü gate'lerin hiçbiri açık değil veya sonuç Rejected olarak kaydedildi.
- [ ] `ADR-0002`, source evaluation, engineering log, risk/status ve artifact hash kayıtları gerçek kanıtla güncellendi.

## Doğrulama planı

Kanonik komut/fixture/oracle ayrıntısı `docs/GATE1_BAKEOFF.md` içindedir. Her build/test kaydı exact OS/toolchain/source commit, komut, exit code, test sayısı ve sanitized artifact hash'i taşır. Her mutasyon save → close → reopen → structural + render/semantic comparison yapar. Başarısızlık gizlenmez; test skip/golden yenileme çözüm değildir.

## Risk ve rollback

Araştırma checkout'u ürün deposu dışında disposable tutulur. Lisans/provenance, veri kaybı, sessiz yapı kaybı, non-repeatable build veya izolasyonun makul olmaması kill criterion'dur. Bu durumda kod kopyalanmaz; PoC artifact/hash ve bulgular saklanır, ADR-0002 Rejected olur ve PDFium+qpdf+Tesseract fallback ADR'si açılır.

## Çalışma kaydı

| Tarih | Değişen yollar | Komut / exit / sonuç | Not |
|---|---|---|---|

## Sonuç

Aktif. İlk iş lisans/provenance ve dependency envanteridir; build araç kurulumu bu audit temizlenmeden başlamaz.
