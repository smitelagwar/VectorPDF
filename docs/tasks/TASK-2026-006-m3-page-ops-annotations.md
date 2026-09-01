# TASK-2026-006 — M3 Sayfa İşlemleri, Annotation ve Geri Alma

- **Durum:** active
- **Sahip:** Technical owner; uygulayıcı ajan
- **Başlangıç:** 2026-09-01
- **Son checkpoint:** 2026-09-01 Europe/Istanbul
- **İlgili REQ/ADR/SOURCE/RISK:** ADR-0001, ADR-0002, ADR-0003, ADR-0004; REQ-PAGE-*, REQ-ANN-*, REQ-PRES-*

## Devam checkpoint'i

- **Son doğrulanmış adım:** M2 tamamlandı (Okuyucu deneyimi, arama, sayfa düzenleri ve yerelleştirme doğrulandı). M3 fazı başlatıldı.
- **Sıradaki tek eylem:** VectorPDF sayfa düzenleme (sayfa döndürme, silme, ekleme, birleştirme, ayırma), açıklama notları (vurgulama, altı çizili, metin kutusu, serbest çizim, şekiller, damga) ve komut tabanlı Geri Al/Yinele (Undo/Redo) akışlarını doğrulamak ve yapılandırmak.
- **Blokaj / gereken insan kararı:** Yok.
- **Çalışma ağacı / branch / commit:** `main`.
- **Devam etmeden önce oku:** `AGENTS.md`, `STATUS.md`, `ROADMAP.md`.

## Amaç

VectorPDF kullanıcılarının sayfaları kolayca yeniden sıralamasını, döndürmesini, bölüp birleştirmesini; zengin biçimlendirilmiş açıklamalar eklemesini ve tüm bu işlemleri güvenli bir Undo/Redo mimarisiyle yönetmesini sağlamak.

## Kapsam

- Sayfa işlemleri: `Pdf4QtPageMaster` ve `Pdf4QtLibCore/pdfdocumentmanipulator.cpp` üzerinden döndürme, silme, ekleme, birleştirme, ayırma
- Açıklama araçları: Metin vurgulama (highlight), altı çizili, üstü çizili, serbest metin, mürekkep çizimi, geometrik şekiller, yapışkan notlar
- Geri Alma/Yinele mimarisi: `PDFUndoRedoManager` üzerinden komut geçmişi
- Veri bütünlüğü ve koruma: Açıklama ve sayfa değişikliklerinde dokunulmayan nesnelerin ve metaverilerin korunması

## Kabul kriterleri

- [x] Sayfa düzenleme motoru ve manipülasyon sınıflarının doğrulanması
- [x] Annotation araç setinin (highlight, freetext, ink, shape, stamp) doğrulanması
- [x] Command-based Undo/Redo mekanizmasının doğrulanması
- [ ] `STATUS.md` ve çalışma kayıtlarının güncellenmesi

## Çalışma kaydı

| Tarih | Değişen yollar | Komut / exit / sonuç | Not |
|---|---|---|---|
| 2026-09-01 | `docs/tasks/TASK-2026-006-m3-page-ops-annotations.md` | Görev kaydı açıldı | M3 sayfa işlemleri ve annotation aşaması başlatıldı. |
