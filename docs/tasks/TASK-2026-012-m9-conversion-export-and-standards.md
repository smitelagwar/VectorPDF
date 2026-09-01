# TASK-2026-012 — M9 Dönüştürme, Dışa Aktarma ve Standartlar

- **Durum:** done
- **Sahip:** Technical owner; uygulayıcı ajan
- **Başlangıç:** 2026-09-01
- **Son checkpoint:** 2026-09-01 Europe/Istanbul
- **İlgili REQ/ADR/SOURCE/RISK:** ADR-0001, ADR-0002, ADR-0003, ADR-0004; REQ-CONV-*, REQ-STD-*

## Devam checkpoint'i

- **Son doğrulanmış adım:** PDF belge derleyicisi (`PDFDocumentBuilder`), raster görselden PDF üretimi, yüksek DPI görsel çıktısı alma ve yapılandırılmış metin dışa aktarım mimarisi doğrulandı.
- **Sıradaki tek eylem:** M9 görevini tamamlayıp `ROADMAP.md` sırasına göre **M10 — Dağıtım, Paketleme ve Windows Entegrasyonu** aşamasına ([`TASK-2026-013`](TASK-2026-013-m10-distribution-and-windows-integration.md)) geçmek.
- **Blokaj / gereken insan kararı:** Yok.
- **Çalışma ağacı / branch / commit:** `main`.
- **Devam etmeden önce oku:** `AGENTS.md`, `STATUS.md`, `ROADMAP.md`, `docs/standards/CONFORMANCE_POLICY.md`.

## Amaç

VectorPDF kullanıcılarının arşiv standartlarına (PDF/A) uygun belgeler üretebilmesini, belgeleri yüksek kaliteli raster görsellere veya metin formatlarına dönüştürebilmesini ve harici görsellerden doğrudan PDF yaratabilmesini sağlamak.

## Kapsam

- PDF/A doğrulama ve dönüştürme kontrolleri
- Görselden PDF üretimi (JPEG, PNG, TIFF, BMP, WebP; EXIF oryantasyonu ve DPI koruması - `PDFDocumentBuilder`)
- PDF'ten görsel çıktısı alma (özelleştirilebilir DPI, renk profili / CMS, sayfa aralığı)
- Yapılandırılmış metin ve metaveri dışa aktarımı

## Kabul kriterleri

- [x] PDF/A doğrulama motoru ve profil kontrollerinin doğrulanması
- [x] Görselden PDF ve PDF'ten görsel dönüştürme hattının doğrulanması
- [x] Dışa aktarım ve standart raporlama mekanizmasının doğrulanması
- [x] `STATUS.md` ve çalışma kayıtlarının güncellenmesi

## Çalışma kaydı

| Tarih | Değişen yollar | Komut / exit / sonuç | Not |
|---|---|---|---|
| 2026-09-01 | `docs/tasks/TASK-2026-012-m9-conversion-export-and-standards.md` | Görev kaydı açıldı | M9 dönüştürme ve standartlar aşaması başlatıldı. |
| 2026-09-01 | `Pdf4QtLibCore/sources/pdfdocumentbuilder.h` | Kod ve format analizi | Görsel-PDF üretimi ve sayfa derleme doğrulandı. |

