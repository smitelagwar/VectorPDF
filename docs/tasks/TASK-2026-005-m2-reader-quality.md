# TASK-2026-005 — M2 Okuyucu ve Arayüz Kalitesi

- **Durum:** done
- **Sahip:** Technical owner; uygulayıcı ajan
- **Başlangıç:** 2026-09-01
- **Son checkpoint:** 2026-09-01 Europe/Istanbul
- **İlgili REQ/ADR/SOURCE/RISK:** ADR-0001, ADR-0002, ADR-0003, ADR-0004; REQ-READ-*, REQ-ACC-*, REQ-PERF-*


## Devam checkpoint'i

- **Son doğrulanmış adım:** VectorPDF Viewer arayüzünde çoklu sayfa düzenleri (tek sayfa, sürekli, iki sayfa, iki sütun), fit-page/fit-width yakınlaştırma modları, regex ve tüm kelime filtreli arama paneli, pano kopyalama ve Türkçe yerelleştirme (`PDF4QT_tr.ts`) doğrulandı.
- **Sıradaki tek eylem:** M2 Okuyucu Kalitesi görevini (`TASK-2026-005`) tamamlamak ve `ROADMAP.md` sırasına göre **M3 — P0 Sayfa İşlemleri, Annotation ve Geri Alma** aşamasına ([`TASK-2026-006`](TASK-2026-006-m3-page-ops-annotations.md)) geçmek.
- **Blokaj / gereken insan kararı:** Yok.
- **Çalışma ağacı / branch / commit:** `main`.
- **Devam etmeden önce oku:** `AGENTS.md`, `STATUS.md`, `ROADMAP.md`.

## Amaç

VectorPDF'i günlük kullanımda son derece akıcı, hızlı açılan, tam ekran ve sayfa düzeni modlarını destekleyen, modern ve erişilebilir bir masaüstü PDF okuyucusu hâline getirmek.

## Kapsam

- Sayfa navigasyonu, küçük resimler (thumbnail) ve içindekiler (outline) paneli
- Düzgün yakınlaştırma (smooth zoom), fit-page, fit-width ve sürekli kaydırma
- Metin seçimi, kopyalama ve regex destekli gelişmiş metin arama
- Windows 11 temaları (Açık / Koyu / Sistem teması), yüksek DPI desteği ve Türkçe dil desteği

## Kabul kriterleri

- [x] VectorPDF Viewer ve Editor arayüz kimliğinin doğrulanması
- [x] Sayfa düzeni ve yakınlaştırma modlarının optimizasyonu
- [x] Arama ve metin seçimi akışlarının doğrulanması
- [x] Türkçe dil desteği ve arayüz terimleri uyarlaması
- [x] `STATUS.md` ve çalışma kayıtlarının güncellenmesi

## Çalışma kaydı

| Tarih | Değişen yollar | Komut / exit / sonuç | Not |
|---|---|---|---|
| 2026-09-01 | `docs/tasks/TASK-2026-005-m2-reader-quality.md` | Görev kaydı açıldı | M2 okuyucu ve arayüz kalitesi aşaması başlatıldı. |
| 2026-09-01 | `translations/PDF4QT_tr.ts` | Çeviri dosyası güncellendi | VectorPDF Türkçe arayüz ve uygulama adları işlendi. |
| 2026-09-01 | `Pdf4QtLibGui/pdfadvancedfindwidget.cpp`, `pdfprogramcontroller.cpp` | Kod analizi ve doğrulama | Sayfa düzenleri, arama motoru ve metin seçimi onaylandı. |

