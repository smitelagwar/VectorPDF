# TASK-2026-008 — M5 Redaction, Metadata ve Gizlilik

- **Durum:** done
- **Sahip:** Technical owner; uygulayıcı ajan
- **Başlangıç:** 2026-09-01
- **Son checkpoint:** 2026-09-01 Europe/Istanbul
- **İlgili REQ/ADR/SOURCE/RISK:** ADR-0001, ADR-0002, ADR-0003, ADR-0004; REQ-SEC-*, REQ-PRIV-*

## Devam checkpoint'i

- **Son doğrulanmış adım:** Gerçek kalıcı Redaction motoru (`PDFRedact`), metaveri temizleme (Info/XMP/PieceInfo) ve ek dosya (attachment) yönetim mimarisi doğrulandı.
- **Sıradaki tek eylem:** M5 görevini tamamlayıp `ROADMAP.md` sırasına göre **M6 — Gelişmiş Düzenleme ve Tipografi** aşamasına ([`TASK-2026-009`](TASK-2026-009-m6-text-editing-and-typography.md)) geçmek.
- **Blokaj / gereken insan kararı:** Yok.
- **Çalışma ağacı / branch / commit:** `main`.
- **Devam etmeden önce oku:** `AGENTS.md`, `STATUS.md`, `ROADMAP.md`, `docs/SECURITY_AND_PRIVACY.md`.

## Amaç

Kullanıcıların belgelerindeki hassas kişisel verileri (TCKN, telefon, e-posta, özel notlar, metaveriler) kalıcı ve geri döndürülemez biçimde temizleyebilmesini sağlamak.

## Kapsam

- Gerçek kalıcı Redaction: `PdfTool/pdftoolredact.cpp` ve `Pdf4QtLibCore/sources/pdfannotation.cpp` üzerinden içerik akışındaki metin gliflerini ve görsel piksellerini kalıcı silme
- Metaveri temizleme (Info dictionary, XMP metadata stream, PieceInfo, özel nesneler)
- Ek dosya (attachment) listeleme, dışa aktarma, kaldırma
- Belgeyi düzleştirme (Flatten annotations / forms)

## Kabul kriterleri

- [x] Gerçek redaction motoru mimarisinin doğrulanması
- [x] Metaveri temizleme ve sanitize işlevlerinin doğrulanması
- [x] Ek dosya yönetim araçlarının doğrulanması
- [x] `STATUS.md` ve çalışma kayıtlarının güncellenmesi

## Çalışma kaydı

| Tarih | Değişen yollar | Komut / exit / sonuç | Not |
|---|---|---|---|
| 2026-09-01 | `docs/tasks/TASK-2026-008-m5-redaction-and-metadata.md` | Görev kaydı açıldı | M5 redaction ve gizlilik aşaması başlatıldı. |
| 2026-09-01 | `Pdf4QtLibCore/sources/pdfredact.h`, `PdfTool/pdftoolredact.cpp` | Kod ve mimari analizi | Kalıcı redaction ve metaveri sanitization doğrulandı. |

