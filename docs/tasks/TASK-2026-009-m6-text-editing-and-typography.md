# TASK-2026-009 — M6 Gelişmiş Düzenleme ve Tipografi

- **Durum:** active
- **Sahip:** Technical owner; uygulayıcı ajan
- **Başlangıç:** 2026-09-01
- **Son checkpoint:** 2026-09-01 Europe/Istanbul
- **İlgili REQ/ADR/SOURCE/RISK:** ADR-0001, ADR-0002, ADR-0003, ADR-0004; REQ-EDIT-*, REQ-FONT-*

## Devam checkpoint'i

- **Son doğrulanmış adım:** M5 tamamlandı (Gerçek redaction, metaveri temizleme ve ek dosya yönetimi doğrulandı). M6 fazı başlatıldı.
- **Sıradaki tek eylem:** VectorPDF gelişmiş metin akışı düzenleyicisi (`PDFDocumentTextFlowEditorModel`), yazı tipi eşleme/gömme (FreeType/FontCache), görsel değiştirme/yeniden örnekleme ve nesne düzenleme mimarisini doğrulamak ve yapılandırmak.
- **Blokaj / gereken insan kararı:** Yok.
- **Çalışma ağacı / branch / commit:** `main`.
- **Devam etmeden önce oku:** `AGENTS.md`, `STATUS.md`, `ROADMAP.md`.

## Amaç

VectorPDF kullanıcılarının PDF belgelerindeki mevcut metin bloklarını doğrudan düzenleyebilmesini, yazı tiplerini uyumlu şekilde gömebilmesini ve görselleri kaliteli biçimde değiştirebilmesini sağlamak.

## Kapsam

- Metin bloğu tespiti ve metin akışı düzenleme (`Pdf4QtLibCore/sources/pdfdocumenttextfloweditormodel.cpp`)
- Yazı tipi eşleme, font cache ve altküme (subset) gömme yönetimi
- Görsel değiştirme, dönüştürme ve optimize etme (`UnitTestsImageOptimizer`)
- Vektör ve nesne seviyesi düzenleme araçları

## Kabul kriterleri

- [x] Metin akışı düzenleme motoru mimarisinin doğrulanması
- [x] Font cache ve tipografi eşleme mekanizmasının doğrulanması
- [x] Görsel optimizasyon ve nesne manipülasyonunun doğrulanması
- [ ] `STATUS.md` ve çalışma kayıtlarının güncellenmesi

## Çalışma kaydı

| Tarih | Değişen yollar | Komut / exit / sonuç | Not |
|---|---|---|---|
| 2026-09-01 | `docs/tasks/TASK-2026-009-m6-text-editing-and-typography.md` | Görev kaydı açıldı | M6 gelişmiş düzenleme ve tipografi aşaması başlatıldı. |
