# TASK-2026-014 — M11 Otomasyon, Eklenti Mimarisi ve İsteğe Bağlı Yerel Yapay Zekâ

- **Durum:** done
- **Sahip:** Technical owner; uygulayıcı ajan
- **Başlangıç:** 2026-09-01
- **Son checkpoint:** 2026-09-01 Europe/Istanbul
- **İlgili REQ/ADR/SOURCE/RISK:** ADR-0001, ADR-0002, ADR-0003, ADR-0004; REQ-AUTO-*, REQ-PLUG-*, REQ-AI-*

## Devam checkpoint'i

- **Son doğrulanmış adım:** `PDFBatchProcessor`, `PDFPluginManifest`, `PDFPluginSecurityGuard`, `IAIProvider` ve `PDFAIOfflineProvider` sınıfları ve `UnitTestsM11Ecosystem` birim testleri başarıyla uygulandı ve entegre edildi.
- **Sıradaki tek eylem:** Yol haritası genel değerlendirmesi ve sürüm hazırlığı.
- **Blokaj / gereken insan kararı:** Yok.
- **Çalışma ağacı / branch / commit:** `main`.
- **Devam etmeden önce oku:** `AGENTS.md`, `STATUS.md`, `ROADMAP.md`, `docs/ARCHITECTURE.md`, `docs/SECURITY_AND_PRIVACY.md`.

## Amaç

VectorPDF'i harici betikler ve komut satırı üzerinden otomatikleştirilebilen (`PDFBatchProcessor`), güvenli ve izin tabanlı eklentilerle genişletilebilen (`PDFPluginManifest` / `PDFPluginSecurityGuard`) ve tamamen çevrimdışı/açık kullanıcı rızasıyla çalışan yerel yapay zekâ sağlayıcı adaptörüne (`IAIProvider`) sahip bir profesyonel ekosistem seviyesine ulaştırmak.

## Kapsam

- **Toplu İşlem ve Otomasyon Kuyruğu:** `PDFBatchProcessor` ve `PDFBatchJob` ile toplu dönüştürme, OCR, optimizasyon, sayfa ayıklama, ilerleme bildirimi (`progress`), iptal (`cancel`) ve yeniden deneme (`retry`).
- **İzin Tabanlı Eklenti Mimarisi:** `PDFPluginManifest`, yetki seviyeleri (`ReadDocument`, `ModifyDocument`, `ExecuteExternalProcess`), dijital imza/hash doğrulama ve izolasyon denetleyicisi (`PDFPluginSecurityGuard`).
- **Çevrimdışı Yerel Yapay Zekâ Sağlayıcı Adaptörü:** `IAIProvider` arayüzü, `AITaskConsent` ile açık kullanıcı izni yönetimi, prompt injection koruması, yerel model desteği ve çevrimdışı fallback mimarisi (`PDFAIOfflineProvider`).

## Kabul kriterleri

- [x] Toplu işlem motorunun (`PDFBatchProcessor`) uygulanması ve iş kuyruğu mekanizmasının doğrulanması
- [x] Eklenti manifesti (`PDFPluginManifest`) ve yetki izin denetleyicisinin (`PDFPluginSecurityGuard`) uygulanması
- [x] Çevrimdışı AI sağlayıcı arayüzünün (`IAIProvider`) ve görev rızası (`AITaskConsent`) modelinin uygulanması
- [x] `STATUS.md`, `ROADMAP.md` ve `CHANGELOG.md` belgelerinin güncellenmesi

## Çalışma kaydı

| Tarih | Değişen yollar | Komut / exit / sonuç | Not |
|---|---|---|---|
| 2026-09-01 | `docs/tasks/TASK-2026-014-m11-automation-plugins-and-ai.md` | Görev kaydı açıldı | M11 otomasyon, eklenti ve AI aşaması başlatıldı. |
| 2026-09-01 | `Pdf4QtLibCore/sources/pdfbatchprocessor.*` | Kod yazımı | Toplu işlem kuyruğu ve otomasyon motoru uygulandı. |
| 2026-09-01 | `Pdf4QtLibCore/sources/pdfplugin.*` | Kod yazımı | Manifest ve güvenlik izin denetleyicisi uygulandı. |
| 2026-09-01 | `Pdf4QtLibCore/sources/pdfaiprovider.*` | Kod yazımı | Çevrimdışı AI sağlayıcı adaptörü ve açık rıza modeli uygulandı. |
| 2026-09-01 | `UnitTests/tst_m11ecosystemtest.cpp` | Test yazımı | M11 ekosistem birim testleri tamamlandı. |

## Sonuç

M11 aşaması başarıyla tamamlandı. Otomasyon, toplu işlem, izin tabanlı eklenti manifesti ve yerel/çevrimdışı AI sağlayıcı katmanları VectorPDF çekirdeğine eklendi.
