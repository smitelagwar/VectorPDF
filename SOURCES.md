# Kaynak ve Upstream Karar Sicili

**Doğrulama kesiti:** 2026-09-01  
**Kural:** Bu dosya bütün araştırma/reference kimliklerinin insan tarafından okunur değerlendirme indeksidir. `docs/sources/SOURCE_REGISTRY.yml`, yalnız karar-kritik veya içe alınmaya aday kaynakların exact makine kaydını tutar; reference-only satırlarla bire bir parite zorunlu değildir. Bir kaynak kod/asset/model/binary olarak kabul edilmeden önce registry kaydı ve `docs/sources/evaluations/` değerlendirmesi zorunludur. Zamanla değişebilen her bilgi kullanımdan önce yeniden doğrulanır.

## Durum anahtarı

- **ADOPT-CANDIDATE:** Gate'ten geçerse fork/bileşen olabilir.
- **COMPONENT-CANDIDATE:** Dar adapter/worker arkasında değerlendirilebilir.
- **REFERENCE:** Davranış, UX veya test fikri; kod kopyalanmaz.
- **WATCH:** Sorun çözülürse yeniden değerlendir.
- **REJECT-CURRENT:** Mevcut lisans/teknik durumla ürün tabanı değildir.

## Çalışan uygulama adayları

| ID | Proje ve sabit sürüm | Lisans kanıtı | Durum | Ne alınabilir / neden |
|---|---|---|---|---|
| SRC-APP-001 | [PDF4QT v1.6.0.0](https://github.com/JakubMelka/PDF4QT/releases/tag/v1.6.0.0), tag commit `23f3829ae0420da801824a0d53d82c53baa2bee9` | [Exact-commit LICENSE](https://github.com/JakubMelka/PDF4QT/blob/23f3829ae0420da801824a0d53d82c53baa2bee9/LICENSE), [MIT değişikliği](https://github.com/JakubMelka/PDF4QT/issues/271) | **ADOPT-CANDIDATE** | Viewer/editor/page master/diff/CLI aynı depo; Windows/C++20/Qt6. `NOTES.txt`, test çalıştırma, historical relicensing provenance ve Qt yükümlülüğü Gate 1'de. |
| SRC-APP-002 | [KillerPDF v1.8.2](https://github.com/SteveTheKiller/KillerPDF/releases/tag/v1.8.2), tag commit `fca9a6f4bd77d1fdde692bd9e402c07e0ec490de` | [Exact-commit GPL-3.0-only](https://github.com/SteveTheKiller/KillerPDF/blob/fca9a6f4bd77d1fdde692bd9e402c07e0ec490de/LICENSE) | **REFERENCE / REJECT-CURRENT** | ADR-0001 permissive rota nedeniyle baseline değil. .NET/WPF davranış ve test taksonomisi referansı; kod alınmaz. |
| SRC-APP-003 | [Open PDF Studio v1.92.0](https://github.com/OpenAEC-Foundation/open-pdf-studio/releases/tag/v1.92.0), commit `8919c8efaacffe43c5e59f2270ba9f038eb1ea17` | [Exact-commit kök LGPL-3.0](https://github.com/OpenAEC-Foundation/open-pdf-studio/blob/8919c8efaacffe43c5e59f2270ba9f038eb1ea17/LICENSE.md); manifestler ve `mupdf` bağımlılığı ayrıca incelenmeli | **WATCH / REJECT-CURRENT** | Multi-process PDFium ve render regression fikirleri. Kök/manifest lisans çelişkisi ve doğrudan AGPL MuPDF kullanımı çözülmeden kod alınmaz. |
| SRC-APP-004 | [Stirling-PDF v2.14.3](https://github.com/Stirling-Tools/Stirling-PDF/releases/tag/v2.14.3), commit `e556eba8326c8349aa0318034cfdb5c442dca21c` | [Tag kök istisnaları](https://github.com/Stirling-Tools/Stirling-PDF/blob/v2.14.3/LICENSE), [desktop](https://github.com/Stirling-Tools/Stirling-PDF/blob/v2.14.3/frontend/editor/src/desktop/LICENSE), [engine](https://github.com/Stirling-Tools/Stirling-PDF/blob/v2.14.3/engine/LICENSE) | **REFERENCE / REJECT-CURRENT** | Araç taksonomisi, workflow/progress/error UX'i. Özel lisanslı yollar nedeniyle current release white-label ürün tabanı değil. |
| SRC-APP-005 | [SumatraPDF](https://github.com/sumatrapdfreader/sumatrapdf) | [COPYING](https://github.com/sumatrapdfreader/sumatrapdf/blob/master/COPYING), [AUTHORS/bileşenler](https://github.com/sumatrapdfreader/sumatrapdf/blob/master/AUTHORS) | **REFERENCE** | Windows başlangıç hızı, sade reader UX, shell/file-association. GPL/AGPL/MuPDF ve editor kapsamı nedeniyle taban değil. |
| SRC-APP-006 | [Okular](https://invent.kde.org/graphics/okular) | [Resmî ürün/lisans](https://apps.kde.org/okular/) | **REFERENCE** | Annotation/form/signature/accessibility UX. GPL/KDE/Poppler zinciri ve reader odağı nedeniyle taban değil. |
| SRC-APP-007 | [Sioyek](https://github.com/ahrm/sioyek) | [LICENSE](https://github.com/ahrm/sioyek/blob/main/LICENSE) | **REFERENCE** | Research navigation, portals, smart jump. GPL/MuPDF ve native editor kapsamı nedeniyle kod alınmaz. |
| SRC-APP-008 | [PDFsam Basic](https://github.com/torakiki/pdfsam) | [Resmî lisans açıklaması](https://pdfsam.org/faq/) | **REFERENCE** | Batch/page-operation job UX. AGPL ve dar kapsam. |
| SRC-APP-009 | [PDF Arranger](https://github.com/pdfarranger/pdfarranger) | [GPL-3.0](https://github.com/pdfarranger/pdfarranger/blob/main/COPYING) | **REFERENCE** | Thumbnail drag/reorder/crop/overlay akışları. |
| SRC-APP-010 | [Xournal++](https://github.com/xournalpp/xournalpp) | [LICENSE](https://github.com/xournalpp/xournalpp/blob/master/LICENSE) | **REFERENCE** | Ink/stylus/layer/autosave; XOPP+PDF background modeli native PDF round-trip değildir. |
| SRC-APP-011 | [ONLYOFFICE DesktopEditors](https://github.com/ONLYOFFICE/DesktopEditors) | [AGPL + ek koşullar](https://github.com/ONLYOFFICE/DesktopEditors/blob/master/LICENSE), [marka politikası](https://www.onlyoffice.com/blog/2026/05/onlyoffice-license-and-trademark-policy) | **REFERENCE / REJECT AS BASE** | Gerçek edit/form/redaction UX'i; devasa office suite ve ek atıf/trademark yükü. |
| SRC-APP-012 | [LibreOffice core](https://git.libreoffice.org/core) | [Resmî lisanslar](https://www.libreoffice.org/about-us/licenses/) | **REFERENCE / optional sidecar** | Dönüşüm/PDF export. Fork edilmez; gerekirse ayrı sınırlı `soffice` worker PoC'u. |

## Motor ve dar bileşenler

| ID | Kaynak | Lisans | Durum | Sınır |
|---|---|---|---|---|
| SRC-ENG-001 | [PDFium canonical](https://pdfium.googlesource.com/pdfium/), [README/build](https://pdfium.googlesource.com/pdfium/+/refs/heads/main/README.md) | [BSD-benzeri çekirdek + Apache/third-party notices](https://pdfium.googlesource.com/pdfium/+/refs/heads/main/LICENSE) | **COMPONENT-CANDIDATE** | Güçlü render/form/annotation/düşük seviye edit. Release SDK yerine commit pin + tekrarlanabilir kendi build'i gerekir; JS/XFA off. |
| SRC-ENG-002 | [qpdf v12.4.1](https://github.com/qpdf/qpdf/releases/tag/v12.4.1), commit `c37f83ae468abb6cc741f43b2f6fdeb66e550ffb` | [Tag Apache-2.0](https://github.com/qpdf/qpdf/blob/v12.4.1/LICENSE.txt), [NOTICE](https://github.com/qpdf/qpdf/blob/v12.4.1/NOTICE.md) | **COMPONENT-CANDIDATE** | Structure/repair/encryption/validation yardımcısı; renderer veya signature engine değildir. Incremental/sign sınırı [tag TODO](https://github.com/qpdf/qpdf/blob/v12.4.1/TODO.md)'da. |
| SRC-ENG-003 | [PDF.js v6.3.289](https://github.com/mozilla/pdf.js/releases/tag/v6.3.289), commit `1c8020a7d4e43668ac287a3ecf9a8dbea17e4c56` | [Tag Apache-2.0](https://github.com/mozilla/pdf.js/blob/v6.3.289/LICENSE) | **COMPONENT-CANDIDATE / REFERENCE** | Web UI seçilirse viewer/text layer/forms/annotation. Tam content editor veya masaüstü uygulama değildir. |
| SRC-ENG-004 | [MuPDF](https://github.com/ArtifexSoftware/mupdf) | [AGPL-3.0 veya ticari](https://mupdf.readthedocs.io/en/latest/license.html) | **REJECT unless AGPL/commercial** | Çok güçlü motor; permissive rota ile lisanssız uyumsuz. JS/WASM wrapper aynı riski taşır. |
| SRC-ENG-005 | [PoDoFo](https://github.com/podofo/podofo) | [NOTICE](https://github.com/podofo/podofo/blob/master/NOTICE): library LGPL-2.0-or-later veya MPL-2.0; tools GPL | **COMPONENT-CANDIDATE** | İmza/yazma için MPL yolu altında PoC; renderer değil, küçük bus factor. |
| SRC-ENG-006 | [Apache PDFBox](https://github.com/apache/pdfbox) | [Apache-2.0](https://github.com/apache/pdfbox/blob/trunk/LICENSE.txt) | **COMPONENT-CANDIDATE** | Form/PDF-A/imza sidecar ihtimali; JRE yükü. İmza doğrulama sınırları [FAQ](https://pdfbox.apache.org/3.0/faq.html)'da. |
| SRC-ENG-007 | [pdfcpu](https://github.com/pdfcpu/pdfcpu) | [Apache-2.0](https://github.com/pdfcpu/pdfcpu/blob/master/LICENSE.txt) | **COMPONENT-CANDIDATE** | Static batch/structural helper; v0.x ve signature sonucu best-effort, trust otoritesi değil. |
| SRC-ENG-008 | [pikepdf](https://github.com/pikepdf/pikepdf) | [MPL-2.0](https://github.com/pikepdf/pikepdf/blob/main/LICENSE.txt) | **REFERENCE / automation PoC** | qpdf tabanlı Python otomasyonu; native qpdf varken çekirdekte ek runtime borcu. |
| SRC-ENG-009 | [pdf-lib](https://github.com/Hopding/pdf-lib) | [MIT](https://github.com/Hopding/pdf-lib/blob/master/LICENSE.md) | **LIMITED COMPONENT** | Basit draw/page/form; mevcut metni çıkarma/değiştirme ve encryption yok; son stabil 1.17.1 eski. Kritik writer yapılmaz. |
| SRC-ENG-010 | [pdfme](https://github.com/pdfme/pdfme) | [MIT](https://github.com/pdfme/pdfme/blob/main/LICENSE.md) | **REFERENCE / limited component** | Template designer/generator fikirleri; genel PDF editorü değil. |
| SRC-ENG-011 | [Apryse PDF.js Express](https://pdfjs.express/) | [Kısıtlayıcı ürün şartları](https://pdfjs.express/static/lic/P_5Feb8PDF.jsExpressLicenseTerms.html) | **REJECT-CURRENT** | Kamu şartları bağımsız/rekabetçi standalone PDF uygulamasını kısıtlıyor; açık görünen UI bunu değiştirmez. |

## OCR

| ID | Kaynak | Lisans | Karar |
|---|---|---|---|
| SRC-OCR-001 | [Tesseract](https://github.com/tesseract-ocr/tesseract), [model listesi](https://tesseract-ocr.github.io/tessdoc/Data-Files.html) | Apache-2.0 | Varsayılan offline OCR adayı; `tur+eng` model artifact'i exact sürüm/hash ile pinlenecek. |
| SRC-OCR-002 | [Leptonica](https://github.com/DanBloomberg/leptonica) | [BSD-2-Clause](https://github.com/DanBloomberg/leptonica/blob/master/leptonica-license.txt) | Tesseract preprocessing bağımlılığı/adayı. |
| SRC-OCR-003 | [PaddleOCR](https://github.com/PaddlePaddle/PaddleOCR) | Apache-2.0 (exact release tekrar doğrulanır) | Büyük runtime/model nedeniyle optional advanced layout/table pack PoC'u. |
| SRC-OCR-004 | [Windows.Media.Ocr](https://learn.microsoft.com/en-us/uwp/api/windows.media.ocr.ocrengine) | Platform API | Dil paketi/Windows bağımlılığı nedeniyle ana motor değil, ölçülmüş fallback olabilir. |

## Standart ve doğrulama kaynakları

| ID | Kaynak | Kullanım |
|---|---|---|
| SRC-STD-001 | [ISO 32000-2:2020](https://www.iso.org/standard/75839.html), [ücretsiz paket/errata](https://pdfa.org/resource/iso-32000-2/), [issue/errata](https://pdf-issues.pdfa.org/32000-2-2020/) | PDF 2.0 normatif taban ve errata |
| SRC-STD-002 | [PDF 2.0 örnekleri](https://github.com/pdf-association/pdf20examples) | Feature fixtures; normatif standardın yerine geçmez |
| SRC-STD-003 | [ISO 19005-4:2020 PDF/A-4](https://www.iso.org/standard/71832.html) | Arşiv profili |
| SRC-STD-004 | [veraPDF validation](https://docs.verapdf.org/validation/), [corpus](https://github.com/veraPDF/veraPDF-corpus) | PDF/A machine validation; tam semantik/görsel garanti değil |
| SRC-STD-005 | [ISO 14289-1 PDF/UA-1](https://www.iso.org/standard/64599.html), [Matterhorn 1.1](https://pdfa.org/resource/the-matterhorn-protocol), [PDF/UA-1 reference suite](https://pdfa.org/resource/pdfua-reference-suite/) | PDF/UA-1 machine + human test paketi |
| SRC-STD-006 | [ETSI EN 319 142-1 v1.2.1](https://www.etsi.org/deliver/etsi_en/319100_319199/31914201/01.02.01_60/en_31914201v010201p.pdf) | Yayımlanmış PAdES tabanı; draft sürüm ürün iddiası olmaz |
| SRC-STD-007 | [ETSI TS 119 102-1 v1.4.1](https://www.etsi.org/deliver/etsi_ts/119100_119199/11910201/01.04.01_60/ts_11910201v010401p.pdf) | İmza doğrulama prosedürü |
| SRC-STD-008 | [ISO 14289-2:2024 PDF/UA-2](https://www.iso.org/standard/82278.html), [veraPDF validation](https://docs.verapdf.org/validation/) | PDF/UA-2: profile-specific validator + ISO 14289-2'ye dayalı ayrı insan/AT kontrolü; UA-1 suite'i UA-2 kanıtı sayılmaz |

## Güvenlik, test ve dağıtım kaynakları

| ID | Kaynak | Kullanım |
|---|---|---|
| SRC-QA-001 | [PDF Association corpora](https://github.com/pdf-association/pdf-corpora) | Güvenli manifestle indirilecek test corpus katalogu |
| SRC-QA-002 | [PDFium tests](https://pdfium.googlesource.com/pdfium_tests/) | Render/parser regression örnekleri |
| SRC-QA-003 | [PDF.js regression PDFs](https://github.com/mozilla/pdf.js/tree/master/test/pdfs) | Cross-engine differential fixtures |
| SRC-SEC-001 | [Windows AppContainer isolation](https://learn.microsoft.com/en-us/windows/win32/secauthz/appcontainer-isolation), [implementation](https://learn.microsoft.com/en-us/windows/win32/secauthz/implementing-an-appcontainer) | Düşük yetkili PDF worker PoC'u |
| SRC-SEC-002 | [Windows Job Objects](https://learn.microsoft.com/en-us/windows/win32/procthread/job-objects) | CPU/RAM/child-process/watchdog sınırları |
| SRC-REL-001 | [Windows dağıtım yolları](https://learn.microsoft.com/en-us/windows/apps/package-and-deploy/choose-distribution-path) | Store/MSIX ve doğrudan dağıtım kararı |
| SRC-REL-002 | [MSIX signing guide](https://learn.microsoft.com/en-us/windows/msix/package/signing-package-overview) | Paket imzalama |
| SRC-REL-003 | [SLSA v1.2](https://slsa.dev/spec/v1.2/) | Build provenance hedefi |
| SRC-REL-004 | [SPDX specification](https://spdx.dev/use/specifications/) / [CycloneDX](https://cyclonedx.org/specification/overview/) | SBOM |
| SRC-REL-005 | [Microsoft supported Windows client versions](https://learn.microsoft.com/en-us/windows/release-health/supported-versions-windows-client), [Windows 11 lifecycle](https://learn.microsoft.com/en-us/lifecycle/products/windows-11-home-and-pro) | ADR-0003 dinamik Windows 11 release/test matrisi |
| SRC-REL-006 | [Windows 10 support ended 14 October 2025](https://support.microsoft.com/en-US/Windows/Deployment/Updates-Lifecycle/windows-10-support-has-ended-on-october-14-2025) | Windows 10'u resmî destek kapsamı dışında bırakma |

## Proje yönetişimi kaynakları

| ID | Kaynak | Kullanım |
|---|---|---|
| SRC-GOV-001 | [Developer Certificate of Origin 1.1](https://developercertificate.org/) | ADR-0001 inbound contribution provenance beyanı |

## AI çalışma sistemi kaynakları

| ID | Kaynak | Karar |
|---|---|---|
| SRC-AI-001 | [Codex AGENTS.md](https://learn.chatgpt.com/docs/agent-configuration/agents-md) | Kısa kök sözleşme; daha yakın dosya daha dar kapsam; gereksiz bağlam yok |
| SRC-AI-002 | [Codex skills](https://learn.chatgpt.com/docs/build-skills) | Tekrarlanabilir dar süreçler `.agents/skills/<name>/SKILL.md` |
| SRC-AI-003 | [Claude Code memory](https://code.claude.com/docs/en/memory) | `CLAUDE.md` yalnız `@AGENTS.md` import eder; gerçeği kopyalamaz |
| SRC-AI-004 | [Gemini CLI context files](https://github.com/google-gemini/gemini-cli/blob/main/docs/cli/gemini-md.md) | `GEMINI.md` kök sözleşmeyi import eder |

## Yerel kanıt kayıtları

### EVID-LOCAL-001 — KillerPDF testleri

- Tarih: 2026-08-31
- Kaynak: geçici clone, upstream güncel `main` commit `256bef5dbe254b89caddb632d694f3271b83ffc8`
- Ortam: Windows, .NET SDK `10.0.400`
- Engine test sonucu: 1.439 passed, 0 failed, 0 skipped
- App test sonucu: 277 passed, 0 failed
- Toplam: 1.716 passed
- Sınır: Upstream'in README'de anılan tüm corpus/uyumluluk kapsamını veya güvenliği kanıtlamaz.

### EVID-LOCAL-002 — PDF4QT kaynak incelemesi

- Tarih: 2026-08-31
- İncelenen güncel commit: `0be824befae6b90789ec700f62a2e66a025e10a4`
- Kaynak dosya sayısı: 1.141
- Unit test modülü bulundu; yerel makinede CMake/Qt toolchain olmadığı için build/test çalıştırılmadı.
- README, lisans, CMake yapısı, uygulama/library modülleri ve test dosyaları incelendi.

## Doğrulanmış referans rapor düzeltmeleri

- PDF4QT güncel lisansı LGPL değil; proje 27 Nisan 2025'te MIT'ye relicensed olduğunu beyan eder. Qt gibi bağımlılıklar kendi lisansını korur.
- PDFium'u tek kelimeyle “Apache-2.0” diye etiketlemek eksiktir; ana lisans BSD-benzeri koşullar ve ek Apache/third-party notice'lar içerir.
- Stirling-PDF current main bütünüyle basit MIT değildir; önemli engine/desktop yolları özel lisansla istisnadır.
- Görsel “signature” desteği kriptografik imzalama/doğrulama kanıtı değildir.
- `pdf-lib` genel mevcut sayfa metni düzenleme veya stratejik PDF writer çözümü değildir.

Bu düzeltmeler, eski `referanslar/` belgelerini silmez; karar sırasında bu sicil önceliklidir.
