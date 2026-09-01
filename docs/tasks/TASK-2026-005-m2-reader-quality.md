# TASK-2026-005 — M2 Okuyucu ve Arayüz Kalitesi

- **Durum:** active
- **Sahip:** Technical owner; uygulayıcı ajan
- **Başlangıç:** 2026-09-01
- **Son checkpoint:** 2026-09-01 Europe/Istanbul
- **İlgili REQ/ADR/SOURCE/RISK:** ADR-0001, ADR-0002, ADR-0003, ADR-0004; REQ-READ-*, REQ-ACC-*, REQ-PERF-*

## Devam checkpoint'i

- **Son doğrulanmış adım:** M1 tamamlandı (DocumentSession, atomik kayıt ve imzalı belge koruması devrede). M2 fazı başlatıldı.
- **Sıradaki tek eylem:** VectorPDF okuyucu deneyimini (PDFViewer arayüzü, sayfa yakınlaştırma/döndürme, metin arama ve seçim özellikleri) optimize etmek ve Windows 11 Fluent tasarım uyumunu güçlendirmek.
- **Blokaj / gereken insan kararı:** Yok.
- **Çalışma ağacı / branch / commit:** `main`.
- **Devam etmeden önce oku:** `AGENTS.md`, `STATUS.md`, `ROADMAP.md`.

## Amaç

VectorPDF'i günlük kullanımda son derece akıcı, hızlı açılan, tam ekran ve sayfa düzeni modlarını destekleyen, modern ve erişilebilir bir masaüstü PDF okuyucusu hâline getirmek.

## Kapsam

- Sayfa navigasyonu, küçük resimler (thumbnail) ve içindekiler (outline) paneli
- Düzgün yakınlaştırma (smooth zoom), fit-page, fit-width ve sürekli kaydırma
- Metin seçimi, kopyalama ve regex destekli gelişmiş metin arama
- Windows 11 temaları (Açık / Koyu / Sistem teması) ve yüksek DPI desteği

## Kabul kriterleri

- [x] VectorPDF Viewer ve Editor arayüz kimliğinin doğrulanması
- [ ] Sayfa düzeni ve yakınlaştırma modlarının optimizasyonu
- [ ] Arama ve metin seçimi akışlarının doğrulanması
- [ ] `STATUS.md` ve çalışma kayıtlarının güncellenmesi

## Çalışma kaydı

| Tarih | Değişen yollar | Komut / exit / sonuç | Not |
|---|---|---|---|
| 2026-09-01 | `docs/tasks/TASK-2026-005-m2-reader-quality.md` | Görev kaydı açıldı | M2 okuyucu ve arayüz kalitesi aşaması başlatıldı. |
