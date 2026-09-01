# Başlangıç İzlenebilirlik Matrisi

**Durum:** Planlama; kod/test ID'leri baseline sonrası doldurulacak  
**Son güncelleme:** 2026-09-01

| Requirement kümesi | Kaynak/politika | ADR/mimari | Test/corpus hedefi | Roadmap |
|---|---|---|---|---|
| REQ-PROD-* | PROJECT, LICENSING | ADR-0001/0002 | identity/license audit | Gate 0/1, M0 |
| REQ-VIEW-* | ISO 32000-2, support matrix | ARCHITECTURE worker/render | ordinary/font/large/UI corpus | M2 |
| REQ-SAVE-* | security + preservation policy | DocumentSession/save state | fault injection + round-trip | M1 |
| REQ-PAGE/ANN/EDIT-* | ISO exact feature clauses `BİLİNMİYOR`; M3/M7 araştırmasında sabitlenecek | Command model | semantic/render/undo property | M3/M7 |
| REQ-FORM-* | ISO exact form clauses `BİLİNMİYOR`; M4 araştırmasında sabitlenecek | capability/single writer | form value+appearance cross-reader | M4 |
| REQ-SEC/PRIV-* | Windows isolation docs, threat model | worker/broker | hostile/no-network/log scanner | M1 sürekli |
| REQ-A11Y-* | PDF/UA profile-specific policy + Windows UIA | accessibility policy | keyboard/Narrator/NVDA/tag diff | M2/M10 |
| REQ-OCR-* | Tesseract/model sources | IOcrProvider | tur+eng CER/WER/bbox/render | M5 |
| REQ-RED-* | ISO redaction model + threat policy | full rewrite/save policy | byte/object/extract/render attacks | M6 |
| REQ-PDFA-* | ISO 19005-4 + veraPDF | conversion job | profile corpus+diff+report | M8 |
| REQ-UA-001/002 | ISO 14289-1 + UA-1 suite + Matterhorn 1.1 | tag-preservation | UA-1 validator+human+AT | M10 |
| REQ-UA-001/003 | ISO 14289-2 + UA-2 validator profile | tag-preservation | separate UA-2 validator+human+AT | M10 |
| REQ-SIGN/CRYPT-* | ETSI/ISO signature+crypto | revision/sign worker | attack/trust/interoperability | M9 |
| REQ-NFR-* | SLSA/SPDX/Microsoft release | release architecture | rebuild/SBOM/install/perf | M0 sürekli |

## Hücre doldurma kuralı

Implementation başladığında her requirement için:

```text
REQ-ID → normative SOURCE-ID/section → ADR-ID → source module/symbol → TEST-ID → fixture IDs → last run artifact/hash → support maturity
```

zinciri eklenir. Otomasyon orphan REQ, test edilmeyen supported capability ve source'suz conformance claim'i bloklar.
