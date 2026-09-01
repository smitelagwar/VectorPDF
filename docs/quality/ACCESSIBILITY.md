# Erişilebilirlik Kalite Planı

**Durum:** Proposed  
**Son güncelleme:** 2026-09-01

Erişilebilirlik iki ayrı üründür: uygulamanın kullanılabilirliği ve uygulamanın okuduğu/ürettiği PDF'nin erişilebilirliği. Birindeki başarı diğerini kanıtlamaz.

## A. Windows uygulaması

P0 gereksinimler:

- Bütün ana işler keyboard-only; logical tab order ve görünür focus
- Menü/toolbar/tool canvas için accessible name, role, state ve shortcut
- Windows high contrast; yalnız renkle anlam yok
- 100/150/200% scaling ve multi-monitor
- Narrator + en az bir bağımsız screen reader smoke
- UI Automation tree kararlı ve anlamlı
- Zoom/font değişiminde clipping/overlap yok
- Error/progress/cancel screen reader'a duyurulur; focus çalınmaz
- Pointer/stylus hedefleri ve keyboard alternatifi
- Animasyon/motion reduced preference

Canvas tabanlı PDF alanı, annotation handles ve form widgets için custom accessibility peer gerekebilir; Gate 1'de baseline kapasitesi ölçülür.

## B. PDF içeriği

Okuma:

- structure tree/role map
- reading order ve page/content association
- document language ve Unicode
- ActualText/alt text
- headings/lists/tables
- form label/state/required
- bookmarks

Düzenleme:

- mevcut tag ilişkisini koru veya değişikliği açıkça raporla
- yeni annotation/form/text için semantik ve accessible-name üret
- content delete/move sonrası dangling structure reference bırakma
- OCR text layer okuma sırası ve dil bilgisini taşısın
- otomatik alt text/reading order önerisi insan onayı olmadan conformant sayılmasın

## Test hattı

1. Unit: accessibility tree/role/name/state mapping
2. Automated Windows UI Automation smoke
3. Keyboard task scripts
4. High contrast/DPI screenshot review
5. Tagged PDF preservation semantic diff
6. PDF/UA-1 için UA-1 reference suite + validator
7. PDF/UA-1 için Matterhorn 1.1 human checklist
8. PDF/UA-2 için pinned UA-2 validator + ISO 14289-2 tabanlı ayrı human checklist
9. Her profile için Narrator/NVDA gerçek görevleri

Her release ana görevlerden en az open/search/form/annotation/save/settings/install akışını keyboard ve screen reader ile smoke eder.

## Başarı iddiası

“Erişilebilir” tek bayrak değildir. UI için test edilen Windows/screen-reader sürümü ve görevler; PDF için profile/version, validator ve insan kontrol kapsamı ilan edilir. Bilinen engeller release notes ve support matrix'te görünür.
