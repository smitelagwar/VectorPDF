# VectorPDF — Nihai Ana Plan

**Belge durumu:** Accepted ana strateji 1.1  
**Araştırma kesiti:** 2026-08-31  
**Son güncelleme:** 2026-09-01  
**Karar biçimi:** Fazlar tarihle değil, ölçülebilir geçiş kapılarıyla ilerler.

## 1. Yönetici özeti

Bu proje için en akıllı rota yeni bir PDF motoru veya yeni bir masaüstü kabuğu yazmak değildir. En iyi hazır ve yeniden markalanabilir aday **PDF4QT v1.6.0.0**'dır: Windows'ta çalışan viewer, editor, page manager, diff ve CLI uygulamalarını; C++20/Qt6 tabanlı PDF kütüphanelerini aynı MIT lisanslı depoda sunar.

Ancak “çalışıyor” ile “bizim kalite tabanımız olmaya hazır” aynı şey değildir. PDF4QT:

- büyük ölçüde tek geliştiriciye bağımlıdır;
- kendi C++ parser/render/edit motorunu içerdiği için kötü amaçlı PDF saldırı yüzeyi büyüktür;
- upstream CI'sinde derleme görünse de incelenen workflow'da `ctest` çalıştırıldığı kanıtlanmamıştır;
- upstream `NOTES.txt` ve issue'lar, form/action/annotation/content-edit boşlukları gösterir;
- 27 Nisan 2025 MIT yeniden lisanslaması için geçmiş katkı provenance'ı ayrıca taranmalıdır.

Bu nedenle nihai teknik karar şöyledir:

> **Kabul edilen rota:** Gelecekte marka ve iş modeli esnekliğini koruyan MIT/permissive ürün; PDF4QT'yi exact release'ten Gate 1 kabul testine al, geçerse fork et ve güvenlik/kalite kabuğunu aşamalı olarak güçlendir. Geçmezse hazır bileşenlerle PDFium + qpdf + Tesseract tabanlı modüler uygulamaya dön. KillerPDF/GPLv3 mevcut ürün rotasının parçası değildir.

Open PDF Studio, Stirling-PDF, ONLYOFFICE, SumatraPDF, Okular ve diğer projelerden kod “karıştırılmaz”. Lisans ve mimari uygunluğa göre yalnız davranış/UX referansı veya dar, bağımsız bileşen olarak değerlendirilir.

## 2. Önce ürün lisansı, sonra kod

“Ücretsiz” bir fiyat kararıdır; açık/kapalı kaynak ve türev eser yükümlülüğünü çözmez.

### Rota A — kabul edilen: permissive esneklik

- Ürün ücretsiz kalabilir.
- Çekirdek veya bütün kaynak istenirse açık tutulabilir.
- Gelecekte ücretli destek, kurumsal özellik, OEM veya kapalı modül seçeneği korunur.
- Birincil aday PDF4QT'dir (MIT); Qt ve diğer üçüncü tarafların kendi yükümlülükleri ayrıca uygulanır.
- GPL/AGPL kod, process sınırı kullanmakla otomatik olarak güvenli hale gelmez; dağıtım/iletişim biçimi hukuk incelemesi ister.

`ADR-0001` ile root MIT, inbound MIT + DCO 1.1 ve resmî ücretsiz/açık çekirdek kabul edilmiştir. Haricî katkı intake hattı M0'a kadar kapalıdır.

### Rota B — mevcut ürün için reddedilen: GPLv3 topluluk ürünü

- Türetilen ve dağıtılan uygulamanın karşılık gelen kaynak kodu GPLv3 koşullarıyla sunulur.
- Kendi marka/adınız kullanılabilir; fakat telif, lisans ve notice'lar korunur; upstream trademark/asset hakları ayrı incelenir.
- KillerPDF hız avantajı sağlayabilir.
- İleride kapalı OEM/enterprise dalı açmak, hak sahiplerinin ayrıca izin vermediği kodlarda mümkün olmayabilir.

Bu rota yeni ve açık bir superseding ADR olmadan yeniden açılamaz; KillerPDF mevcut Gate 1 bake-off'una girmez.

### Gate 0 çıkışı

`docs/adr/0001-product-license-model.md` Rota A ile **Accepted** olmuştur. Upstream ürün kodu yine `ADR-0002` Gate 1 kanıtıyla Accepted olmadan bu depoya alınmaz.

Bu deponun outbound MIT ve inbound MIT+DCO 1.1 politikası seçilmiştir. DCO otomasyonu ve review sahipliği M0'da kurulana kadar haricî katkı kabul edilmez.

## 3. Hazır taban seçimi

### 3.1 Aday sınıflandırması

| Aday | Hazır ürün kapsamı | Lisans/marka esnekliği | Başlıca risk | Karar |
|---|---|---|---|---|
| PDF4QT v1.6.0.0 | Viewer, editor, page tools, diff, CLI, forms, annotation, imza iddiaları | Güçlü: MIT çekirdek; Qt/third-party yükümlülükleri var | Tek geliştirici, native parser güvenliği, test/conformance kanıtı sınırlı | **Gate 1 birincil fork adayı** |
| KillerPDF v1.8.2 | WPF uygulama + ayrı .NET engine; geniş özellik/test iddiası | GPLv3; kabul edilen permissive rota ile uyumsuz | Nisan 2026'da doğmuş çok genç proje; büyük UI dosyaları; engine iddiaları bağımsız kanıtlanmalı | **REFERENCE / mevcut rota için reject** |
| PDFium + qpdf + Tesseract | Hazır motorlar; ürün UI/workflow yok | Güçlü permissive temel | Ürün kabuğu ve edit orkestrasyonu bize kalır | **PDF4QT elenirse güvenli yedek** |
| Open PDF Studio v1.92.0 | Geniş Tauri/SolidJS ürün iddiası | Kök LGPL, manifest MIT ve doğrudan AGPL MuPDF bağımlılığı arasında belirsizlik | Çok genç; provenance/lisans çelişkisi | **Şimdilik kod alma, izle** |
| Stirling-PDF current main | Çok geniş araç kataloğu | Önemli desktop/engine yollarında özel kısıtlayıcı lisans | White-label/production dağıtım hakkı belirsiz veya kısıtlı | **Yalnız workflow referansı** |
| SumatraPDF / Okular | Olgun viewer deneyimi | GPL/AGPL ve karma bağımlılıklar | Tam içerik editorü değiller | **UX ve davranış referansı** |
| MuPDF | Çok güçlü render/edit motoru | AGPL-3.0 veya ticari lisans | Permissive/kapalı rota ile uyumsuz | **Lisans alınmadıkça hariç** |
| ONLYOFFICE Desktop | Geniş gerçek edit yetenekleri | AGPL + ek görünür atıf/trademark şartları | Devasa office-suite mimarisi | **Taban değil, UX referansı** |

### 3.2 Gate 1 — kör fork yerine bake-off

PDF4QT v1.6.0.0 Windows 11 x64 ortamında aşağıdaki öldürücü kapılarla sınanır. KillerPDF kabul edilen rota nedeniyle bake-off kapsamına alınmaz.

#### A. Hukuk ve provenance

- Exact tag/commit ve bütün submodule/package/binary/model/font bağımlılıkları çıkarılmış olmalı.
- Lisans metni, dosya/dizin istisnaları ve dağıtım yükümlülükleri doğrulanmalı.
- PDF4QT'nin MIT yeniden lisanslamasında tarihsel katkı sahipliği otomatik tarama + mümkünse hukuk incelemesinden geçmeli.
- Qt modül lisansları, dinamik linking/relink yolu ve source/offer yükümlülüğü kanıtlanmalı.
- Ürün adı/logo/trademark ve installer kimlikleri temiz biçimde değiştirilebilir olmalı.

#### B. Tekrarlanabilir kurulum

- Temiz Windows ortamında belgelenmiş tek komut zinciriyle build.
- Lock/pin olmadan ağdan “latest” çekilmemeli.
- İki temiz runner'ın imzasız payload sonuçları karşılaştırılmalı.
- Unit/integration testleri gerçekten çalışmalı; yalnız compile yeşili kabul edilmez.

#### C. Zorunlu iş akışları

- Açma/render/zoom/scroll/search/select/copy
- Page reorder/rotate/delete/extract/merge/split
- Annotation create/edit/delete + undo/redo
- AcroForm text/check/radio/combo/button fill; appearance doğru save/reopen
- Encrypted PDF açma/kaydetme davranışı
- Türkçe font, bozuk font, CJK/RTL smoke test
- Signed PDF'nin algılanması ve varsayılan salt-okunur davranış
- Crash recovery ve atomik save feasibility PoC'u; production davranışı M1'de uygulanır

#### D. Preservation ve güvenlik

- Dokunulmayan sayfaların render/semantic farkı kabul bütçesinde kalmalı.
- Outline, metadata, attachment, tag tree, form appearance, annotation ve bilinmeyen nesnelerin korunma matrisi çıkarılmalı.
- Bozuk, çok büyük ve düşmanca örneklerde hang/OOM/UI crash olmamalı.
- Parser/render'ın ayrı worker sınırına alınabilirliği bir spike ile gösterilmeli.
- JavaScript/Launch/external file/network/XFA varsayılan deny olmalı veya kapatılabilirliği kanıtlanmalı.

#### E. Değiştirilebilirlik

- Marka, installer, app ID ve update endpoint'i tek yerde yapılandırılmalı.
- Bir örnek araç uçtan uca eklenip test edilerek modülerlik ölçülmeli.
- Büyük/tightly-coupled UI ve motor dosyaları haritalanmalı; upstream merge maliyeti tahmin edilmelidir.

#### Gate 1 sonucu

- **Geçer:** exact tag/commit fork edilir; ilk release yeni özellik eklemeden yalnız temiz rebrand + güvenilir build olur.
- **Koşullu geçer:** riskler sahip/tarih/mitigation ile kayıt altına alınır; güvenlik veya veri kaybı riski “sonra bakarız” olamaz.
- **Kalır:** kod içe alınmaz; PoC dalı arşivlenir ve yedek rota başlar.

## 4. Hedef ürün mimarisi

Fork edilen taban ilk gün yeniden yazılmaz. Önce çalışan davranış dondurulur, sonra sınırlar oluşturulur.

```text
Windows UI / ViewModel
        |
Application use-cases + capability policy
        |
DocumentSession + Command Journal + Undo/Redo
        |
Engine adapters (tek mutasyon sahibi)
        |
Low-privilege workers: parse/render | OCR | conversion | signature
        |
Atomic save/recovery + validation + audit-safe diagnostics
```

### Zorunlu sınırlar

- **DocumentSession:** Açık belgenin kimliği, revision'ı, dirty state'i, imza durumu, destek/koruma uyarıları ve recovery kaydının tek sahibi.
- **Command modeli:** Kullanıcı mutasyonları deterministik komutlardır; undo/redo ve crash journal aynı semantiği kullanır.
- **Tek writer:** Aynı revision'ı yalnız bir engine yazar. Başka motora geçiş dosya sınırında yapılır.
- **Worker isolation:** UI PDF byte'ını yorumlamaz; worker timeout, bellek/CPU/child-process kotalarıyla çalışır. AppContainer/Windows App Isolation uygunluğu Gate 1 feasibility spike'ında ölçülür, production sınırı M1'de uygulanır.
- **Atomik save:** Orijinal asla çalışma buffer'ı değildir. Temp → flush → reopen → validate → gerekiyorsa render/semantic diff → replace; hata halinde orijinal ve recovery kalır.
- **Capability policy:** Her dosya için desteklenen, kayıplı ve engellenen işlemler görünürdür.
- **Provider adapter:** OCR, imza, conversion ve ileride AI motorları değiştirilebilir arayüz arkasındadır.

Detaylar [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md) içindedir.

## 5. Özellik sırası

### Temel 0 — güvenli ürün tabanı

Rebrand, installer, hata raporu gizliliği, worker prototipi, atomik save, recovery, corpus harness, dependency pin/SBOM. Bunlar “altyapı işi” değil ürünün güvenilirlik özelliğidir.

### Temel 1 — kusursuz okuyucu

Hızlı açılış, büyük belge sanallaştırma, zoom/scroll, thumbnail, text search/select/copy, outline, links için güvenli onay, keyboard/high-contrast/UIA, print.

### Temel 2 — günlük düzenleme

Page operations, annotation, overlay text/image, crop, metadata, attachments, undo/redo, autosave/recovery. “Overlay” kullanıcıya gerçek içerik düzenleme diye sunulmaz.

### Temel 3 — formlar ve tarama

AcroForm doldurma, appearance generation, flatten kopyası, form tasarımının sınırlı ilk seviyesi; ayrı OCR worker, `tur+eng`, confidence/bbox ve aranabilir katman.

### Profesyonel 1 — zor belge işlemleri

Gerçek redaction, güvenli sanitize kopyası, daha ileri content-object edit, compare, batch, PDF/A profil dönüşümü. Her biri kayıp raporu ve bağımsız doğrulama ister.

### Profesyonel 2 — güven ve erişilebilirlik

Revision modeli olgunlaştıktan sonra dijital imza doğrulama, PAdES B/T; sonra LT/LTA araştırması. Tag koruma/iyileştirme, PDF/UA remediation ve insan + ekran okuyucu test hattı.

### Ekosistem — en son

CLI/automation API, güvenli plugin modeli, isteğe bağlı yerel AI yardımcıları, kurumsal entegrasyon. Plugin ve AI belgelere sınırsız erişmez; capability/permission modeli ve ayrı threat model gerekir.

## 6. Kalite tanımı

Bir özellik ancak şu zincir tamamlandığında “destekleniyor” olabilir:

```text
REQ → normatif/resmî kaynak → ADR (gerekiyorsa) → kod → fixture/test → son doğrulama sonucu
```

Asgari her mutasyon testi:

1. Girdiyi aç.
2. Tek tanımlı işlemi uygula.
3. Yeni dosyaya kaydet.
4. Kapatıp yeniden aç.
5. Yapısal doğrula.
6. Etkilenen sayfaları render-diff et.
7. Text/metadata/form/tag/attachment/imza gibi semantik yapıları karşılaştır.
8. Başka okuyucularda smoke test yap.

Release kapıları ve corpus ayrıntısı [docs/quality/QUALITY_AND_TESTING.md](docs/quality/QUALITY_AND_TESTING.md) dosyasındadır.

## 7. Güvenlik ve mahremiyet

Her PDF, yerel diskten gelse bile güvensiz girdidir. Varsayılan politika:

- PDF/OCR/conversion worker'larında ağ yok; belge içeriği telemetri yok; imza revocation/timestamp için yalnız isteğe bağlı ve endpoint-kısıtlı TrustNetworkBroker istisnası;
- JavaScript, Launch, dış dosya, UNC, XFA, rich media ve 3D deny;
- URI allowlist + hedefi açık gösterme + kullanıcı onayı;
- attachment otomatik açılmaz;
- parse/render/OCR/imza ana UI dışında;
- boyut, sayfa, nesne, recursion, dekompresyon, resim pikseli, süre ve RAM kotası;
- parola/anahtar/metin/PDF byte loglanmaz;
- updater yalnız imzalı, hash'li, rollback/freeze korumalı artifact kabul eder.

Detay ve threat model [docs/SECURITY_AND_PRIVACY.md](docs/SECURITY_AND_PRIVACY.md) içindedir.

## 8. Standart iddiaları

- Normatif taban ISO 32000-2:2020 + yayımlanmış errata'dır.
- “PDF 2.0 destekli” tek onay kutusu yoktur; okuma/render/yazma/koruma/doğrulama ayrı izlenir.
- PDF/A export ayrı, kayıplı olabilen dönüştürme işidir; hedef profil ve veraPDF sonucu raporlanır.
- PDF/UA otomatik validator sonucu değildir; PDF/UA-1 için Matterhorn/reference-suite, PDF/UA-2 için ISO 14289-2 tabanlı ayrı insan kontrolü ve her profile assistive-technology testi gerekir.
- PAdES sonucu tek yeşil rozet değildir: byte integrity, post-sign change, crypto, chain, trust, revocation, timestamp ve qualification ayrıdır.

Başlangıç matrisi [docs/standards/PDF_SUPPORT_MATRIX.md](docs/standards/PDF_SUPPORT_MATRIX.md), iddia ve doğrulama politikası [docs/standards/CONFORMANCE_POLICY.md](docs/standards/CONFORMANCE_POLICY.md) dosyasındadır.

## 9. Windows dağıtımı

`ADR-0003` ile ilk kanal Microsoft Store üzerinden MSIX olarak kabul edilmiştir: son kullanıcı güveni, imzalama ve güncelleme yükünü azaltır. Doğrudan indirme ileride açılırsa kamuya güvenilen code-signing ve RFC 3161 timestamp gerekir; self-signed sertifika son kullanıcı çözümü değildir.

Store paketli olmak parser sandbox'ı anlamına gelmez. Uygulamanın full-trust süreçleri ayrıca izole edilir.

Her release:

- sabit toolchain/dependency;
- temiz runner build'i;
- mümkünse iki runner'da imzasız payload karşılaştırması;
- SPDX veya CycloneDX SBOM;
- third-party notices/lisanslar;
- SHA-256 toplamları;
- provenance/attestation;
- EXE/DLL/MSIX/updater imzası + timestamp;
- staged rollout ve rollback planı üretir.

## 10. Yapay zekâ hafızası ve halüsinasyon savunması

Tek bir dev `MASTER.md` veya sohbet geçmişi hafıza değildir. Kalıcı hafıza Git'teki ayrıştırılmış belgelerdir:

- `AGENTS.md`: kısa anayasa ve yönlendirici
- `PROJECT.md`: değişmeyen ürün niyeti
- `STATUS.md`: bugünkü durum ve yalnız sonraki işler
- `SOURCES.md` / registry: doğrulanmış dış gerçekler
- ADR: neden bu karar verildi
- requirements/support matrix: neyi vaat ediyoruz ve kanıtı
- engineering log: ne yapıldı/çalıştırıldı
- Git diff: tam değişiklik
- `CHANGELOG.md`: yalnız kullanıcıya görünür davranış

Codex kök/yakın `AGENTS.md` dosyalarını; Claude `CLAUDE.md`; Gemini `GEMINI.md` kullanır. Vendor dosyaları gerçeği kopyalamaz, kök sözleşmeyi import eder. Tekrarlanan işler `.agents/skills/` altında dar ve tetiklenebilir becerilerdir; ürün mimarisi `SKILLS.md` içine gömülmez.

AI, kanıt olmadan sürüm/lisans/API/uyumluluk uyduramaz; lisans kabul edemez; güvenlik sınırını/testi kapatamaz; golden baseline'ı kendi başına yenileyemez; release key veya update root'unu değiştiremez. Ayrıntı [docs/AI_WORKFLOW.md](docs/AI_WORKFLOW.md) içindedir.

## 11. En tehlikeli yanlış yollar

1. Sırf çok özellik var diye güncel Stirling-PDF veya lisansı çelişkili Open PDF Studio kodunu kopyalamak.
2. “Ücretsiz” olduğu için GPL/AGPL/LGPL yükümlülüğünün yok olduğunu sanmak.
3. Baştan yeni Tauri/Electron/WPF arayüzü yazıp çalışan ürün avantajını kaybetmek.
4. PDF4QT'yi hiç corpus/fuzz/security kapısı olmadan körlemesine rebrand etmek.
5. Birden fazla PDF motorunun aynı belgeyi sırayla/aynı anda bilinçsizce yazmasına izin vermek.
6. İlk sürüme reflow text edit, PAdES-LTA, PDF/UA ve plugin/AI ekleyerek güvenilir temel oluşmadan kapsamı patlatmak.
7. Siyah rectangle'ı redaction; çizilmiş imzayı dijital imza; PDF açılmasını conformance olarak pazarlamak.
8. Kullanıcı belgesini debug log, crash dump, bulut analizi veya AI eğitim verisine sızdırmak.

## 12. İlk somut yürütme emri

1. `ADR-0001`, `ADR-0003` ve `ADR-0004` kabul edildi; Gate 0 ürün rotası sabitlendi.
2. Şimdi `ADR-0002` kriterleriyle PDF4QT bake-off audit çalışma alanı ürün deposu dışında kurulur; ürün deposuna kod merge edilmez.
3. Kaynak ve dependency envanteri üretilir; lisans/provenance kırmızıysa teknik PoC durur.
4. Temiz Windows 11 x64 build + gerçek unit test + 25 dosyalık smoke corpus tamamlanır.
5. Atomik save ve worker-isolation spike'ı yapılır.
6. Sonuç `ADR-0002` içine kanıtlarla yazılır; geçerse exact tag/commit fork edilir.
7. Product owner nihai adı M0 rebrand başlamadan seçer. İlk milestone yalnız baseline dondurma, yeni marka ve tekrarlanabilir build'dir; yeni özellik eklenmez.

Planın ilerleme sırası ve gate çıktıları [ROADMAP.md](ROADMAP.md) dosyasında bağlayıcı hale getirilmiştir.
