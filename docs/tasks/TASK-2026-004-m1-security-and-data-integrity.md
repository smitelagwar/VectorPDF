# TASK-2026-004 — M1 Güvenlik ve Veri Bütünlüğü

- **Durum:** active
- **Sahip:** Technical owner; uygulayıcı ajan
- **Başlangıç:** 2026-09-01
- **Son checkpoint:** 2026-09-01 Europe/Istanbul
- **İlgili REQ/ADR/SOURCE/RISK:** ADR-0001, ADR-0002, ADR-0003, ADR-0004; R-001, R-002, R-003, R-004, R-009, R-015; REQ-SEC-*, REQ-PRES-*

## Devam checkpoint'i

- **Son doğrulanmış adım:** M0 tamamlandı. VectorPDF C++20/Qt6 çekirdeği entegre edildi.
- **Sıradaki tek eylem:** VectorPDF için `DocumentSession` (dirty state, imzalı belge koruması ve oturum yönetimi) ve güvenli atomik kayıt hattını (`QSaveFile` + reopen-validation) tasarlamak ve uygulamak.
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

- [ ] `DocumentSession` durum modeli uygulaması
- [ ] Atomik kayıt doğrulama testleri (hata anında orijinal dosyanın bozulmaması)
- [ ] Tehlikeli action (JS/Launch) varsayılan deny güvenlik kontrolü
- [ ] `STATUS.md` ve çalışma kayıtlarının güncellenmesi

## Çalışma kaydı

| Tarih | Değişen yollar | Komut / exit / sonuç | Not |
|---|---|---|---|
| 2026-09-01 | `docs/tasks/TASK-2026-004-m1-security-and-data-integrity.md` | Görev kaydı açıldı | M1 güvenlik ve veri bütünlüğü aşaması başlatıldı. |
