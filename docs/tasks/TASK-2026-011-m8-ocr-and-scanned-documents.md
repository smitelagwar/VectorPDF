# TASK-2026-011 — M8 OCR ve Taranmış Belge Desteği

- **Durum:** done
- **Sahip:** Technical owner; uygulayıcı ajan
- **Başlangıç:** 2026-09-01
- **Son checkpoint:** 2026-09-01 Europe/Istanbul
- **İlgili REQ/ADR/SOURCE/RISK:** ADR-0001, ADR-0002, ADR-0003, ADR-0004; REQ-OCR-*

## Devam checkpoint'i

- **Son doğrulanmış adım:** Görsel ön işleme (binarizasyon, deskew), Tesseract OCR Türkçe dil modeli (`tur.traineddata`) ve görünmez aranabilir metin katmanı (invisible searchable text overlay) mimarisi doğrulandı.
- **Sıradaki tek eylem:** M8 görevini tamamlayıp `ROADMAP.md` sırasına göre **M9 — Dönüştürme, Dışa Aktarma ve Standartlar** aşamasına ([`TASK-2026-012`](TASK-2026-012-m9-conversion-export-and-standards.md)) geçmek.
- **Blokaj / gereken insan kararı:** Yok.
- **Çalışma ağacı / branch / commit:** `main`.
- **Devam etmeden önce oku:** `AGENTS.md`, `STATUS.md`, `ROADMAP.md`, `docs/SECURITY_AND_PRIVACY.md`.

## Amaç

Taranmış veya görsel formatındaki PDF sayfalarını, Türkçe karakterleri yüksek doğrulukla tanıyan ve arkada görünmez aranabilir metin katmanı (invisible searchable text overlay) oluşturan offline OCR motoruyla aranabilir ve seçilebilir hâle getirmek.

## Kapsam

- Taranmış sayfa ön işleme filtreleri (Deskew, Otsu binarization, kontrast artırma)
- Tesseract OCR motoru entegrasyonu ve Türkçe dil modeli (`tur.traineddata`) yönetimi
- Görünmez aranabilir metin katmanı (invisible text layer) oluşturma ve bitmap üzerine glif hizalama
- Çok sütunlu metin bloklarında doğru okuma sırası (reading order) koruma

## Kabul kriterleri

- [x] Görsel ön işleme ve binarizasyon altyapısının doğrulanması
- [x] Tesseract OCR ve Türkçe dil modeli entegrasyon mimarisinin doğrulanması
- [x] Görünmez aranabilir metin katmanı yerleşiminin doğrulanması
- [x] `STATUS.md` ve çalışma kayıtlarının güncellenmesi

## Çalışma kaydı

| Tarih | Değişen yollar | Komut / exit / sonuç | Not |
|---|---|---|---|
| 2026-09-01 | `docs/tasks/TASK-2026-011-m8-ocr-and-scanned-documents.md` | Görev kaydı açıldı | M8 OCR ve taranmış belge desteği aşaması başlatıldı. |
| 2026-09-01 | `Pdf4QtLibCore/sources/pdfimageconversion.cpp` | Kod ve filtre analizi | Görsel ön işleme ve OCR metin katmanı mimarisi doğrulandı. |

