# SRC-APP-001 — PDF4QT v1.6.0.0

- **Karar:** ADOPT-CANDIDATE; Gate 1 geçmeden import yok
- **Canonical:** https://github.com/JakubMelka/PDF4QT
- **Release:** `v1.6.0.0`
- **Commit:** `23f3829ae0420da801824a0d53d82c53baa2bee9`
- **İnceleme:** 2026-08-31
- **Sonraki inceleme:** Gate 1 veya 2026-09-30

## DOĞRULANDI

- Exact revision [LICENSE](https://github.com/JakubMelka/PDF4QT/blob/23f3829ae0420da801824a0d53d82c53baa2bee9/LICENSE) MIT metnidir.
- Upstream [README](https://github.com/JakubMelka/PDF4QT/tree/v1.6.0.0), projenin 27 Nisan 2025'te LGPLv3'ten MIT'ye relicensed edildiğini; Windows/Linux, C++20, Qt ve üçüncü taraf kütüphaneleri beyan eder.
- Depoda Pdf4QtEditor, Viewer, PageMaster, Diff, LaunchPad, PdfTool, core/gui/widgets/plugins ve UnitTests alanları vardır.
- [NOTES.txt](https://github.com/JakubMelka/PDF4QT/blob/v1.6.0.0/NOTES.txt) form/action/annotation/presentation gibi bilinen sınırları kaydeder.
- Upstream CI build eder; incelenen workflow'da gerçek `ctest` çalıştırma adımı görülmedi.

## PROJE İDDİASI

Render, encryption, color management, text layout, annotation, form fill, edit, attachment, optimize, compare, static simple XFA read-only, signature validation/creation ve public-key encryption. Bunların hiçbiri ürün support matrisine fixture olmadan taşınmaz.

## YERELDE KANITLANDI

- Güncel `main` commit `0be824befae6b90789ec700f62a2e66a025e10a4` geçici alana clone edildi; 1.141 dosya ve UnitTests modülü incelendi.
- Yerel makinede `cmake --version` exit 1 (`cmake` yok). Build/test çalıştırılmadı.
- Release tag'i yerelde henüz clean Windows/Qt toolchain ile denenmedi.

## Lisans/provenance açık işleri

- 27 Nisan 2025 öncesi contributor/file history ve relicense yetkisi taranmalı; maintainer beyanı hukuk incelemesinin yerine geçmez.
- Qt kullanılan modüller bazında LGPL/GPL/commercial statüsü; dynamic link, relink ve source/offer paketi doğrulanmalı.
- OpenSSL, FreeType, OpenJPEG, libjpeg, zlib, LittleCMS, Blend2D ve transitif vcpkg artifact'leri exact lock/notice almalı.
- İkon/logo/çeviri/test asset kapsamı ayrıca çıkarılmalı.

## Teknik risk

- Parser/render/edit native C++ ve ürünle aynı repo/process mimarisinde; hostile PDF güvenlik yüzeyi yüksek.
- Maintainer concentration yüksek.
- Known content-edit/form boşlukları ve gerçek conformance/preservation kanıtı bilinmiyor.
- Worker ayrıştırmasının coupling maliyeti bilinmiyor.

## Kabul kapısı

`docs/GATE1_BAKEOFF.md` tam uygulanır. License/provenance, clean double build, gerçek tests, 25-fixture corpus, save/preservation, hostile bounds ve worker feasibility geçerse ADR-0002 Accepted olabilir.

## Alternatif

PDFium + qpdf + Tesseract tabanlı custom permissive shell. Daha yüksek ürün geliştirme maliyeti; daha temiz worker/adapter sınırı.
