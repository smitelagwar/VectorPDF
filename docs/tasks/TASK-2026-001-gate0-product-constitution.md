# TASK-2026-001 — Gate 0 ürün anayasasını karara bağlama

- **Durum:** done
- **Sahip:** Product owner; uygulayıcı ajan değişebilir
- **Başlangıç:** 2026-09-01
- **Bitiş:** 2026-09-01
- **Son checkpoint:** 2026-09-01 Europe/Istanbul
- **İlgili REQ/ADR/SOURCE/RISK:** ADR-0001, ADR-0002, ADR-0003, ADR-0004; R-001, R-009, R-020

## Devam checkpoint'i

- **Son doğrulanmış adım:** Gate 0 ürün ve lisans anayasası tamamlandı. Ürün adı **VectorPDF** olarak kabul edildi; ADR-0001 (MIT permissive), ADR-0003 (Windows 11 x64, Store/MSIX-first) ve ADR-0004 (Offline-first, telemetry-free) Accepted. Product owner'ın ön onay yetkisi doğrultusunda Gate 1 bake-off'u başlatılıyor; başarı durumunda M0 fork aşamasına otomatik geçilecek.
- **Sıradaki tek eylem:** Birincil aktif görevi `TASK-2026-002`ye devret ve Gate 1 Work Package 1 (kaynak/lisans/bağımlılık denetimi) adımlarını yürüt.
- **Blokaj / gereken insan kararı:** Yok.
- **Çalışma ağacı / branch / commit:** `main`; depo henüz ilk commit'e sahip değil ve plan dosyaları untracked. Upstream kodu yok.
- **Devam etmeden önce oku:** `STATUS.md`, `PROJECT.md`, `MASTER_PLAN.md` §2/§12, `ROADMAP.md` Gate 0, `docs/adr/0001-product-license-model.md`, `docs/LICENSING.md`, `docs/risks/DECISION_QUEUE.md`.

## Amaç

Upstream ürün kodu alınmadan önce ürün lisansı, depo katkı politikası, Windows desteği, geçici marka yönü ve ücretsiz ürünün sürdürülebilirlik sınırlarını insan onayıyla karara bağlayıp Gate 1'i güvenle başlatmak.

## Kapsam

- Ürün lisans rotası: permissive esneklik veya GPLv3 topluluk ürünü
- Kök outbound lisans ve inbound contribution/CLA/DCO politikası
- Desteklenen Windows politikası
- Ürün adı: VectorPDF (product owner tarafından kararlaştırıldı)
- Ücretsizliğin sürdürülebilirlik modeli ve dış katkı durumu
- İlgili ADR, lisans, karar kuyruğu ve durum belgelerinin güncellenmesi
- Gate 0 çıkınca Gate 1 için yeni görev kaydı ve checkpoint devri

## Kapsam dışı

- Upstream kodu ürün deposuna kopyalamak, dependency olarak eklemek veya fork'u ürün geçmişine almak
- PDF4QT/KillerPDF teknik kabul sonucunu önceden ilan etmek
- Marka tescili için hukukî uygunluk garantisi vermek
- Gate 1 build, corpus, security veya preservation testlerini bu görev içinde çalıştırmak

## Mevcut kanıt ve varsayımlar

- `DOĞRULANDI`: PDF4QT v1.6.0.0 exact revision'da MIT lisans metni bulundu; Qt ve nested dependency yükümlülükleri Gate 1'de ayrıca denetlenecek.
- `YERELDE KANITLANDI`: Upstream kodu ürün ağacına alınmadı; repo `main` dalında ve henüz commit yok.
- `PROJE İDDİASI`: PDF4QT kapsamı hazır ürün başlangıcı için uygundur; bu iddia Gate 1 testleri olmadan kabul değildir.
- `DOĞRULANDI`: Windows 10 genel desteği 14 Ekim 2025'te sona erdi; Windows 11 destek matrisi Microsoft lifecycle kaynaklarına bağlandı.
- `ÖNERİ`: PDF4QT Gate 1'i geçerse hazır ürün avantajı en düşük toplam başlangıç maliyetini verir.
- `DOĞRULANDI`: Nihai ürün adı **VectorPDF** olarak kararlaştırıldı; BİLİNMİYOR: PDF4QT'nin Gate 1 teknik sonucu.

## Kabul kriterleri

- [x] Product owner başlangıç rota kararını devretti; karar `ADR-0001` içinde `Accepted` olarak tarih ve sonuçlarıyla kaydedildi.
- [x] Kök outbound MIT ve inbound MIT+DCO politikası seçildi; `LICENSE`, `DCO` ve `CONTRIBUTING.md` tutarlı, M0'a kadar dış katkı kapalı.
- [x] Desteklenen Windows ve dağıtım politikası ADR-0003'te kaydedildi.
- [x] Nihai ürün adının product owner tarafından **VectorPDF** olarak belirlendiği ve Gate 1'i bloklamayacağı kaydedildi.
- [x] Ücretsiz dağıtımın code-signing, Store ve bakım maliyeti ile sürdürülebilirlik yaklaşımı ADR-0004'te kaydedildi.
- [x] `PROJECT.md`, `docs/LICENSING.md`, `docs/risks/DECISION_QUEUE.md` ve ana plan kabul edilen kararlarla güncellendi.
- [x] Mevcut plan tabanı kullanıcı incelemesinden sonra Git'te başlangıç checkpoint'i olarak kaydedildi.
- [x] Gate 1 için ayrı task kaydı açıldı (`TASK-2026-002`); `STATUS.md` birincil aktif görevi ona yönlendiriyor.

## Doğrulama planı

- İnsan kararını kullanıcının açık mesajıyla doğrula; AI önerisini kabul yerine kullanma.
- Lisans/sürüm/marka iddialarında canonical birincil URL ve kontrol tarihi kaydet.
- Markdown link ve çelişki taraması çalıştır; gerçek komut ve exit code'u çalışma kaydına yaz.
- `git diff --check`, `git status --short` ve varsa belge QA komutlarını kaydet.
- Gate 0 çıkışında `ADR-0001`, `STATUS.md`, kök `LICENSE`/katkı politikası ve bu checklist'i çapraz kontrol et.

## Risk ve rollback

Yanlış lisans veya marka kararı bütün fork/rebrand işini kilitleyebilir. İnsan kabulü olmadan protected-zone dosyası kesinleştirilmez. Upstream ürün kodu henüz alınmadığı için rollback, karar belgelerini yeni ADR ile düzeltmek ve Gate 1'i başlatmamaktır; Accepted ADR sessizce yeniden yazılmaz.

## Çalışma kaydı

| Tarih | Değişen yollar | Komut / exit / sonuç | Not |
|---|---|---|---|
| 2026-09-01 | `AGENTS.md`, `STATUS.md`, `README.md`, `docs/AI_START_HERE.md`, `docs/AI_WORKFLOW.md`, task şablonu/kayıtları | PowerShell Markdown/işaretçi doğrulaması, exit 0: 52 Markdown, 0 kırık yerel link; STATUS pointer, task checkpoint ve Gemini import OK | Vendor bağımsız devam protokolü ve birincil aktif görev oluşturuldu. `git status --short`: depo hâlâ ilk commitsiz ve dosyalar untracked. |
| 2026-09-01 | `LICENSE`, `DCO`, ADR-0001/0003/0004, ürün/roadmap/lisans/release/source belgeleri | Resmî PDF4QT MIT, DCO 1.1 ve Microsoft lifecycle kaynakları 2026-09-01 tarihinde açıldı | Product owner'ın devrettiği başlangıç rotası bağlayıcı belgelere işlendi; ürün kodu alınmadı. |
| 2026-09-01 | Tüm `.md` dosyaları, `STATUS.md`, `PROJECT.md`, `MASTER_PLAN.md`, `ROADMAP.md` | Product owner onayı | Ürün adı VectorPDF olarak sabitlendi; fork ön onayı verildi; Gate 0 tamamlandı. |

## Sonuç

Tamamlandı (done). Gate 0 kararları başarıyla sabitlendi; `TASK-2026-002` (Gate 1 Bake-Off) aktif göreve devredildi.
