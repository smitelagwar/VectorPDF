# SRC-APP-001 — PDF4QT v1.6.0.0

- **Karar:** ADOPT-CANDIDATE; Gate 1 WP1 tamamlandı, Gate 1 build/test ve corpus kapıları bekleniyor
- **Canonical:** https://github.com/JakubMelka/PDF4QT
- **Release:** `v1.6.0.0`
- **Commit:** `23f3829ae0420da801824a0d53d82c53baa2bee9`
- **İnceleme:** 2026-09-01
- **Sonraki inceleme:** Gate 1 sonuçlanması veya 2026-09-30

## DOĞRULANDI

- Exact revision `23f3829ae0420da801824a0d53d82c53baa2bee9` kök [LICENSE](https://github.com/JakubMelka/PDF4QT/blob/23f3829ae0420da801824a0d53d82c53baa2bee9/LICENSE) MIT metnidir (Copyright (c) 2018-2025 Jakub Melka).
- Proje ağacındaki 428 C/C++ dosyasının 426'sı açık MIT lisans başlığı taşımaktadır; kalan 2 dosya (`JBIG2_Viewer/mainwindow.h` ve `pdfimageconversion.cpp`) aynı yazara ait olup kök MIT lisansı kapsamındadır.
- **Telif ve Relicense Tarihçesi (G1-LIC-005):**
  - Proje 2018'de LGPLv3 olarak başlatılmıştır.
  - 2024-04-01 tarihinde Çek Cumhuriyeti hukukuna tabi, Jakub Melka'ya sınırsız/devredilebilir/alt-lisanslanabilir hak tanıyan bağlayıcı Contributor License Agreement (CLA) devreye alınmıştır (`78aca34b`, `793cf59c`).
  - 2025-04-27 tarihinde Jakub Melka depoyu LGPLv3'ten MIT lisansına geçirmiştir (`81bf102f`, `88030d10`, `f5d73eb3`).
  - CLA öncesi dönemdeki tüm haricî katkılar incelenmiştir: Yalnızca 4 katkıcıdan toplam 22 commit (Nyall Dawson: 12 derleme/uyarı düzeltmesi; Raffaele Mancuso: 5 doküman/desktop düzeltmesi; Raphael Cotty: 4 Linux QBS/Conan betiği; Alexey Pavlov: 1 mingw uyarısı). Bu katkılar telif riski veya blokaj oluşturmayan küçük derleme/dokümantasyon yamalarıdır.
  - Relicense sonrası 17 haricî commit (çeviriler ve minör uyarılar) doğrudan MIT kapsamında kabul edilmiştir.
- **Doğrudan ve Transitif Bağımlılık Envanteri (G1-LIC-002):**
  - C/C++ Bağımlılıkları (vcpkg / CMake):
    - `tbb` (oneTBB): Apache-2.0
    - `openssl`: Apache-2.0
    - `lcms` (LittleCMS 2): MIT
    - `zlib`: zlib License (Permissive)
    - `openjpeg`: 2-clause BSD
    - `freetype`: FreeType License (FTL, atıflı permissive)
    - `libjpeg-turbo`: IJG License / BSD-style
    - `libpng`: libpng License
    - `blend2d` + `asmjit`: zlib License
  - Gömülü Varlıklar:
    - `Pdf4QtLibCore/cmaps/`: Adobe CMap (3-clause BSD / Adobe CMap License)
    - `resources/unicode-glyph-names.txt` & `zapfdingbats-glyph-names.txt`: Adobe Glyph List (3-clause BSD)
    - `Pdf4QtLibCore/liberation-fonts-ttf/`: Liberation Fonts (SIL OFL 1.1 with Liberation exception)
    - `Pdf4QtLibCore/aatl/source.txt`: Adobe Approved Trust List URL
- **Qt6 Modül Lisans Matrisi (G1-LIC-004):**
  - Kullanılan modüller: `Qt6::Core`, `Qt6::Gui`, `Qt6::Widgets`, `Qt6::Svg`, `Qt6::Xml`, `Qt6::PrintSupport`, `Qt6::TextToSpeech`, `Qt6::Concurrent`.
  - Tümü Community LGPLv3 kapsamındadır; hiçbir GPL-only Qt modülü kullanılmamaktadır.
  - Dinamik bağlama (dynamic linking) ve `THIRD_PARTY_NOTICES.md` + About ekranında LGPLv3 metni/atıfı sağlanarak tam uyum mümkündür.
- **Marka ve Rebrand Haritası (G1-LIC-003):**
  - Upstream kimliği: "PDF4QT", "Jakub Melka", `io.github.JakubMelka.Pdf4qt`, `assets/logo.png`.
  - Rebrand planı: VectorPDF olarak tüm uygulama adları, masaüstü dosyaları, AppxManifest identity ve logolar değiştirilebilir; zorunlu upstream atıfları `THIRD_PARTY_NOTICES.md` ve About diyaloğunda korunur.
- [NOTES.txt](https://github.com/JakubMelka/PDF4QT/blob/v1.6.0.0/NOTES.txt) form/action/annotation/presentation gibi bilinen sınırları kaydeder.
- Upstream CI build eder; CI iş akışında varsayılan `PDF4QT_BUILD_TESTS` kapalıdır ve `ctest` çalıştırılmaz. Test executable'ları `UnitTests` ve `UnitTestsImageOptimizer` olarak tanımlıdır.

## PROJE İDDİASI

Render, encryption, color management, text layout, annotation, form fill, edit, attachment, optimize, compare, static simple XFA read-only, signature validation/creation ve public-key encryption. Bunların hiçbiri ürün support matrisine fixture olmadan taşınmaz.

## YERELDE KANITLANDI

- Exact release `v1.6.0.0` (commit `23f3829ae0420da801824a0d53d82c53baa2bee9`) izole geçici klasöre (`C:\Users\hsyn\AppData\Local\Temp\pdf4qt_audit`) clone edildi; 428 C/C++ dosyası, 286 SVG ikonu, vcpkg manifesti, CMake dosyaları ve tüm commit geçmişi incelendi.
- Yerel çalışma ortamında `git` (2.53.0.2), `dotnet` (10.0), `node` (24.15) ve `winget` mevcuttur; yerel makinede `cmake`, `ninja`, `cl.exe` ve `Qt6` kurulu değildir.
- Release tag'i lisans ve provenance açısından Work Package 1 kriterlerini (G1-LIC-001..005) eksiksiz karşılamaktadır.

## Lisans/provenance bulgusu

- **Sonuç:** PASS. Dağıtımı veya permissive MIT lisanslama modelini engelleyen çözümsüz telif/lisans sorunu bulunmamaktadır.
- Zorunlu gereksinimler: Qt LGPLv3 dinamik bağlama, Liberation OFL ve 3rd party notice paketlemesi VectorPDF M0 sürümünde `THIRD_PARTY_NOTICES.md` içine dahil edilecektir.

## Teknik risk

- Parser/render/edit native C++ ve ürünle aynı repo/process mimarisinde; hostile PDF güvenlik yüzeyi yüksek.
- Maintainer concentration yüksek (Jakub Melka).
- Known content-edit/form boşlukları ve gerçek conformance/preservation kanıtı bilinmiyor.
- Worker ayrıştırmasının coupling maliyeti bilinmiyor.

## Kabul kapısı

`docs/GATE1_BAKEOFF.md` tam uygulanır. WP1 lisans/provenance geçmiştir. WP2 (clean double build & tests), WP3 (25-fixture corpus), WP4 (kullanıcı senaryoları), WP5 (preservation), WP6 (hostile & worker feasibility) ve WP7 (rebrand/bakım) adımları tamamlandığında ADR-0002 Accepted yapılabilecektir.

## Alternatif

PDFium + qpdf + Tesseract tabanlı custom permissive shell. Daha yüksek ürün geliştirme maliyeti; daha temiz worker/adapter sınırı.

