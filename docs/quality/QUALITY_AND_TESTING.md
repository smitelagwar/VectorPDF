# Kalite ve Test Stratejisi

**Durum:** Active başlangıç politikası  
**Son güncelleme:** 2026-09-01

## Kalitenin tanımı

PDF programında kalite yalnız render güzelliği değildir:

- belge veri bütünlüğü ve preservation
- güvenlik ve kaynak tüketimi
- format/standard uyumu
- başka okuyucularla birlikte çalışabilirlik
- crash recovery ve açıklanabilir hata
- performans ve uzun oturum kararlılığı
- UI ve çıktı erişilebilirliği
- gizlilik ve supply-chain güveni
- sürdürülebilir, test edilebilir mimari

## Feature kanıt zinciri

`REQ → source/spec → ADR → implementation → fixture/test → recorded result → support matrix`

Zincir yoksa özellik `experimental` veya `unknown`dur. Upstream checkbox'ı destek kanıtı değildir.

## Corpus yönetimi

Normal Git'e büyük veya saldırgan PDF konmaz. `tests/corpus/manifest.yml` artifact kaydını tutar; download/cache aracı hash doğrular ve zararlı örnekleri ayrı karantina klasörüne koyar.

Her kayıt:

- stable `id`
- canonical upstream URL
- pinned version/commit
- SHA-256
- lisans ve license evidence
- beklenen davranış/oracle
- malicious flag ve safety note
- boyut ve acquisition method
- hangi REQ/testleri kapsadığı

içerir. Rastgele kullanıcı PDF'si açık rıza, redaction ve lisans olmadan corpus değildir.

### Corpus sınıfları

1. Basit PDF 1.x/2.0, xref table/stream, object stream
2. Bozuk/recoverable ve unrecoverable dosyalar
3. Şifreli, permission flag ve farklı crypto revision
4. Fontlar: subset, embedded/non-embedded, Type 0/1/3, Türkçe, CJK, RTL, complex script
5. Büyük sayfa, çok sayfa, yüksek DPI/image ve compression bomb
6. AcroForm alanları, appearance varyantları, XFA detection
7. Annotation türleri, replies/popups/layers
8. Outline/link/action/attachment/optional content
9. Tagged PDF/PDF-UA, reading order, alt text
10. PDF/A profile olumlu/olumsuz örnekleri
11. Signed/certified/multiple revision, Shadow/ISA/SWA attack samples
12. Scanned, rotated, skewed, noisy, multi-column Turkish/English OCR
13. Redaction ve incremental-history leakage
14. Unknown/private extension preservation

Başlıca upstream kaynaklar `SOURCES.md` içindeki `SRC-QA-*` kayıtlarıdır.

## Test katmanları

### 1. Unit ve property tests

- object/xref parse ve serialization
- coordinate transforms/page boxes/rotation
- command/inverse/undo/redo
- revision state machine
- atomic-save path/fault cases
- form value/appearance mapping
- text/OCR coordinate mapping
- path canonicalization ve URI policy

Property tests random operation sequences sonrası invariant'ları sınar: reopen mümkün, command count/revision tutarlı, undo başlangıca döner, source değişmez.

### 2. Component/worker contract

- exact protocol/version negotiation
- timeout/cancel/crash/OOM
- malformed request ve oversized payload
- no-network/no-child-process enforcement
- sanitized error/log
- output hash ve capability negotiation

### 3. Round-trip/preservation

Her mutasyon için save → close → reopen → validate. Şunların beklenen korunması ölçülür:

- dokunulmayan page content ve render
- unknown objects/extensions
- tags/reading order
- form fields/appearance
- annotations/replies
- outline/links/actions
- attachments/metadata
- encryption ve permissions
- signature revision/ByteRange

Preservation matrisinde “korunur”, “bilinen kayıp + onay”, “engellenir” veya “unknown” vardır.

### 4. Golden visual

- Sabit renderer/version, font seti, color profile, DPI, locale ve OS image.
- Exact pixel yalnız deterministik alanlarda; antialias farkı için açık perceptual threshold.
- Baseline değişimi bağımsız gözle inceleme + gerekçe ister.
- AI veya test yazarı kendi output'unu otomatik golden olarak onaylamaz.

### 5. Semantic differential

- text spans/order/Unicode
- page boxes/rotation
- object/category counts
- metadata/outline/attachments
- forms/annotations/tags
- signed revisions

PDFium/PDF.js/MuPDF/Poppler/Acrobat karşılaştırması yardımcıdır; majority vote normatif doğru değildir. Fark spec'e bağlanarak triage edilir.

### 6. Conformance

- PDF syntax/structure validator
- PDF/A için pinned veraPDF ve profile fixture
- PDF/UA-1 için profile validator + PDF/UA-1 reference suite + Matterhorn 1.1 insan maddeleri
- PDF/UA-2 için pinned UA-2 validator profile + ISO 14289-2'ye dayalı ayrı insan/assistive-technology kontrolleri; UA-1 suite sonucu yeniden kullanılmaz
- PAdES için ETSI prosedürü + independent implementation/attack corpus

Bir validator pass'i görsel doğruluğu, semantic preservation veya hukuki imza sonucunu tek başına kanıtlamaz.

### 7. Fuzz ve hostile

- Parser, xref/object stream, image/font decoder, form/annotation, serializer, OCR input
- Native kodda ASan/UBSan uygun build; Windows hedefte AppVerifier/mitigation smoke gerekirse
- Crash, hang, timeout ve OOM test başarısızlığıdır.
- Minimize edilmiş crash fixture + regression test + upstream disclosure kaydı olur.

### 8. UI/E2E

- keyboard-only ve screen reader
- high contrast, 100/150/200% scaling, multi-monitor
- Türkçe/İngilizce locale ve path characters
- file association, drag/drop, recent list, print
- unsaved close, disk full, access denied, external modification, process kill/recovery
- clean install/update/rollback/uninstall

### 9. Cross-reader smoke

Çıktı en az seçili Adobe Reader, Edge ve Firefox/Chrome/PDF viewer sürümlerinde açılır. Sürümler release evidence'a yazılır; “Acrobat'ta açıldı” standard compliance değildir.

## Gate 1 bake-off test paketi

### 25 dosyalık minimum smoke

- 5 ordinary/text/font varyantı
- 3 page ops/outline/link
- 4 AcroForm/appearance
- 3 annotation
- 2 encrypted
- 2 signed/certified
- 2 tagged/PDF/A
- 2 scanned Turkish/English
- 2 corrupt/hostile/large

Her adayda aynı scripted kullanıcı işleri ve aynı oracle kullanılır. Sonuç `pass/fail/blocked/unknown` + artifact hash ile kaydedilir.

### Öldürücü hatalar

- source file corruption/data loss
- silent loss of signature/tag/form/annotation/attachment without policy
- arbitrary code execution/network access
- unbounded hang/OOM on bounded fixture
- license/provenance block
- clean build veya testin tekrarlanamaması

## Performance bütçeleri

İlk absolute değerler Gate 1 benchmark donanımı ve corpus ile belirlenir; hayalî sayı yazılmaz. Her ölçüm:

- hardware/OS/power mode
- cold/warm cache
- file hash/pages/size/features
- build/config/commit
- P50/P95/max ve peak working set

taşır.

İzlenen metrikler:

- cold launch ve ilk sayfa görünme
- next-page render latency
- search first/all result
- thumbnail population
- large-file scroll frame pacing
- save/reopen/validate
- OCR page throughput
- idle/active/large-document RAM
- worker crash recovery
- installer/update size/time

Baseline sonrası her milestone ratchet budget belirler. Bütçe aşımı düzeltme, açık trade-off ADR'si veya feature scope azaltımı gerektirir.

## Severity

- **P0:** Veri kaybı/bozulma, security/privacy ihlali, yanlış imza/redaction sonucu, update compromise, yaygın crash. Release bloklanır.
- **P1:** Ana workflow çalışmıyor, ciddi compatibility/accessibility/performance regression. Normalde release bloklanır.
- **P2:** Workaround'lı kusur, sınırlı format/UX sorunu. Risk açık ve triage edilmiş olabilir.
- **P3:** Kozmetik/iyileştirme.

Issue sayısı değil kullanıcı etkisi, veri/güven riski ve tekrar oranı önceliklidir.

## Definition of Done

Bir değişiklik tamam değildir, eğer:

- kabul kriteri ve relevant REQ yoksa;
- test fixture/oracle yok veya çalıştırılmadıysa;
- save/reopen/preservation kanıtı gerekiyorsa ama yoksa;
- security/privacy/license/accessibility etkisi değerlendirilmediyse;
- error/cancel/recovery yolu yoksa;
- docs/support matrix/source/ADR gerektiği halde güncellenmediyse;
- build/SBOM/notices kırıldıysa;
- rollback yolu bilinmiyorsa.

## Release kalite kapısı

- P0/P1 açık hata yok veya P1 için açık, kullanıcıyı koruyan kabul edilmiş istisna
- full unit/integration/golden/hostile/conformance-relevant test yeşil
- supported Windows clean install/update/uninstall
- SBOM/notices/provenance/signatures doğrulanmış
- malware scan ve dependency advisory review
- performance/accessibility regression review
- backup/recovery/fault-injection smoke
- release notes capabilities'i abartmıyor
- rollback artifact ve procedure hazır

Flaky test mute edilmez; owner, neden, quarantine süresi ve kaldırma kriteriyle kayıt altına alınır.
