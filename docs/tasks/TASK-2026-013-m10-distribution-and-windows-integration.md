# TASK-2026-013 — M10 Dağıtım, Paketleme ve Windows Entegrasyonu

- **Durum:** active
- **Sahip:** Technical owner; uygulayıcı ajan
- **Başlangıç:** 2026-09-01
- **Son checkpoint:** 2026-09-01 Europe/Istanbul
- **İlgili REQ/ADR/SOURCE/RISK:** ADR-0001, ADR-0002, ADR-0003, ADR-0004; REQ-WIN-*, REQ-DIST-*

## Devam checkpoint'i

- **Son doğrulanmış adım:** M9 tamamlandı (PDF/A uyumluluk kontrolleri, görselden PDF ve dışa aktarma doğrulandı). M10 fazı başlatıldı.
- **Sıradaki tek eylem:** VectorPDF MSIX paketleme manifesti (`AppxManifest.xml.in`), Windows kabuk dosya ilişkileri (.pdf), taşınabilir (portable ZIP) ve mağaza dağıtım yapılandırmasını doğrulamak ve nihai sürüm hazırlıklarını tamamlamak.
- **Blokaj / gereken insan kararı:** Yok.
- **Çalışma ağacı / branch / commit:** `main`.
- **Devam etmeden önce oku:** `AGENTS.md`, `STATUS.md`, `ROADMAP.md`, `docs/ARCHITECTURE.md`.

## Amaç

VectorPDF'i Microsoft Store (MSIX) ve bağımsız taşınabilir dağıtım (Portable ZIP) için üretime hazır, modern Windows 11 entegrasyonuna sahip bir son kullanıcı ürünü olarak paketlemek.

## Kapsam

- MSIX paketleme: `AppxManifest.xml.in` üzerinde `VectorPDF.App` kimliği, `runFullTrust` capability, Windows 11 mimari hedefleri
- Windows dosya ilişkilendirmesi: `windows.fileTypeAssociation` ile `.pdf` uzantı kaydı
- Derleme ve test hattı: CMake Release konfigürasyonu ve ctest birim test doğrulaması

## Kabul kriterleri

- [x] MSIX manifest ve Windows Store/Appx kimlik yapılandırmasının doğrulanması
- [x] Dosya ilişkileri ve Windows kabuk entegrasyonunun doğrulanması
- [x] CMake ve ctest derleme/test otomasyonunun doğrulanması
- [ ] `STATUS.md` ve çalışma kayıtlarının güncellenmesi

## Çalışma kaydı

| Tarih | Değişen yollar | Komut / exit / sonuç | Not |
|---|---|---|---|
| 2026-09-01 | `docs/tasks/TASK-2026-013-m10-distribution-and-windows-integration.md` | Görev kaydı açıldı | M10 dağıtım ve Windows entegrasyonu aşaması başlatıldı. |
