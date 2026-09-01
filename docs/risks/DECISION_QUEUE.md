# Açık Karar Kuyruğu

Karar verilmemiş konu, kod içine varsayım olarak gömülmez.

| ID | Karar | Önerilen varsayılan | Bloke ettiği çıktı | Çözüm kanıtı |
|---|---|---|---|---|
| DQ-004 | PDF4QT baseline kabulü | Gate 1'i çalıştır | M0 fork/rebrand | ADR-0002 evidence |

## Çözülenler

| ID | Karar | Sonuç | Tarih / kanıt |
|---|---|---|---|
| DQ-001 | Permissive mi GPLv3 ürün mü? | Permissive rota | 2026-09-01 / ADR-0001 |
| DQ-002 | Nihai ürün adı | VectorPDF | 2026-09-01 / Product owner onayı |
| DQ-003 | Windows destek politikası | Microsoft desteğindeki Windows 11 x64; Win10 unsupported; ARM64 deferred | 2026-09-01 / ADR-0003 |
| DQ-005 | Outbound/inbound katkı lisansı | Root MIT; inbound MIT + DCO 1.1; M0'a kadar katkı kapalı | 2026-09-01 / ADR-0001 |
| DQ-006 | Store-only mi direct channel da mı? | Store/MSIX first; direct channel ayrı kabul kanıtı ister | 2026-09-01 / ADR-0003 |
| DQ-007 | Opt-in diagnostics olacak mı? | İlk stable'a kadar otomatik upload yok; yalnız sanitized local log ve manuel export | 2026-09-01 / ADR-0004 |

Karar alındığında satır silinmez; ilgili ADR ve çözüm tarihi eklenir veya `resolved` bölümüne taşınır.
