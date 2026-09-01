# Playbook: PDF özelliği ekleme

1. Kullanıcı işini ve doğru teknik adı yaz: overlay mi content edit mi, visual mi crypto signature mı, mark mı applied redaction mı?
2. Stable REQ ID oluştur/bağla; kapsam ve exclusions belirle.
3. ISO/ETSI/resmî API kaynağını `SOURCES` kaydına bağla.
4. `PDF_SUPPORT_MATRIX`te mevcut read/render/create/edit/preserve/validate durumunu kaydet.
5. Input capability detection ve unsupported/lossy policy tasarla.
6. `DocumentCommand`, precondition, affected objects, inverse/checkpoint ve signed/tagged/PDF-A etkisini tanımla.
7. En az bir olumlu, olumsuz, malformed, preservation ve cross-reader fixture seç.
8. Unit + save/reopen + semantic/render diff + error/cancel/recovery testlerini yaz.
9. Security/privacy/accessibility/performance/license etkisini review et.
10. Kanıt olmadan maturity yükseltme. Destek yalnız tanımlı scope ile `SUPPORTED` olur.

Full rewrite, revision, imza, redaction, encryption, tag veya conformance etkisi varsa ADR/security review gerekir.
