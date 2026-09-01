# Hedef Mimari

**Durum:** Proposed  
**Son güncelleme:** 2026-09-01  
**Bağlayıcı karar:** `ADR-0002` kabul edilene kadar teknoloji ayrıntıları değişebilir; aşağıdaki sınırlar değişmez.

## Mimari hedef

Çalışan upstream ürünü koruyarak, PDF gibi düşmanca ve kayıp riski yüksek bir formatı dar yetkili süreçlerde işlemek; bütün kullanıcı mutasyonlarını tek belge oturumu ve doğrulanabilir save pipeline'ı üzerinden geçirmek.

## Rota kararı

### Rota A — PDF4QT Gate 1'i geçerse

- UI: upstream Qt Widgets/Qt6 yapısı korunur; ilk aşamada UI framework rewrite yapılmaz.
- Dil/build: C++20, CMake, vcpkg; exact compiler/Qt/dependency pinleri.
- PDF motoru: PDF4QT core tek parser/writer/render sahibi olarak başlar.
- qpdf: yalnız bağımsız structure/repair/validation yardımcısı; aynı transaction'da writer olmaz.
- OCR: Tesseract/Leptonica ayrı worker ve adapter.
- İzolasyon: core/render servisleri aşamalı olarak UI dışı worker'a çıkarılır. Forkun motor/API sınırları önce dependency map ile çıkarılır.
- Upstream ile fark: küçük patch seti, adapter sınırları ve mümkün olan düzeltmeleri upstream'e gönderme.

### Rota B — PDF4QT Gate 1'i geçmezse

- Windows kabuğu için ayrı ADR: WPF/.NET LTS veya C++/Qt shell, 5 günlük vertical-slice PoC ile seçilir.
- Render/form/annotation: pinned PDFium C API worker.
- Structure/repair/encryption/optimization: pinned qpdf worker/adapter.
- OCR: pinned Tesseract/Leptonica worker.
- İleri writer/imza boşlukları: yalnız dar PoC ve lisans incelemesi sonrası PoDoFo MPL yolu veya PDFBox sidecar.
- PDF.js yalnız web tabanlı UI seçilirse; ikinci ana renderer olarak gereksiz yere eklenmez.

### Tarihsel alternatif: GPL rotası

Bu rota ADR-0001 ile mevcut ürün için reddedilmiştir ve uygulanmaz. İleride ADR-0001 açıkça supersede edilirse KillerPDF mevcut WPF/.NET 10 mimarisiyle yeniden değerlendirilebilir; GPL kararı mimari kalite kapılarını düşürmez.

## Mantıksal katmanlar

```text
┌──────────────────────────────────────────────────────────────┐
│ Windows UI                                                   │
│ View/ViewModel, accessibility, commands, progress/cancel     │
└──────────────────────────┬───────────────────────────────────┘
                           │ typed use-cases
┌──────────────────────────▼───────────────────────────────────┐
│ Application                                                  │
│ open/save/export, policy, capability checks, job queue        │
└──────────────────────────┬───────────────────────────────────┘
                           │ one authoritative session
┌──────────────────────────▼───────────────────────────────────┐
│ Document domain                                              │
│ DocumentSession, revision, commands, undo/redo, preservation │
└─────────────┬────────────────┬──────────────────┬────────────┘
              │                │                  │
       ┌──────▼──────┐  ┌──────▼──────┐   ┌──────▼──────┐
       │ PDF worker  │  │ OCR worker  │   │ Sign worker │
       │ parse/render│  │ images/text │   │ CMS/revision│
       │ single write│  │ no mutation │   │ no UI       │
       └──────┬──────┘  └─────────────┘   └─────────────┘
              │
       ┌──────▼─────────────────────────────────────────┐
       │ Save/recovery broker                            │
       │ temp, flush, validate, compare, atomic replace │
       └────────────────────────────────────────────────┘
```

Katman kuralı: UI motor tiplerini bilmez; domain Windows handle'ı veya Qt widget'ı bilmez; PDF/OCR/conversion/signature-parse worker'ları kullanıcı ayarlarını veya credential'ları görmez; altyapı ürün politikasına karar vermez. Sertifika deposu/private key ve revocation/timestamp ağı yalnız ayrı, dar yetkili broker'lar üzerinden kullanılır.

## `DocumentSession`

Her açık belge için tek otorite:

- `DocumentId`: yol değil, oturum kimliği
- `SourceFingerprint`: boyut + kontrollü hash + son yazma bilgisi
- `BaseRevision` ve aktif revision
- `DirtyState`, `SaveState`, `RecoveryState`
- imza/certification varlığı ve allowed-change politikası
- encryption/permission bilgisi; permission bayrağı DRM garantisi olarak sunulmaz
- capabilities: `supported`, `lossy-with-consent`, `blocked`, `unknown`
- açık views ve background jobs
- ordered `DocumentCommand` journal
- korunması gereken yapı snapshot'ı

Dosya disk üzerinde dışarıdan değişirse sessiz overwrite yapılmaz; üç-yollu çözüm veya “yeni kopya” gerekir.

## Komut, undo ve recovery

Kullanıcı mutasyonu `DocumentCommand` olarak tanımlanır:

```text
command_id
document_revision_before
operation + normalized parameters
affected_pages/objects
preconditions/capabilities
inverse or checkpoint strategy
result_revision
evidence/test hook
```

- UI event'i doğrudan PDF object graph'ını değiştirmez.
- Undo yalnız ekranda geri alma değil, document revision semantiğidir.
- Büyük operasyonlarda tam kopya yerine checkpoint + command journal kullanılabilir; correctness bellek tasarrufundan önce gelir.
- Recovery journal belge içeriğini plain text loglamaz; şifreli belge için gizli payload politikası ayrı ADR ister.
- Replay deterministik değilse recovery checkpoint'ten açılır ve kullanıcıya eksik işlemler gösterilir.

## Save state machine

```text
CLEAN
  └─command→ DIRTY
DIRTY
  └─save→ WRITING_TEMP
WRITING_TEMP
  └─flush success→ REOPENING
REOPENING
  └─parse success→ VALIDATING
VALIDATING
  ├─structure + required comparisons pass→ COMMITTING
  └─fail→ DIRTY + RECOVERY_AVAILABLE
COMMITTING
  ├─atomic replace success→ CLEAN
  └─fail→ DIRTY + TEMP_PRESERVED
```

Kurallar:

- Kaynak dosya write handle'ı çalışma süresince açık tutulmaz.
- Temp mümkünse aynı volume/directory'de, tahmin edilemez ad ve uygun ACL ile oluşturulur.
- `flush` uygulama buffer'ı + OS seviyesinde uygulanır; tam dayanıklılık davranışı Windows filesystem'lerinde fault-injection ile ölçülür.
- Doğrulama yalnız parser “açtı” değildir: qpdf/engine structure + required semantic + seçili render diff.
- Replace öncesi source fingerprint tekrar kontrol edilir.
- Backup/recovery retention kullanıcı politikasıdır; temp çöpü gizlice büyümez.

### Incremental ve full save

- Varsayılan, format/preservation testiyle kanıtlanan motor davranışıdır; “incremental her zaman daha güvenli” değildir.
- İmzalı belgede önce ByteRange/revision/certification policy incelenir.
- Redaction/sanitize için eski içeriği taşıyan incremental history kabul edilmez; full rewrite gerekir.
- PDF/A/conformance conversion yeni artifact üretir; kaynak üzerine yazmaz.

## Worker ve broker sınırı

### Worker görevleri

- Parse, object traversal, render, text extract
- Content mutation ve serialization
- OCR/preprocessing
- Dönüşüm/LibreOffice gibi optional sidecar
- Signature revision/CMS parsing ve kriptografik doğrulama; trust/network/key erişimi ayrı broker'dadır

### Broker görevleri

- Kullanıcı onayını ve ürün politikasını uygular.
- Worker'a path string yerine gereken en dar read handle/kopyayı verir.
- Çıktıyı karantina temp alanında alır; doğrulama sonrası commit eder.
- URI/attachment/print/clipboard gibi OS yetkili eylemleri worker adına doğrudan yapmaz.
- `TrustNetworkBroker`, yalnız M9 politikası izin verirse AIA/OCSP/CRL/RFC 3161 için endpoint/scheme/size/time kısıtlı istek yapar; PDF'nin genel ağ vekili değildir.
- `KeyBroker`, Windows certificate store/CNG/smart-card işlemini yapar; private key baytını UI'ye veya signature worker'a vermez, yalnız kontrollü sign operation sonucu döndürür.

### İzolasyon hedefi

1. Ayrı process + crash boundary
2. Job Object: RAM/CPU/time/child-process limitleri, kill-on-close
3. Düşük integrity/restricted token
4. Uygun olduğunda AppContainer/LPAC veya Windows App Isolation
5. PDF/OCR/conversion/signature-parse worker'larında network, user profile, registry, credential ve clipboard deny; Trust/Key broker'larında yalnız açık capability ve allowlist
6. DEP/ASLR/CFG/CET gibi uyumlu mitigations

MSIX tek başına bu sınırı sağlamaz. PoC gerçek Windows hedeflerinde çalışmalıdır.

## Engine adapter sözleşmesi

Her motor için tek tip yetenek bildirimi gerekir:

```text
OpenResult { document_id, capabilities, warnings, unsupported_features }
RenderRequest { revision, page, transform, clip, dpi, color_policy }
MutationPlan { preconditions, affected_objects, preservation_risks }
SaveResult { output_hash, validation, warnings, lost_features }
```

- Unsupported capability fallback ile gizlenmez.
- Engine error'ları kullanıcıya “bozuk dosya” diye genellenmeden category + safe detail ile çıkar.
- İki motorun render farkında majority vote doğru sayılmaz; spec ve fixture triage gerekir.

## OCR sınırı

`IOcrProvider` sonucu text string değildir:

- word/line/block
- polygon veya bbox
- confidence
- language
- orientation/coordinate transform
- engine/model version ve hash

Recognition ile searchable PDF text-layer writer ayrıdır. Varsayılan taranmış piksel değiştirilmez.

## İmza sınırı

- Signature image annotation, signature field, CMS crypto verification, certificate trust ve PAdES profile ayrı tiplerdir.
- Private key UI/domain/log katmanına çıkmaz; Windows certificate store/CNG/smart-card adapter'ı düşünülür.
- Revocation/AIA/timestamp için genel network yetkisi verilmez; TrustNetworkBroker kullanıcı/policy onayı, scheme/host/IP/redirect/size/time kontrolleriyle SSRF ve data exfiltration'a karşı sınırlandırılır. Belge içeriği broker'a gönderilmez.
- qpdf/PDFium tek başına hukuki/trust doğrulayıcı sayılmaz.
- Signed document mutation policy, command execute edilmeden önce çalışır.

## Repository hedef yapısı

Baseline seçildikten sonra upstream yapısını gereksiz yere bozmadan şu kavramsal alanlar oluşturulur:

```text
src/
  App/                 UI composition
  Application/         use-cases, policy, jobs
  Document/            session, commands, preservation
  Workers/
    Pdf/
    Ocr/
    Signature/
  Infrastructure/      filesystem, logging, update, packaging
tests/
  unit/
  integration/
  golden/
  corpus/              manifest only; hostile files outside normal Git
third_party/
docs/
```

İsimler fork diline göre uyarlanır. Mantıksal bağımlılık yönü korunur.

## Modüler Eklenti ve Servis Mimarisi (M12 & M13)

### 1. VectorPDFConversion (M12)
- İki yönlü dönüştürme: PDF'den Office (DOCX/XLSX/PPTX), metin, HTML5, EPUB, JSON ve WIA tarayıcı arayüzü.

### 2. VectorPDFDocumentTools (M13)
- **Foundation & Transactions:** `DocumentToolTransaction`, `DocumentToolValidator` (imzalı belge koruması ve geçici dosya doğrulaması).
- **Fill & Sign:** Çizim, metin ve görsel imza motoru, Windows DPAPI şifreli yerel depolama (`SignatureAssetStore`).
- **Page Decorations:** Header/Footer, Watermark, Background ve dinamik şablon motoru (`DecorationTemplateEngine`).
- **Content Tools:** Gelişmiş Bul/Değiştir, Türkçe yerel harf düzeni koruma (`İstanbul` -> `Ankara`), taşma heuristiği.
- **Smart Redact:** T.C. Kimlik (11 basamaklı sağlama), IBAN (ISO 13616 MOD-97), Kart (Luhn) ve PII algılayıcıları.
- **Flatten PDF:** Form ve açıklama düzleştirme; web köprülerini ve dijital imzaları koruyan filtreleme.

### 3. VectorPDFAccessibility (M13)
- **Accessibility Checker:** Belge başlığı, dil, tag ağacı, okuma sırası, tablo ve görsel kuralları.
- **Structure & Reading Order:** `/StructTreeRoot`, `/ParentTree`, `/MCID` yönetimi ve görsel okuma katmanı (`ReadingOrderOverlay`).
- **Auto-Tagging:** Yerel sezgisel analizci (`NativeHeuristicTagger`), Docling köprüsü ve yerel AI alternatif metin motoru.
- **Dürüst Doğrulama:** Harici `veraPDF` doğrulaması olmadan kesin PDF/UA uyumluluk iddiası verilmez.

## Mimari başarı ölçütleri

- Bir parser crash'i UI süreç kaybı yaratmaz.
- Bir save hatası kaynak belgenin hash'ini değiştirmez.
- Her kullanıcı mutasyonunun command/revision izi vardır.
- Bir dependency tek adapter sınırından değiştirilebilir; ürün politikası engine içinde dağılmaz.
- Capability/support iddiaları `PDF_SUPPORT_MATRIX` ve test ID'sine izlenebilir.
- Hassas belge içeriği log/crash dump/telemetride görülmez.
