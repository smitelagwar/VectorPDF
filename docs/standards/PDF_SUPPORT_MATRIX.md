# PDF Destek ve Koruma Matrisi

**Durum:** Gate 1 öncesi — hiçbir upstream iddiası ürün desteği sayılmaz  
**Son güncelleme:** 2026-09-01

## Maturity

- `UNKNOWN`: Henüz fixture/kanıt yok.
- `EXPERIMENTAL`: Geliştirici PoC'u; veri kaybı olabilir, varsayılan kapalı.
- `PREVIEW`: Belirli corpus'ta geçer, support sınırı görünür.
- `SUPPORTED`: Tanımlı scope, regression corpus ve release gate mevcut.
- `CONFORMANT:<profile/version>`: Normatif profil + validator/insan/interop kanıt paketi mevcut.
- `BLOCKED`: Bilinçli olarak izin verilmiyor.

“Açar” ile “görüntüler”, “düzenler”, “kaydederken korur” ve “standardı doğrular” ayrı hücrelerdir.

## Başlangıç matrisi

| Yetenek | Aç/parse | Görüntüle | Oluştur | Düzenle | Save'de koru | Doğrula | İlk kanıt hedefi | Durum |
|---|---|---|---|---|---|---|---|---|
| PDF 1.4–1.7 temel | Gate 1 | Gate 1 | Gate 1 | Sınırlı | Gate 1 | Structure | ordinary corpus | UNKNOWN |
| PDF 2.0 temel | Gate 1 | Gate 1 | Sonra | Sınırlı | Gate 1 | ISO feature map | pdf20examples | UNKNOWN |
| xref stream/object stream | Gate 1 | N/A | Gate 1 | N/A | Gate 1 | structure | syntax corpus | UNKNOWN |
| Linearized PDF | Gate 1 | Gate 1 | P2 | N/A | preservation | qpdf/engine | web/large fixture | UNKNOWN |
| Incremental revisions | Gate 1 | N/A | M9 | command-specific | Gate 1 | revision diff | signed/multi-rev corpus | UNKNOWN |
| Text extraction/search | Gate 1 | UI | N/A | N/A | N/A | semantic | Turkish/CJK/RTL corpus | UNKNOWN |
| Overlay text/image | Gate 1 | Gate 1 | M3 | own objects | M3 | render/object | round-trip | UNKNOWN |
| Existing text object edit | Gate 1 | Gate 1 | N/A | M7-A/B/C | M7 | semantic/render | font/layout corpus | UNKNOWN |
| Page operations | Gate 1 | Gate 1 | M3 | M3 | M3 | semantic/render | page corpus | UNKNOWN |
| Annotations | Gate 1 | Gate 1 | M3 | M3 | M3 | object/render | annotation corpus | UNKNOWN |
| AcroForm | Gate 1 | Gate 1 | M4 | fill/design L1 | M4 | value/appearance | form corpus | UNKNOWN |
| XFA | Detect | Static read maybe | BLOCKED | BLOCKED | preserve only if proven | detect | XFA fixtures | BLOCKED execution |
| JavaScript/actions | Detect | BLOCKED exec | BLOCKED | BLOCKED | preserve/remove policy | detect | hostile action corpus | BLOCKED execution |
| Attachments | Gate 1 | metadata | M3 | add/remove M3 | M3 | hash/list | attachment corpus | UNKNOWN |
| Optional content/layers | Gate 1 | Gate 1 | P2 | P2 | Gate 1 | object/render | OCG corpus | UNKNOWN |
| Encryption legacy | Gate 1 | Gate 1 | BLOCKED new | policy | Gate 1 | crypto params | crypto corpus | UNKNOWN read |
| AES-256 Revision 6 | Gate 1 | Gate 1 | M9 | M9 | M9 | interop/object | crypto corpus | UNKNOWN |
| Visual signature image | Gate 1 | Gate 1 | M3 overlay | M3 | M3 | render | ordinary fixture | UNKNOWN |
| Digital signature inspect | Gate 1 | appearance | N/A | N/A | signed policy | M9 | revision corpus | UNKNOWN |
| PAdES B/T | Gate 1 detect | appearance | M9 | N/A | incremental policy | M9 | ETSI + interop | UNKNOWN |
| PAdES LT/LTA | Detect maybe | appearance | Post-M9 | N/A | strict | Post-M9 | ETSI/trust | DEFERRED |
| Redaction marks | Gate 1 | Gate 1 | M6 | M6 | M6 | mark objects | redaction corpus | UNKNOWN |
| Applied redaction | Gate 1 | Gate 1 | M6 | irreversible | full rewrite | M6 attacks | recovery tests | UNKNOWN |
| PDF/A-4 | Gate 1 | Gate 1 | M8 conversion | constrained | M8 | veraPDF + diff | corpus | UNKNOWN |
| PDF/UA-1 tagged content | Gate 1 | Gate 1 | M10 | remediation M10 | preservation early | M10 | UA-1 suite + Matterhorn 1.1 + AT | UNKNOWN |
| PDF/UA-2 tagged content | Gate 1 | Gate 1 | M10 | remediation M10 | preservation early | M10 | UA-2 validator profile + ISO-based human/AT | UNKNOWN |
| OCR searchable layer | N/A | source render | M5 | correction | M5 | CER/WER/render | tur+eng ground truth | UNKNOWN |
| Rich media/3D | Detect | BLOCKED | BLOCKED | BLOCKED | preserve/remove policy | detect | hostile corpus | BLOCKED execution |

## Feature-level kayıt şablonu

Bir satır `SUPPORTED` yapılırken ek kayıt:

```yaml
capability_id: CAP-FORM-ACRO-TEXT
scope: PDF 1.7/2.0 AcroForm text fields, documented exclusions
read: supported
render: supported
create: preview
edit: supported-fill-only
preserve: supported-with-fixtures
validate: value + appearance + cross-reader
requirements: [REQ-FORM-001]
fixtures: [FORM-001, FORM-007]
test_run: <artifact/url/hash>
engine_version: <exact commit>
known_losses: []
last_verified: YYYY-MM-DD
```

## Çatışma politikaları

| Durum | İşlem | Politika |
|---|---|---|
| Signed/certified PDF | Her mutasyon | Varsayılan read-only; imza/revision etkisi önceden açık |
| PDF/A target | Encryption | Engelle; PDF/A şifrelemeyle bağdaşmaz |
| Applied redaction | Incremental save | Eski içeriği taşıyorsa engelle; full rewrite |
| Tagged/UA document | Content edit | Tag preservation kanıtsızsa uyar/engelle |
| Unknown object/extension | Full rewrite | Preservation kanıtsızsa yeni kopya + kayıp raporu veya engel |
| XFA/JavaScript | Form fill/save | Execution yok; static/preservation scope açık |
| OCR | Existing text layer | Duplicate/overlap detection; varsayılan overwrite yok |

## İddia dili

Kabul: “AcroForm metin ve checkbox alanlarını tanımlı corpus'ta doldurur ve appearance'ı korur.”  
Kabul değil: “Tüm formları destekler.”

Kabul: “PDF/A-4 dönüşümü veraPDF sürüm X ile geçti; görsel fark raporu mevcut.”  
Kabul değil: “Arşiv standardına tamamen uygundur.”

Kabul: “İmzalı byte bütünlüğü doğrulandı; zincir güveni offline olduğundan bilinmiyor.”  
Kabul değil: “İmza geçerli.”
