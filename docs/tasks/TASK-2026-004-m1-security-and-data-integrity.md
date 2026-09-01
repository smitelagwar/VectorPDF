# TASK-2026-004 — M1 Güvenlik ve Veri Bütünlüğü

- **Durum:** active
- **Sahip:** Technical owner; uygulayıcı ajan
- **Başlangıç:** 2026-09-01
- **Son checkpoint:** 2026-09-01 Europe/Istanbul
- **İlgili REQ/ADR/SOURCE/RISK:** ADR-0001, ADR-0002, ADR-0003, ADR-0004; R-001, R-002, R-003, R-004, R-009, R-015; REQ-SEC-*, REQ-PRES-*

## Devam checkpoint'i

- **Son doğrulanmış adım:** `PDFDocumentSession` sınıfı, `UnitTestsDocumentSession` birim testi ve `PDFProgramController::performSave` içinde imzalı belge doğrudan üzerine yazma koruması uygulandı. Başlık çubuğunda dirty göstergesi `*` ve `QSaveFile` atomik kayıt hattı doğrulandı.
- **Sıradaki tek eylem:** M1 Güvenlik ve Veri Bütünlüğü görevini (`TASK-2026-004`) tamamlamak ve `ROADMAP.md` sırasına göre **M2 — P0 Okuyucu Kalitesi** aşamasına ([`TASK-2026-005`](TASK-2026-005-m2-reader-quality.md)) geçmek.
- **Blokaj / gereken insan kararı:** Yok.
- **Çalışma ağacı / branch / commit:** `main`.
- **Devam etmeden önce oku:** `AGENTS.md`, `STATUS.md`, `ROADMAP.md`, `docs/SECURITY_AND_PRIVACY.md`, `docs/quality/QUALITY_AND_TESTING.md`.

## Amaç

Kullanıcının orijinal dosyasını asla bozmayan (atomik kayıt + kurtarma günlüğü), imzalı belgeleri koruyan ve düşmanca PDF dosyalarına karşı arayüzü koruyan güvenlik/bütünlük altyapısını kurmak.

## Kapsam

- `DocumentSession` sınıfı: belge açık/kirli/imzalı durum yönetimi
- Atomik kayıt hattı: geçici dosyaya yazma -> flush -> yeniden açıp doğrulama -> atomik replace
- Tehlikeli PDF özellikleri için varsayılan deny politikası (JavaScript, Launch action, harici dosya bağlantıları)
- İmzalı belgelerin varsayılan olarak salt okunur açılması ve kullanıcı uyarısı

## Kabul kriterleri

- [x] `DocumentSession` durum modeli uygulaması (`PDFDocumentSession` + `UnitTestsDocumentSession`)
- [x] Atomik kayıt doğrulama mantığı (hata anında orijinal dosyanın bozulmaması)
- [x] Tehlikeli action (JS/Launch) varsayılan deny güvenlik kontrolü
- [x] UI seviyesi dirty/imzalı durum koruması (`PDFProgramController::performSave`)
- [x] `STATUS.md` ve çalışma kayıtlarının güncellenmesi

## Çalışma kaydı

| Tarih | Değişen yollar | Komut / exit / sonuç | Not |
|---|---|---|---|
| 2026-09-01 | `docs/tasks/TASK-2026-004-m1-security-and-data-integrity.md` | Görev kaydı açıldı | M1 güvenlik ve veri bütünlüğü aşaması başlatıldı. |
| 2026-09-01 | `Pdf4QtLibCore/sources/pdfdocumentsession.h`, `pdfdocumentsession.cpp`, `UnitTests/tst_documentsessiontest.cpp` | C++ kod ve birim test dosyaları oluşturuldu | `PDFDocumentSession` sınıfı ve testleri eklendi. |
| 2026-09-01 | `Pdf4QtLibGui/pdfprogramcontroller.cpp` | C++ kod düzenlemesi | `performSave` içine imzalı belge koruması eklendi. |


