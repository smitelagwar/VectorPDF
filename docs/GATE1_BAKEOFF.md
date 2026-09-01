# Gate 1 Baseline Bake-off Runbook

**Durum:** Active — PDF4QT v1.6.0.0  
**Amaç:** Ürün tabanını popülerlik, ekran görüntüsü veya README iddiasıyla değil aynı hukukî/teknik kanıtla seçmek.

## Adaylar

- **PDF4QT v1.6.0.0** tek kabul adayıdır; başarısızsa custom permissive fallback için ayrı vertical slice başlar.
- KillerPDF/GPLv3 mevcut ürün rotasının ve bu bake-off'un kapsamı dışındadır.
- Open PDF Studio/Stirling/ONLYOFFICE/MuPDF bu bake-off'ta baseline adayı değildir; lisans kararı teknik puanla aşılmaz.

## İzolasyon ve kayıt

- Adaylar ürün ağacına kopyalanmaz; ayrı araştırma worktree/VM'inde exact tag'den checkout edilir.
- Her checkout için tag'in dereference edilmiş commit'i ve recursive dependency/submodule durumu kaydedilir.
- Build cache hariç temiz VM snapshot kullanılır.
- Aday A'nın çıktısı aday B'nin klasörünü/cache'ini kullanmaz.
- Kullanılan OS image, compiler/SDK/Qt/CMake/vcpkg/.NET ve package lock hash'leri rapora yazılır.
- Her komut, exit code, duration ve artifact SHA-256 ile kaydedilir; secret/path/document content sanitize edilir.

## Work package 1 — lisans ve provenance

| Test | Kanıt | Pass |
|---|---|---|
| G1-LIC-001 | Exact revision root+nested license inventory | Belirsiz/çelişkili path yok |
| G1-LIC-002 | Direct/transitive runtime/build/data/model/font/asset tree | Her artifact source+SPDX+hash sahibi |
| G1-LIC-003 | Rebrand/trademark/attribution map | Upstream identity temizlenebilir, zorunlu notice korunur |
| G1-LIC-004 | Binary distribution obligations | Source/relink/notice paketi üretilebilir |
| G1-LIC-005 | Contributor/relicense provenance | Dağıtımı bloke eden unresolved hak sorunu yok |

PDF4QT için 27 Nisan 2025 öncesi commit contributor haritası ve Qt module license matrix zorunludur.

Her unresolved lisans hakkı **öldürücü failure**dır; teknik puan verilmez.

## Work package 2 — temiz build ve test

| Test | Kanıt | Pass |
|---|---|---|
| G1-BLD-001 | Upstream talimatıyla clean build | Exit 0; manual IDE click gerekmeyen runbook |
| G1-BLD-002 | İkinci temiz build | Aynı source/locks; beklenen unsigned payload farkı açıklı |
| G1-BLD-003 | Unit/integration test discovery | Test listesi ve gerçekten çalıştırılan runner |
| G1-BLD-004 | Installer/package | Standard user clean install/open/uninstall |
| G1-BLD-005 | Offline/restricted rebuild | Restore sonrası beklenmeyen network/latest yok |

PDF4QT upstream CI compile başarısı yeterli değildir; `ctest` veya gerçek test executable'ları çalıştırılır.

## Work package 3 — ortak corpus

İlk 25 fixture `tests/corpus/manifest.yml` içine exact hash ve license ile eklenir:

| Grup | Adet | Özellik |
|---|---:|---|
| Ordinary/text/fonts | 5 | Türkçe Unicode, subset/non-embedded, CJK/RTL smoke, PDF 1.7/2.0 |
| Page/outline/link | 3 | mixed boxes/rotation, outline destinations, safe URI |
| AcroForm | 4 | text/check-radio/combo, appearance edge case, readonly/required |
| Annotation | 3 | text markup, ink/free-text, replies/popups |
| Encryption | 2 | modern AES + legacy-read compatibility |
| Signature/revision | 2 | valid signed + post-sign modified/certified |
| Tagged/PDF-A | 2 | tag preservation + valid/invalid target profile |
| Scanned OCR input | 2 | Turkish/English, rotated/noisy/multi-column |
| Corrupt/hostile/large | 2 | recoverable corruption + bounded resource attack |

Fixture oracle source/spec'e bağlanır. Adaylar aynı renderer DPI/color/font ortamında sınanır.

## Work package 4 — kullanıcı işleri

Her senaryo screen recording değil makine okunur sonuç + gerekirse screenshot üretir.

### Reader

- G1-UX-001: cold open, first page, 100-page scroll, thumbnails
- G1-UX-002: Turkish search/select/copy; match navigation
- G1-UX-003: outline/internal link; external URI deny/confirm
- G1-UX-004: keyboard-only open/search/zoom/close ve high contrast

### Page operations

- G1-PAGE-001: reorder/rotate/delete/duplicate + undo/redo + save/reopen
- G1-PAGE-002: merge/split/extract; outline/link preservation
- G1-PAGE-003: crop/page boxes ve dokunulmayan sayfa diff

### Annotation

- G1-ANN-001: highlight/underline/strikeout
- G1-ANN-002: ink/free text/shape/comment; edit/delete
- G1-ANN-003: save/reopen ve cross-reader appearance

### Forms

- G1-FORM-001: text/check/radio/combo fill
- G1-FORM-002: appearance stream, Turkish font, readonly/required
- G1-FORM-003: save/reopen/cross-reader/export data
- G1-FORM-004: XFA detection ve execution-off behavior

### Save/recovery feasibility

- G1-SAVE-001: normal save-to-new-copy; source hash unchanged
- G1-SAVE-002: disk full/access denied/worker kill simulation
- G1-SAVE-003: external source modification race
- G1-SAVE-004: app/process crash ve recovery

Bu senaryolar Gate 1'de production implementation beklemez: mevcut davranışı ölçer ve disposable fixture üzerinde vertical-slice feasibility kanıtı üretir. Upstream'de atomic-save/recovery yoksa aday otomatik elenmeyebilir; fakat M1'de eklenebilirliği kanıtlanmalı ve production gate tamamlanana kadar kaynak üzerine varsayılan save kapatılmalıdır.

## Work package 5 — preservation

Her mutation öncesi/sonrası:

- structure/parser validation
- rendered affected + untouched pages
- extracted text/order
- page boxes/rotation
- outline/links/actions
- form values/appearance
- annotations/replies
- attachment list/hash
- metadata
- tag tree
- signature revisions/ByteRange
- unknown/private objects

karşılaştırılır. Sonuç `preserved`, `expected change`, `lossy with consent`, `blocked`, `unknown` olarak raporlanır. Unknown kritik yapıda save destekli sayılmaz.

## Work package 6 — hostile ve isolation spike

- Parser/render crash/hang/OOM ana UI'yi düşürüyor mu?
- Worker ayrı process'e çıkarılabiliyor mu; hangi global/UI coupling engelliyor?
- Job Object timeout/RAM/child process kontrolü uygulanabiliyor mu?
- Network/profile/registry/clipboard erişimi kısıtlanabiliyor mu?
- JavaScript/Launch/external/XFA/rich media build/runtime policy ile kapatılabiliyor mu?
- Worker çıktısı broker validation'dan önce kullanıcı yoluna ulaşabiliyor mu?

PoC tam production sandbox olmak zorunda değildir; fakat mimari olarak mümkün olduğunu ve en riskli coupling'i gösterir. Ana UI process'ten ayrılamayan unbounded native parser için Accepted security exception olmadan aday elenir.

## Work package 7 — bakım ve ürünleştirme

- G1-MNT-001: marka/app ID/installer/update endpoint'i tek patch setiyle değişir.
- G1-MNT-002: örnek küçük araç/use-case UI→domain→engine→test eklenir.
- G1-MNT-003: 20 değişmiş upstream dosyasından oluşan sentetik sync conflict ölçümü.
- G1-MNT-004: core/UI en büyük dosyalar, dependency cycles ve global state map.
- G1-MNT-005: security advisory/release response ve maintainer concentration.
- G1-MNT-006: crash symbol/debug ve release package üretilebilirliği.

Yeni örnek feature ürün branch'ine alınmaz; aday modülerliğini ölçmek içindir.

## Öldürücü kriterler

Aşağıdakilerden biri adayın toplam puanına bakılmadan `FAIL` verir:

- license/provenance/redistribution block
- temiz build'in iki kez tekrarlanamaması
- source corruption veya P0 silent data loss
- bounded fixture'da kontrolsüz code execution/network/child process
- hostile input'un UI'yi sistematik çökertmesi ve makul worker ayrışmasının gösterilememesi
- signed/tagged/form/attachment gibi kritik yapıyı fark etmeden kaydetme
- installer/update kimliğinin upstream'ten güvenli ayrılamaması

## Öldürücü kriter geçerse ağırlıklı skor

Bu ağırlıklar ürün önceliğidir, dış gerçek değildir:

| Alan | Ağırlık |
|---|---:|
| Veri bütünlüğü/preservation/save-recovery | 25 |
| Security/isolation/resource bounds | 20 |
| Lisans/provenance/marka sürdürülebilirliği | 15 |
| Build/test/package reproducibility | 15 |
| P0 kullanıcı workflow coverage | 10 |
| Bakım/modülerlik/upstream sync | 10 |
| Performance/accessibility/Windows UX | 5 |

Her alan 0–5 puan, `alan/5 × ağırlık`. Skorun yanında ham failure ve evidence zorunludur. Tek sonuç:

- `>= 80`: accept adayı; açık P1 riskler sahipli
- `65–79`: yalnız süreli remediation spike sonrası tekrar değerlendir
- `< 65`: reject

Security/lisans/veri bütünlüğü alanlarından herhangi biri 3'ün altındaysa toplam ne olursa olsun accept yoktur.

## Sonuç raporu

Her aday için:

- exact source/toolchain/locks
- license/provenance sonucu
- test command/exit/count/artifact hashes
- corpus pass/fail/unknown ve minimal repro
- performance P50/P95/peak
- preservation losses
- sandbox spike diagram ve kalan privilege
- rebrand/upstream patch map
- score + öldürücü kriter sonucu
- top 10 remediation ve tahmini replacement cost
- `ADOPT`, `REMEDIATE-AND-RETEST` veya `REJECT`

Rapor `ADR-0002` kanıtıdır. Ürün kodu yalnız ADR Accepted olduktan sonra exact baseline'dan içe alınır.
