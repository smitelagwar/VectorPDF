# TASK-2026-003 — M0 VectorPDF baseline entegrasyonu ve yapılandırması

- **Durum:** active
- **Sahip:** Technical owner; uygulayıcı ajan
- **Başlangıç:** 2026-09-01
- **Son checkpoint:** 2026-09-01 Europe/Istanbul
- **İlgili REQ/ADR/SOURCE/RISK:** ADR-0001, ADR-0002, ADR-0003, ADR-0004; SRC-APP-001

## Devam checkpoint'i

- **Son doğrulanmış adım:** Gate 1 tamamlandı, ADR-0002 Accepted yapıldı. M0 fazı başlatıldı.
- **Sıradaki tek eylem:** Açık kaynak baseline C++20 / Qt6 kod tabanını depoya aktarmak, VectorPDF CMake ve proje yapısını kurmak.
- **Blokaj / gereken insan kararı:** Yok.
- **Çalışma ağacı / branch / commit:** `main`.
- **Devam etmeden önce oku:** `AGENTS.md`, `STATUS.md`, `ROADMAP.md`, `docs/adr/0002-upstream-baseline.md`.

## Amaç

Açık kaynak tabanını depoya entegre ederek VectorPDF masaüstü uygulamasının geliştirilebilir, derlenebilir ve çalıştırılabilir çekirdeğini kurmak.

## Kapsam

- Kaynak dosyaların (Pdf4QtLibCore, Pdf4QtLibGui, Pdf4QtLibWidgets, Pdf4QtViewer, Pdf4QtEditor, Pdf4QtPageMaster, Pdf4QtDiff, PdfTool, UnitTests, CMakeLists.txt, vcpkg.json) depoya aktarımı
- VectorPDF adlandırması ve proje yapılandırmasının CMake üzerinde kurulması
- Derleme komutlarının ve geliştirme adımlarının hazırlanması

## Kabul kriterleri

- [x] Upstream kod tabanının çalışma ağacına entegrasyonu
- [x] CMakeLists.txt ve bağımlılık manifestlerinin projede yerini alması
- [x] `STATUS.md` ve ilgili belgelerin M0 durumuna güncellenmesi

## Çalışma kaydı

| Tarih | Değişen yollar | Komut / exit / sonuç | Not |
|---|---|---|---|
| 2026-09-01 | Proje kökü | `Copy-Item` kaynak modülleri ve CMake yapılandırması | C++20/Qt6 açık kaynak kod tabanı depoya aktarıldı. |
