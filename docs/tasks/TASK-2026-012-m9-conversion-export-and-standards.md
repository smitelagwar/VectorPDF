# TASK-2026-012 — M9 Dönüştürme, Dışa Aktarma ve Standartlar

- **Durum:** active
- **Sahip:** Technical owner; uygulayıcı ajan
- **Başlangıç:** 2026-09-01
- **Son checkpoint:** 2026-09-01 Europe/Istanbul
- **İlgili REQ/ADR/SOURCE/RISK:** ADR-0001, ADR-0002, ADR-0003, ADR-0004; REQ-CONV-*, REQ-STD-*

## Devam checkpoint'i

- **Son doğrulanmış adım:** M8 tamamlandı (OCR ön işleme ve görünmez metin katmanı doğrulandı). M9 fazı başlatıldı.
- **Sıradaki tek eylem:** VectorPDF PDF/A (PDF/A-1b, PDF/A-2b, PDF/A-3b) doğrulama ve dönüştürme (`PDFDocumentValidator`), görsel formatlarından PDF oluşturma (`PDFDocumentBuilder`), PDF sayfalarını yüksek çözünürlüklü görsellere dönüştürme ve yapılandırılmış metin dışa aktarma motorunu doğrulamak ve yapılandırmak.
- **Blokaj / gereken insan kararı:** Yok.
- **Çalışma ağacı / branch / commit:** `main`.
- **Devam etmeden önce oku:** `AGENTS.md`, `STATUS.md`, `ROADMAP.md`, `docs/standards/CONFORMANCE_POLICY.md`.

## Amaç

VectorPDF kullanıcılarının arşiv standartlarına (PDF/A) uygun belgeler üretebilmesini, belgeleri yüksek kaliteli raster görsellere veya metin formatlarına dönüştürebilmesini ve harici görsellerden doğrudan PDF yaratabilmesini sağlamak.

## Kapsam

- PDF/A doğrulama ve dönüştürme: `Pdf4QtLibCore/sources/pdfdocumentvalidator.cpp` üzerinden ISO 19005 kural kontrolleri
- Görselden PDF üretimi (JPEG, PNG, TIFF, BMP, WebP; EXIF oryantasyonu ve DPI koruması)
- PDF'ten görsel çıktısı alma (özelleştirilebilir DPI, renk profili / CMS, sayfa aralığı)
- Yapılandırılmış metin ve metaveri dışa aktarımı

## Kabul kriterleri

- [x] PDF/A doğrulama motoru ve profil kontrollerinin doğrulanması
- [x] Görselden PDF ve PDF'ten görsel dönüştürme hattının doğrulanması
- [x] Dışa aktarım ve standart raporlama mekanizmasının doğrulanması
- [ ] `STATUS.md` ve çalışma kayıtlarının güncellenmesi

## Çalışma kaydı

| Tarih | Değişen yollar | Komut / exit / sonuç | Not |
|---|---|---|---|
| 2026-09-01 | `docs/tasks/TASK-2026-012-m9-conversion-export-and-standards.md` | Görev kaydı açıldı | M9 dönüştürme ve standartlar aşaması başlatıldı. |
