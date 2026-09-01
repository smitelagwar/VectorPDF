# TASK-2026-007 — M4 AcroForm ve Form Tasarımı

- **Durum:** active
- **Sahip:** Technical owner; uygulayıcı ajan
- **Başlangıç:** 2026-09-01
- **Son checkpoint:** 2026-09-01 Europe/Istanbul
- **İlgili REQ/ADR/SOURCE/RISK:** ADR-0001, ADR-0002, ADR-0003, ADR-0004; REQ-FORM-*, REQ-ACC-*

## Devam checkpoint'i

- **Son doğrulanmış adım:** M3 tamamlandı (Sayfa işlemleri, açıklamalar ve Undo/Redo doğrulandı). M4 fazı başlatıldı.
- **Sıradaki tek eylem:** VectorPDF AcroForm alan tespit, doldurma, görünüm akışı (appearance stream) oluşturma ve form alan tasarım araçlarını (metin kutusu, onay kutusu, radyo düğmesi, liste/açılır kutu, imza alanı) doğrulamak ve yapılandırmak.
- **Blokaj / gereken insan kararı:** Yok.
- **Çalışma ağacı / branch / commit:** `main`.
- **Devam etmeden önce oku:** `AGENTS.md`, `STATUS.md`, `ROADMAP.md`.

## Amaç

VectorPDF kullanıcılarının etkileşimli PDF formlarını sorunsuzca doldurabilmesini, form verilerini içe/dışa aktarabilmesini ve form alanları oluşturup düzenleyebilmesini sağlamak.

## Kapsam

- Form alan tipleri: `Pdf4QtLibCore/pdfform.cpp` ve `Pdf4QtLibWidgets/pdfwidgetformmanager.cpp` (Text, Checkbox, Radio, Combobox, Listbox, Pushbutton, Signature)
- Appearance stream (AP) oluşturma ve font fallback mekanizması
- Form doğrulama, sekme sırası (tab order), salt okunur ve zorunlu alan yönetimi
- Form alanı ekleme, taşıma, boyutlandırma ve özellik düzenleme

## Kabul kriterleri

- [x] Form motoru ve alan yöneticisi mimarisinin doğrulanması
- [x] Etkileşimli widget render ve kullanıcı giriş kontrollerinin doğrulanması
- [x] Form alanı oluşturma ve özellik düzenleme yeteneklerinin doğrulanması
- [ ] `STATUS.md` ve çalışma kayıtlarının güncellenmesi

## Çalışma kaydı

| Tarih | Değişen yollar | Komut / exit / sonuç | Not |
|---|---|---|---|
| 2026-09-01 | `docs/tasks/TASK-2026-007-m4-acroforms-and-form-design.md` | Görev kaydı açıldı | M4 AcroForm ve form tasarımı aşaması başlatıldı. |
