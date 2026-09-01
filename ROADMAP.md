# Gate Tabanlı Yol Haritası

**Durum:** Active 1.1  
**Son güncelleme:** 2026-09-01

Takvim değil kanıt yönetir. Bir fazın tarihi yaklaşınca gate gevşetilmez; kapsam küçültülür. `P0/P1/P2/P3`, öncelik sırasıdır; ürün sürüm numarası değildir.

## Gate 0 — ürün ve lisans anayasası

**Durum:** Tamamlandı — 2026-09-01 (`ADR-0001`, `ADR-0003`, `ADR-0004`)

**Amaç:** Yanlış lisans/marka seçimiyle bütün kod tabanını kilitlememek.

**Çıktılar**

- `ADR-0001`: permissive rota, root MIT, inbound MIT + DCO 1.1; M0'a kadar haricî katkı kapalı
- Belirlenen resmî ürün adı: **VectorPDF** (M0 öncesinde GitHub/domain/trademark ön taraması tamamlanır)
- `ADR-0003`: desteklenen Windows 11 x64 ve Store/MSIX-first dağıtım
- `ADR-0004`: offline-first, otomatik telemetry upload yok ve ücretsiz çekirdek sürdürülebilirliği
- Kaynak kabul/ret politikası

**Çıkış kapısı**

- Product owner başlangıç rota kararını devretmiş; Accepted ADR'ler ve root `LICENSE` ile bağlayıcı hale gelmiştir.
- Kök MIT ve DCO provenance politikası seçilmiş, katkı açılış kapısı yazılmıştır.
- Nihai adın Gate 1'i bloklamaması; M0 rebrand'dan önce product owner seçimi ve GitHub/domain/trademark ön taraması yapılması kayıt altındadır.
- Ücretsiz dağıtımın code-signing, Store ve bakım maliyeti risk siciline bağlanmıştır.

## Gate 1 — baseline bake-off ve fork kararı

**Durum:** Aktif — PDF4QT v1.6.0.0 tek birincil aday

**Amaç:** PDF4QT'nin iddia değil gerçek ürün tabanı olup olmadığını ölçmek; geçmezse permissive fallback rotasına dönmek.

**Çıktılar**

- Exact release/commit lock
- Dependency tree, SPDX lisansları, notice/provenance raporu
- Temiz Windows build runbook'u ve artifact hash'leri
- Gerçek unit/integration test raporu
- En az 25 dosyalık smoke corpus sonucu
- Forms/page ops/annotation/save-reopen davranışı; atomic-save/recovery için yalnız disposable fixture üzerinde feasibility spike ve gap planı (production implementation M1)
- Parser worker-isolation spike sonucu
- Bakım/merge/branding teknik borç raporu
- `ADR-0002` Accepted veya Rejected

**Çıkış kapısı**

- Belge bozma/veri kaybı yaratan P0 hata yoktur.
- Lisans/provenance blokajı yoktur.
- Build ve test en az iki temiz denemede tekrarlanır.
- Ana özellik senaryoları save → reopen → validate geçer.
- Atomic-save/recovery eksikse M1'de uygulanabilirliği vertical slice ile kanıtlanmış ve bu sürede kaynak üzerine save'i kapatan geçici koruma tanımlanmıştır.
- Worker ayrıştırması uygulanabilir veya kabul edilmiş, süreli mitigation vardır.

## M0 — upstream dondurma ve temiz rebrand

**Amaç:** Yeni özellik eklemeden sahibini bildiğimiz, tekrar üretilebilir ilk ürün tabanı.

**Çıktılar**

- Upstream remote + pinned base tag/commit + upstream sync politikası
- Belirlenen **VectorPDF** adı için GitHub/domain/trademark ön taraması ve yazılı onay
- Yeni ad, logo, package/app IDs, file associations, protocol IDs, installer publisher alanları
- Upstream trademark/assets temizliği; telif ve lisans metinleri korunmuş paket
- Lockfiles, `THIRD_PARTY_NOTICES`, ilk SBOM
- CI'da build + unit test + installer smoke
- “Vanilla baseline” golden ekran ve davranış kayıtları

**Kapsam dışı:** Yeni PDF özelliği, büyük UI yeniden yazımı, motor değiştirme.

**Çıkış kapısı:** Temiz VM'de install/open/uninstall; upstream baseline ile beklenmeyen davranış farkı yok.

## M1 — P0 güvenlik ve veri bütünlüğü

**Amaç:** Kullanıcı dosyasını kaybetmeyen ve düşmanca PDF'nin UI'yi ele geçirmesini zorlaştıran temel.

**Çıktılar**

- `DocumentSession`, dirty/revision/signed-state modeli
- Atomic save + fsync/flush + reopen/validate + replace
- Recovery journal ve crash-restart akışı
- UI dışı parse/render worker prototipi; Job Object limit/watchdog/cancel
- Dangerous actions deny policy
- Hassas içerik taşımayan structured diagnostics
- Hostile corpus smoke + timeout/OOM/crash beklentileri

**Çıkış kapısı:** Fault injection'da orijinal dosya byte-identical kalır; worker crash'i UI'yi düşürmez; recovery deterministik çalışır.

## M2 — P0 okuyucu kalitesi

**Amaç:** Günlük kullanımda hızlı, erişilebilir ve tahmin edilebilir okuyucu.

**Çıktılar**

- Open/recent/drag-drop; thumbnail/outline
- Smooth zoom/scroll/page virtualization; fit modes; rotate view
- Search/select/copy; match navigation
- Safe links; print; basic properties
- Keyboard-only, focus, high contrast, scaling ve Windows UI Automation
- Büyük dosya/açılış/render bellek bütçeleri

**Çıkış kapısı:** Desteklenen Windows VM matrisi ve büyük corpus'ta crash/hang yok; P50/P95 bütçeleri ölçülmüş ve kaydedilmiş.

## M3 — P0 sayfa işlemleri, annotation ve undo

**Amaç:** En sık ihtiyaçları güvenli mutasyon modeliyle sunmak.

**Çıktılar**

- Reorder/rotate/delete/duplicate/extract/insert/merge/split
- Highlight/underline/strikeout/free text/ink/shapes/stamp/comment
- Overlay text/image ve açıkça doğru adlandırılmış crop
- Command-based undo/redo; multi-document iş sınırları
- Autosave/recovery integration
- Preservation matrix: outline/link/tag/form/annotation/attachment/metadata/unknown objects

**Çıkış kapısı:** Her komut için round-trip + undo/redo property test; dokunulmayan sayfa ve yapıların fark bütçesi geçer.

## M4 — P0/P1 AcroForm

**Amaç:** Güvenilir form doldurma ve kontrollü form tasarımı.

**Çıktılar**

- Text/check/radio/combo/list/button/signature-field detection ve fill
- Appearance stream generation; font fallback/subset politikası
- Validation/required/read-only; tab order ve erişilebilir adlar
- Flatten yalnız ayrı kopya ve geri döndürülemez uyarısıyla
- Form tasarım seviye 1: alan ekle/taşı/boyutlandır/özellik
- XFA read-only/unsupported politika; JavaScript off

**Çıkış kapısı:** Acrobat/Edge ve seçili bağımsız okuyucularda görünüm + save/reopen; veri export/import round-trip; tag/label kaybı raporu.

## M5 — P1 tarama ve çevrimdışı OCR

**Amaç:** Türkçe/İngilizce taranmış PDF'yi aranabilir yapmak.

**Çıktılar**

- `IOcrProvider`; varsayılan Tesseract/Leptonica ayrı worker
- Pinned `tur+eng` model ve SHA-256/provenance
- 300 DPI kontrollü render, orientation/deskew/denoise pipeline
- Word bbox/polygon/confidence/language/model-version sonucu
- Görüntüyü koruyan görünmez text layer; low-confidence inceleme UI'si
- PaddleOCR yalnız opsiyonel gelişmiş layout/table PoC

**Çıkış kapısı:** Sabit Türkçe/İngilizce ground truth üzerinde CER/WER, bbox IoU, search/copy, P50/P95 ve RAM bütçesi; cloud upload yok.

## M6 — P1 gerçek redaction ve sanitize kopyası

**Amaç:** Üstüne kutu çizmek yerine geri getirilemeyen içerik kaldırma.

**Çıktılar**

- Mark-for-redaction + preview + explicit apply
- Text/image/vector ve ilgili metadata/attachment/history temizliği
- Full rewrite gerektiğinde açık uyarı; imza etkisi analizi
- Byte scan, extraction, render ve incremental-history testleri
- Sanitize işlemi ayrı kayıplı kopya + dönüşüm raporu

**Çıkış kapısı:** Eski içerik string/object/revision taramasında ve render/extract testlerinde geri alınamaz; “garantili güvenli” gibi mutlak pazarlama yok.

## M7 — P1/P2 içerik nesnesi düzenleme

**Amaç:** Acrobat benzeri edit kabiliyetini dürüst seviyelerle büyütmek.

**Seviye A:** Mevcut text object üzerinde sınırlı karakter değişimi; aynı font/encoding/geometri.  
**Seviye B:** Text box yerleştirme, font fallback, satır kırma ve basit layout.  
**Seviye C:** Paragraf reflow, complex script shaping, reading order/tag güncelleme. Ayrı Ar-Ge ve beta etiketi.

**Çıkış kapısı:** Font subset/glyph encoding, transforms, clipping, kerning/shaping, RTL/CJK/Türkçe, tag tree ve accessibility preservation fixture'ları. Desteklenmeyen belge için işlem engellenir veya rasterize/overlay gibi kayıplı seçenek açıkça adlandırılır.

## M8 — P2 PDF/A ve conformance dönüşümleri

**Amaç:** “Farklı Kaydet” değil, raporlu profil dönüşümü.

**Çıktılar**

- İlk hedef profil ADR ile seçilir; öneri PDF/A-4, gerekirse 4f
- Embedded font/color profile/metadata ve forbidden-feature policy
- Pinned veraPDF validation
- Dönüşüm öncesi/sonrası görsel + semantik fark ve kayıp raporu
- PDF/A + encryption çatışma kontrolü

**Çıkış kapısı:** Resmî/reference corpus + veraPDF geçişi; insan tarafından dönüşüm raporu onayı; tam uygunluk iddiası yalnız destek matrisi kanıtıyla.

## M9 — P2 dijital imza ve şifreleme

**Amaç:** Kriptografiyle güven hissi değil, açıklanabilir doğrulama.

**Sıra**

1. Signed PDF discovery ve değişiklik etkisi
2. Byte integrity + CMS crypto sonucu
3. Certificate chain/trust/revocation/timestamp ayrımı
4. Windows certificate store/CNG ile PAdES B
5. RFC 3161 ile T
6. LT/LTA yalnız uzun dönem validation ve attack corpus kanıtından sonra

Revocation/AIA/timestamp için genel worker ağı açılmaz: explicit policy/consent, endpoint ve SSRF kısıtlı TrustNetworkBroker; private key için non-exportable KeyBroker kullanılır. Offline sonuç “bilinmiyor” durumunu korur.

**Çıktılar:** AES-256 Revision 6 yeni yazma varsayılanı; eski şifrelemeyi yalnız okuma; anahtar/parola sıfırlama; Shadow/ISA/SWA ve çoklu revision testleri.

**Çıkış kapısı:** Tek yeşil “geçerli” etiketi yok; offline/expired/revoked/untrusted/post-sign-change sonuçları ayrı; bağımsız validator/Acrobat/ETSI testleri.

## M10 — P2 PDF/UA ve erişilebilirlik iyileştirme

**Amaç:** Uygulama erişilebilirliği ile üretilen belge erişilebilirliğini ayrı ama ciddi biçimde ele almak.

**Çıktılar**

- Structure tree/reading order/document language/ActualText/alt text/headings/lists/tables/forms/bookmarks
- Edit sırasında tag-preservation guard
- PDF/UA-1: validator + UA-1 reference suite + Matterhorn 1.1 insan maddeleri
- PDF/UA-2: pinned UA-2 validator profile + ISO 14289-2'ye dayalı ayrı insan/assistive-technology checklist'i
- Narrator/NVDA/keyboard/high contrast gerçek testleri

**Çıkış kapısı:** Her profile için ayrı otomatik validator + profile özgü insan checklist + assistive-technology sonucu; UA-1 kanıtı UA-2 için yeniden kullanılmaz ve yalnız makine geçişi ürün iddiası olmaz.

## M11 — P3 otomasyon, plugin ve isteğe bağlı AI

**Amaç:** Güvenilir temel bozulmadan ürünün genişlemesi.

**Çıktılar**

- Stable CLI/use-case API; batch queue/progress/cancel/retry
- Signed plugin manifest, capability permissions, process isolation, compatibility range
- Yerel AI için provider adapter; belge erişimi per-task consent; prompt injection/threat model
- AI feature eval seti ve offline fallback

**Çıkış kapısı:** Plugin/updater trust chain, revocation ve sandbox; AI kapalıyken bütün temel PDF özellikleri çalışır; belge varsayılan olarak dış servise gitmez.

## Sürekli hatlar

Her milestone şu işleri birlikte taşır:

- Upstream security/release watch ve dependency yenileme
- Corpus büyütme, fuzz crash minimization ve regression fixture
- SBOM/notices/provenance
- Performance, accessibility ve Turkish locale testleri
- Risk register/ADR/source freshness
- Backup/recovery ve installer/update smoke

## Sürümleme ilkesi

- `0.x`: API/format değişebilir; yine de belge veri bütünlüğü tavizsizdir.
- `1.0`: P0 okuyucu + sayfa + annotation + temel form + güvenli save/recovery tamamlanmış ve uzun süreli corpus kanıtı vardır.
- Profesyonel özellikler, hazır olmadan 1.0'ı bekletmek yerine ayrı capability maturity etiketi taşır: `experimental`, `preview`, `supported`, `conformant:<profile>`.
