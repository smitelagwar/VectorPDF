# PDF Masaüstü Uygulaması — Claude'un Sentezi, Kaynak Doğrulaması ve GPT-5.6 Ultra İçin Referans Doküman

> **Hazırlayan:** Claude (Anthropic) · **Tarih:** 31 Ağustos 2026
> **Amaç:** Bu belge, kullanıcının GPT-5.6 "Sol" Ultra moduna göndereceği nihai talebe **referans/girdi** olarak hazırlanmıştır. Kullanıcının şu ana kadar üç farklı yapay zekadan aldığı MD dosyalarını (`pdf-uygulamasi-yol-haritasi.md`, `Windows_PDF_Araci_GitHub_Arastirmasi_ve_Gelistirme_Ana_Plani.md`, `PDF_Araci_Gelistirme_Yol_Haritasi.md`) tek tek okudum, aralarındaki **çelişkileri gerçek web araması ile doğruladım**, ve kullanıcının istediği asıl yeni katmanı — **halüsinasyon azaltan, hafızası olan, değişiklik geçmişini tutan bir MD-dosya mimarisi** — bu belgede somut şablonlarla veriyorum.
>
> Diğer üç belgeyi tekrar özetlemiyorum; onlar zaten kapsamlı. Burada **onlarda bulduğum hatalar**, **benim eklediğim doğrulanmış araştırma** ve **istediğin "hafıza sistemi"** var.

---

## 1. ÖNCE EN KRİTİK KISIM: Üç dokümanı doğruladım, işte bulduklarım

Bu, tam olarak neden böyle bir sisteme ihtiyacın olduğunun kanıtı. Üç AI'ın çıktısını GitHub'da tek tek kontrol ettim:

| Konu | Hangi dosya ne dedi | Gerçek durum (31 Ağustos 2026'da doğrulandı) |
|---|---|---|
| **PDF4QT — GitHub kullanıcı adı** | Doküman 2: `JakubMelka/PDF4QT` ✅ · Doküman 3: `JurajKoval/PDF4QT` ❌ | Gerçek repo: **`github.com/JakubMelka/PDF4QT`**. Doküman 3'teki kullanıcı adı yanlış/uydurma. |
| **PDF4QT — lisans** | Doküman 2: "çekirdek MIT" ❌ · Doküman 3: "LGPLv3" ✅ | Proje resmi sitesinde açıkça yazıyor: **"operates under the LGPLv3 license."** MIT değil. Doküman 2 burada yanılmış — ve bu senin gelecekteki lisans kararını doğrudan etkileyecek bir hata. |
| **Stirling-PDF — lisans modeli** | Doküman 3: "GPLv3" ❌ · Doküman 1 ve 2: karışık/open-core ✅ | Gerçek: **açık çekirdek MIT**, ama `app/proprietary/`, `frontend/editor/src/desktop/`, `src/saas/`, `src/cloud/` gibi klasörler ayrı, ticari lisanslı (open-core model, "Stirling PDF Inc." telifli). Ayrıca artık **resmi bir Tauri tabanlı masaüstü istemcisi** de var — bu üç dokümanda da geçmiyor, aşağıda Bölüm 4'te işliyorum. |
| **KillerPDF'in gerçekliği** | Doküman 2: çok detaylı, kuşkulu derecede spesifik rakamlar (1.436 test, 2.907 dosyalık corpus) | **Doğrulandı — gerçek proje.** `github.com/SteveTheKiller/KillerPDF`, GPLv3, aktif geliştiriliyor (29 Ağustos 2026'da v1.8.1 yayınlanmış), 3.700+ yıldız, 279 fork. Doküman 2'nin genel tasviri isabetli. Bu sefer "çok spesifik = uydurma" varsayımım yanlış çıktı — bu da kaydedilmesi gereken bir ders: **spesifiklik tek başına güvenilirlik kanıtı değil, ama tek başına şüphe kanıtı da değil; asıl kanıt doğrulamadır.** |
| **Azure Artifact Signing (Trusted Signing) fiyatı ve Türkiye durumu** | Doküman 1: "$9.99/ay, bireysel geliştiriciler için Türkiye şu an kapsam dışı" | **Doğrulandı, doğru.** Microsoft'un resmi dokümantasyonu: bireysel geliştiriciler için sadece ABD ve Kanada; kurumlar için ABD, Kanada, AB, İngiltere + (Public Trust için ayrıca Avustralya, Yeni Zelanda, Japonya, Güney Kore, Singapur, İsviçre, Norveç, İsrail). **Türkiye'den bireysel başvuru mümkün değil** — sadece Türkiye'de kurulu bir şirketin de işini görmeyeceğini unutma, liste net. |

**Bunun senin için pratik sonucu:** Hiçbir AI çıktısına (benimkiler dahil) doğrulamadan güvenme — özellikle lisans, fiyat ve "bu repo şunu yapabiliyor" gibi iddialarda. Bölüm 5'te bunu otomatikleştiren bir dosya sistemi (`SOURCES.md`) öneriyorum; tam olarak yukarıdaki gibi hataları önlemek için.

---

## 2. Kendi geçmişinden bir hatırlatma — çözülmesi gereken bir çelişki

Daha önce benimle konuştuğun bir oturumda, **"PDFusion"** adında, **PySide6 + pypdfium2** ile Adobe Acrobat tarzı arayüze sahip bir masaüstü uygulaması planladığını ve bunu Antigravity üzerinden geliştirmeye başladığını belirtmiştin.

Şu an elindeki üç yeni doküman ise tamamen farklı üç yığın öneriyor: Tauri+React+JS (Doküman 1 ve 3) veya C#/WPF (KillerPDF fork, Doküman 2) veya C++/Qt (PDF4QT fork, Doküman 2 ve 3). **Python/PySide6 hiçbirinde yok.**

Bu, GPT-5.6'ya göndermeden önce netleştirmen gereken bir karar: PDFusion'ı bu projenin önceki bir iterasyonu olarak mı görüyorsun (yani tamamen terk edip yeni yığına mı geçiyorsun), yoksa bu üç doküman PDFusion'dan habersiz, ayrı bir hat mı? İkisini paralel sürdürmek — aynı anda iki farklı teknoloji yığınında aynı ürünü geliştirmeye çalışmak — hem senin hem AI ajanlarının kafasını en çok karıştıracak şey olur. GPT-5.6'ya vereceğin nihai talepte bu kararı açıkça yaz.

---

## 3. Üç yol var — kendi değerlendirmem

| | **A) KillerPDF fork** (GPLv3, C#/WPF) | **B) PDF4QT fork** (LGPLv3, C++/Qt) | **C) Modüler hibrit — sıfırdan kabuk** (Tauri/React + izin verici lisanslı kütüphaneler) |
|---|---|---|---|
| Ürüne en hızlı ulaşma | En hızlı — zaten çalışan, özellik dolu bir uygulama | Hızlı — olgun bir viewer/editor var | En yavaş — her şeyi sen/AI kurar |
| Marka/telif özgürlüğü | Kısıtlı — türev GPLv3 kalır, kaynağı asla kapatamazsın | Orta — LGPLv3, GPL'den esnek ama yine copyleft: PDF4QT'nin kendi koduna yaptığın değişiklikler açık kalmalı (sen doğrudan uygulamayı değiştiriyorsan bu seni kapsar) | Tam — Apache/MIT tabanlı kütüphaneler, istediğin an kapalı/ticari katman ekleyebilirsin |
| AI-ajanla (Claude Code, Codex, GPT-5.6) geliştirme güvenliği | Yüksek — C#/.NET, modern AI ajanların en rahat çalıştığı ekosistemlerden biri | **Düşük-orta** — C++ bellek yönetimi + Qt/CMake/vcpkg build zinciri, AI ajanların en çok "sessiz hata" ürettiği alanlardan biri | Yüksek — TypeScript/React + Rust (ince katman); pdf.js/pdf-lib gibi kütüphaneler iyi belgeli, AI'ların eğitim verisinde bol örnek var |
| İleride "belki ücretli Pro katman" opsiyonu | **Kapalı kapı** — GPLv3 bunu pratikte imkansızlaştırır | Kısmen açık — bileşen mimarisine bağlı | Tam açık |
| Risk | Genç proje (Nisan 2026 doğumlu), mimari hâlâ hareketli olabilir | C++/Qt öğrenme eğrisi + gerçek editör hâlâ olgunlaşıyor (bkz. PDF4QT issue #337, #410 — content-editor hataları açık) | "Gerçek Acrobat editörü" hedefine en uzun yoldan varır |

**Benim tavsiyem: C — modüler hibrit, sıfırdan kabuk.** Gerekçem üçe dayanıyor:

1. **Sen açıkça "benim seçeceğim isimle, tamamen benim istediğim şekilde" dedin ve ileride "Adobe Acrobat gibi bir sürü özellik" istiyorsun.** Bu, marka ve lisans özgürlüğünü en baştan garantiye almanı gerektirir. GPLv3 fork'u bunu geri dönülemez şekilde kapatır; "ücretsiz kalacağım zaten" dediğin bugün doğru olsa bile, 2-3 yıl sonra fikrini değiştirme hakkını bugünden satmış olursun.
2. **AI-ajanla geliştirme güvenliği gerçek bir mühendislik kriteri, süs değil.** C++/Qt/CMake/vcpkg zinciri, bir AI ajanının "derledi ama bozdu" ya da "bellek hatası ekledi ama fark etmedi" riskinin en yüksek olduğu ortamlardan biri. Sen kod yazmayacaksın, AI yazacak — o zaman AI'ın en az hata yaptığı ekosistemi seçmek stratejik bir karar, sadece teknik tercih değil.
3. **KillerPDF ve PDF4QT'yi "kod tabanı" değil "özellik/UX referansı" olarak kullanabilirsin** — ikisi de gerçek, aktif, iyi dokümante projeler. Onların özellik listelerini, menü yapısını, hangi sırayla neyi çözdüklerini incelemek tamamen serbest (fikir/özellik telif altında değil); kodlarını kopyalamak değil.

**Ama bu benim tercihim, senin kararın değil.** GPT-5.6'ya gönderirken bu üç yolu ona da göster ve "ben C'yi seçtim, sebebi şu" de — ya da onun karşı argümanını iste. Karar D0 niteliğinde bir şey, ucu açık bırakma (Doküman 2'nin de vurguladığı gibi).

---

## 4. Doğrulanmış kütüphane/repo matrisi

Aşağıdaki tablo üç dokümanın ortak önerdiklerini + benim doğruladığım güncel bilgileri birleştiriyor.

| Bileşen | Ne işe yarar | Lisans | Not |
|---|---|---|---|
| **pdf.js** (Mozilla) | Görüntüleme, metin katmanı, arama, temel annotation | Apache-2.0 | Firefox'un kendi PDF görüntüleyicisi; en olgun seçenek |
| **PDFium** (Google/Chromium) | Yüksek performanslı native rendering | BSD tarzı, izin verici | Chrome'un motoru; masaüstü katmanında native render için |
| **pdf-lib** | Oluşturma, birleştirme, bölme, form doldurma, sayfa işlemleri | MIT | **Mevcut sayfa metnini doğrudan düzenleyemiyor** — sadece form alanlarını. Resmi dokümantasyonları bunu açıkça belirtiyor. "Gerçek metin düzenleme" hedefini buna göre kur (bkz. Bölüm 7). |
| **qpdf** | Onarım, doğrusallaştırma, şifre çözme/şifreleme, düşük seviye yapı işlemleri | Apache-2.0 | Endüstri standardı, C++ |
| **pikepdf** | qpdf'nin Python sarmalayıcısı | MPL-2.0 | Python tarafı tercih edilirse |
| **Tesseract / tesseract.js** | OCR | Apache-2.0 / MIT | WASM derlemesi native kurulum istemez |
| **PdfPig** | .NET tabanlı metin/koordinat çıkarma | Apache-2.0 | C#/.NET yoluna gidilirse faydalı |
| **Tauri v2** | Masaüstü kabuk | MIT/Apache-2.0 | ~10-18 MB kurulum, ~35-80 MB boşta RAM — Electron'a göre çok daha hafif |
| **KillerPDF** (`SteveTheKiller/KillerPDF`) | Referans/benchmark — kod tabanı değil | GPLv3 | Doğrulandı, aktif, WPF/.NET; özellik listesi ve UX akışı için incele |
| **PDF4QT** (`JakubMelka/PDF4QT`) | Referans/benchmark — kod tabanı değil | **LGPLv3** (Doküman 2'nin "MIT" iddiası yanlış) | Doğrulandı, aktif, C++/Qt; **Türkçe dil dosyası zaten var** (issue #304'te "Turkish language file is ready" — Türkçe kullanıcı katkısı bile geçmiş, iyi bir işaret) |
| **Stirling-PDF** (`Stirling-Tools/Stirling-PDF`) | Özellik envanteri referansı + **artık resmi Tauri masaüstü istemcisi de var** | MIT çekirdek + ayrı lisanslı proprietary/desktop/saas/cloud klasörleri | 81k+ yıldız, en büyük topluluk; "hangi özellik nasıl konumlandırılmalı" sorusuna iyi bir cevap kaynağı |
| **PDF Arranger** | Referans — sayfa sürükle-bırak UX'i | GPL | Python/GTK/pikepdf; sayfa manipülasyon algoritması için iyi örnek |
| **SumatraPDF** | Referans — saf hız/bellek optimizasyonu | GPLv3 (çekirdekte MuPDF) | Ne kadar hafif bir viewer olunabileceğinin kanıtı |

**Kaçın:** `MuPDF`/`PyMuPDF` (AGPL-3.0), `Ghostscript` (AGPL/ticari çift lisans) — ikisi de projenin marka/lisans özgürlüğünü kapatan copyleft riskleri taşıyor. Üç dokümanın da hemfikir olduğu tek şey bu ve doğru.

---

## 5. İstediğin asıl şey: Halüsinasyon azaltan, hafızası olan, log tutan MD dosya mimarisi

Bu bölüm, üç yüklediğin dokümanın **hiçbirinde yok** — sen bunu özellikle benden istedin, o yüzden burası bu belgenin ana katkısı.

### 5.1 Mevcut alışkanlığını koruyoruz, üstüne inşa ediyoruz

Diğer projelerinde (mühendislik sitesi vb.) zaten `CLAUDE.md` / `AGENTS.md` / `GEMINI.md` / `PROJECT.md` düzenini kullanıyorsun. Bu iyi bir refleks — sıfırdan farklı bir sistem önermiyorum, üstüne üç eksik parçayı (karar günlüğü, değişiklik günlüğü, kaynak doğrulama) ekliyorum.

### 5.2 Güncel gerçek: `AGENTS.md` artık resmi bir endüstri standardı

Bunu bilmen önemli çünkü GPT-5.6, Claude Code, Codex, Gemini CLI gibi araçların hepsi bunu artık native destekliyor:

- `AGENTS.md`, Ağustos 2025'te OpenAI, Google, Cursor ve Factory'nin ortak çalışmasıyla resmileşti; Aralık 2025'te Linux Foundation'ın **Agentic AI Foundation**'ına devredildi.
- 2026 itibarıyla 60.000'den fazla açık kaynak repo bu formatı kullanıyor; 20'den fazla AI aracı (Codex, Cursor, Gemini CLI, Windsurf, Aider, Zed, Devin, Amp, JetBrains Junie, vb.) native destekliyor.
- **Claude Code hâlâ `CLAUDE.md` okuyor** ama önerilen yöntem `CLAUDE.md` içinden `AGENTS.md`'yi import etmek (`@AGENTS.md` satırı) — böylece tek bir gerçek kaynağın olur, iki dosyayı senkron tutmak zorunda kalmazsın.
- Etkili `AGENTS.md` dosyalarının ortak özelliği (2.500+ repo analizinden): **tam komutlar** ("`npm run test:unit`", "önce testleri çalıştır" değil), **sadece varsayılandan sapan kurallar** (herkesin zaten bildiğini yazma), **gerçek kod örnekleri** (uzun açıklama yerine), **açık sınırlar** ("`/vendor/` klasörüne asla dokunma", "sır/anahtar commit etme"). Mimari genel bakış bölümleri araştırmalarda **etkisiz** bulunmuş — çıkar, yerine komutlar ve kısıtlar koy.
- **Önemli uyarı:** Araştırmalar, bir AI aracının kendi ürettiği (`/init` gibi otomatik komutlarla) context dosyalarının **performansı düşürdüğünü** gösteriyor — ajan, gereksiz ayrıntıları sadık bir şekilde takip edip gereksiz yere geniş arama yapıyor. Yani: bu dosyaları bir AI'a "sen yaz" deyip bırakma; sen (ya da güvendiğin bir AI oturumu) gözden geçirip budamalı.

### 5.3 Önerilen dosya seti

Endüstride yaygın "Memory Bank" desenini (Cline/Cursor/Copilot ekosistemlerinde standartlaşmış) senin mevcut alışkanlığınla birleştirdim. **Aşırı mühendislik yapma** — "sadece eksikliğini hissettiğinde dosya ekle" prensibiyle başla:

```
/repo-kök/
├── AGENTS.md          ← TÜM AI ajanları için ortak operasyonel talimat (kaynak dosya)
├── CLAUDE.md           ← Sadece "@AGENTS.md" import + Claude'a özgü notlar (varsa)
├── GEMINI.md            ← Aynı mantık, Gemini'ye özgü notlar (varsa)
├── PROJECT.md           ← Vizyon, marka kararı, "neden bu proje var", mevcut faz
├── DECISIONS.md         ← Karar günlüğü (ADR tarzı) — NE karar verildi, NEDEN, hangi alternatif elendi
├── CHANGELOG.md         ← Gerçekleşen değişikliklerin kronolojik kaydı (Keep a Changelog formatı)
├── SOURCES.md           ← Her önemli teknik/lisans iddiası için: doğrulanmış mı, kaynak, tarih
├── STATUS.md            ← Şu an ne yapılıyor, sıradaki adım (1 ekrana sığacak kadar KISA)
└── docs/
    ├── ARCHITECTURE.md  ← (proje büyüyünce) katman/modül haritası — kısa tut, gezinme aracı değil
    └── CORPUS.md        ← (Faz 2'den itibaren) test PDF seti, kenar durumları, bilinen sorunlar
```

### 5.4 Her dosyanın ne içermesi, ne içermemesi gerektiği

**`AGENTS.md`** — kısa, komut odaklı, sıkıcı olmalı. Örnek iskelet:
```markdown
# AGENTS.md

## Build & Test
- Kurulum: `npm install`
- Geliştirme: `npm run tauri dev`
- Test: `npm run test` (tek dosya: `npm run test -- path/to/file.test.ts`)
- Build: `npm run tauri build`

## Kurallar (varsayılandan sapanlar)
- pdf-lib ile mevcut sayfa metnini DEĞİL, sadece form alanlarını değiştir (bkz. PROJECT.md §Bilinen Sınırlar)
- Yeni bir üçüncü parti kütüphane eklemeden önce SOURCES.md'de lisansını kaydet
- Orijinal dosyayı asla doğrudan overwrite etme — bkz. PROJECT.md §Kaydetme Kuralları

## Dokunma
- `/vendor/` klasörüne elle dokunma
- `.env`, imzalama sertifikaları, API anahtarlarını asla commit etme

## Öncelik sırası (çakışma olursa)
1. Bu dosyadaki güvenlik/kaydetme kuralları
2. DECISIONS.md'deki en güncel ilgili karar
3. PROJECT.md'deki genel yön
```

**`DECISIONS.md`** — her giriş şu formatta, en yeni en üstte:
```markdown
## 2026-09-03 — Masaüstü kabuk: Tauri v2 seçildi, Electron değil
**Karar:** Tauri v2.
**Neden:** Kurulum boyutu (~15 MB vs ~150 MB), izin sistemi varsayılan kısıtlı, PDF ayrıştırma güvenlik riski taşıyor.
**Elenen alternatif:** Electron — daha olgun ekosistem ama gereksiz büyük ayak izi.
**Kim önerdi / hangi oturum:** Claude (sentez dokümanı), GPT-5.6 Ultra onayladı.
**Geri dönülebilir mi:** Evet, ilk 2-3 hafta içinde Rust tarafı tıkanırsa Electron'a geçilebilir; sonrasında maliyeti artar.
```
Bu format, "bunu neden böyle yapmıştık?" sorusuna 6 ay sonra da cevap verir — Doküman 1'in "karar günlüğü" fikrini kalıcı hale getiriyor.

**`CHANGELOG.md`** — [Keep a Changelog](https://keepachangelog.com) formatı, insan-okunur, her sürüm/faz sonunda güncellenir. AI oturumu bir özellik bitirdiğinde otomatik ekler, sen elle düzenlemezsin.

**`SOURCES.md`** — Bölüm 1'deki hataları önleyen dosya. Her satır:
```markdown
- [DOĞRULANDI 2026-08-31] PDF4QT lisansı: LGPLv3 — kaynak: jakubmelka.github.io/ "operates under the LGPLv3 license"
- [İDDİA — doğrulanmadı] pdf-lib v-son sürüm metin düzenlemeyi destekliyor mu? → yeni özellik eklemeden önce kontrol et
```
Kural: **Kod içine yeni bir kütüphane, API, lisans iddiası veya "şu repo şunu yapabiliyor" cümlesi girmeden önce bu dosyaya bakılır; yoksa web'den doğrulanıp eklenir.** Bu, tam olarak Bölüm 1'deki KillerPDF/PDF4QT/Stirling-PDF karışıklığını önleyecek disiplin.

**`STATUS.md`** — en sık değişen dosya, kasıtlı olarak kısa: şu an hangi faz, hangi görev üzerinde çalışılıyor, bilinen açık blokajlar, sıradaki 1-3 adım. Büyüdüğü an CHANGELOG'a taşınır, burada birikmesine izin verilmez.

### 5.5 Oturum başı/sonu protokolü

Yeni bir AI oturumu (Claude Code, Codex, GPT-5.6 fark etmez) başladığında sırayla: `AGENTS.md` → `PROJECT.md` → `STATUS.md` → `DECISIONS.md`'nin son 3-5 girdisi. Oturum bitmeden önce: `STATUS.md` güncellenir, varsa yeni `DECISIONS.md` girdisi eklenir, `CHANGELOG.md`'ye yapılan iş yazılır. Bunu "hatırlarsan yaparım" değil, **her oturumun zorunlu son adımı** olarak GPT-5.6'ya ilet — sistem ancak disiplin bozulmazsa işe yarar.

---

## 6. UI/UX taktikleri (ek notlarım)

- **"Her özellik var" değil, "her özellik keşfedilebilir."** Doküman 2'nin bu noktası isabetli: özellik sayısını artırmak yerine bir komut paleti (Ctrl/Cmd+K tarzı arama) ekle — Acrobat'ın en büyük UX sorunu kalabalık menüler.
- **Kısayolları Acrobat'a kasıtlı olarak yakın tut.** Kullanıcıların çoğu Acrobat'tan geliyor olacak; `Ctrl+Shift+7` gibi alışılmış kombinasyonları yeniden icat etme, tanıdıklık geçiş sürtünmesini azaltır.
- **Her tehlikeli işlemde önizleme + güvenli varsayılan.** Özellikle redaksiyon, toplu silme, formu düzleştirme gibi geri dönüşü zor işlemler.
- **Sol panel: küçük resimler / sağ panel: seçili aracın özellikleri** — Doküman 3'ün önerdiği düzen endüstri standardıyla (Acrobat, Foxit, PDF4QT) uyumlu, kullanıcı tanıdıklığı için değiştirme.
- **Sürükle-bırak her yerde** çalışmalı: dosya açma, sayfa yeniden sıralama, görsel ekleme.
- **Otomatik taslak/kurtarma en baştan** — Doküman 2'nin "autosave/crash recovery" vurgusu doğru, ilk fazlardan itibaren olmalı, sona bırakılacak bir "nice to have" değil.
- **Türkçe + İngilizce baştan i18n dosyasında** tutulmalı (Doküman 1'in notu doğru) — sonradan eklemek pahalıya patlar.

---

## 7. Teknik tuzaklar — üç dokümanın ortak vurguladıkları + benim eklediklerim

Üçü de haklı olarak şunları vurguluyor, tekrar etmiyorum ama listeliyorum çünkü GPT-5.6'nın bunları atlamaması gerekiyor: PDF'in "kelime işlemci belgesi olmadığı" gerçeği, font subsetting/gömülü olmayan font sorunu, büyük dosyada RAM şişmesi (virtualized rendering şart), görsel imza ile dijital imza arasındaki fark, redaksiyonun "siyah kutu boyamak" olmadığı (içerik gerçekten silinmeli).

**Benim eklediklerim:**
- **Kaydetme güvenliği kırmızı çizgi olmalı.** Orijinal dosya asla doğrudan üzerine yazılmamalı; atomic save (geçici dosyaya yaz → doğrula → yer değiştir) + otomatik yedek. Bir PDF programının kullanıcı belgesini bozması, güven kaybının en hızlı yolu.
- **Türkçe karakter/glyph testi ayrı bir test kategorisi olmalı** — özellikle font gömme ve OCR Türkçe dil paketi tarafında (ı, ş, ğ, ç, ö, ü karakterlerinin özellikle eski/taranmış belgelerde ve bazı gömülü fontlarda sorun çıkarması yaygın).
- **KVKK bilgilendirmesi, herhangi bir telemetri/çökme raporu eklenmeden önce tasarlanmalı**, sonradan yama olarak değil (Doküman 1'in notu doğru ve önemli).
- **Lisans satırlarını `SOURCES.md`'ye işlemeden hiçbir üçüncü parti kod/kütüphane eklenmemeli** — Bölüm 1'deki hatalar tam olarak bunun eksikliğinden doğuyor.

---

## 8. Kısa faz iskeleti (ilkeler, GPT-5.6 kendi detaylı fazlarını üretecek)

Süre tahmini vermiyorum (AI destekli geliştirmede güvenilmez); bunun yerine **sıra** ve her fazın **"bitti" tanımı**:

1. **İskelet** — proje açılıyor/kapanıyor/çökmüyor, marka/isim netleşmiş, `AGENTS.md`+`PROJECT.md` kurulu.
2. **Görüntüleyici** — herhangi bir PDF'i SumatraPDF kadar rahat açıp gezebiliyorsun.
3. **Temel dosya işlemleri** — birleştir/böl/döndür/sırala/şifrele, Stirling-PDF'in temel araçlarının çoğunu karşılıyor.
4. **Form + imza** — AcroForm doldur, düzleştir, görsel + dijital imza (ikisi UI'da net ayrı).
5. **Annotation + overlay düzeltme** — vurgulama, not, çizim; "gerçek metin düzenleme" değil, dürüst bir "düzeltme" konumlandırması.
6. **OCR + temel dönüştürme** — taranmış belgede arama yapılabiliyor.
7. **Paketleme/dağıtım** — code signing (Türkiye'den Azure Artifact Signing bireysel olarak kapalı — bkz. Bölüm 1 — bu yüzden Microsoft Store/MSIX rotası öncelikli olmalı), otomatik güncelleme.
8. **Büyüme** — gerçek content-editing motoru, Pro katman (mimari buna baştan izin veriyorsa), bulut senkron, mobil companion.

---

## 9. GPT-5.6 Ultra'ya iletirken ekleyebileceğin yönlendirme

Bu belgeyi gönderirken şunu da ekle: *"Claude'un bu dokümanda bulduğu üç lisans/gerçeklik hatasını (Bölüm 1) referans al ve kendi nihai planında aynı hataları tekrarlamadığından emin ol. Ayrıca Bölüm 5'teki MD dosya mimarisini nihai plana birebir işle — bu benim özellikle istediğim kısım. Bölüm 2'deki PDFusion çelişkisine de nihai planda açıkça karar ver."*

---

## 10. Bu oturumda doğruladığım kaynaklar (31 Ağustos 2026)

- KillerPDF — https://github.com/SteveTheKiller/KillerPDF (repo, releases, README)
- PDF4QT — https://github.com/JakubMelka/PDF4QT ve https://jakubmelka.github.io/ (lisans: LGPLv3)
- Stirling-PDF — https://github.com/Stirling-Tools/Stirling-PDF (LICENSE + engine/LICENSE, open-core model, resmi Tauri masaüstü istemcisi)
- Azure Artifact Signing (Trusted Signing) — https://learn.microsoft.com/en-us/windows/apps/package-and-deploy/code-signing-options ve https://learn.microsoft.com/en-us/azure/artifact-signing/quickstart
- AGENTS.md standardı — Linux Foundation Agentic AI Foundation kapsamında, OpenAI/Google/Cursor/Factory işbirliğiyle
- "Memory Bank" deseni — Cline/Cursor/Copilot ekosistemlerinde yaygın MD-tabanlı ajan hafızası pratiği

*(Not: Ben avukat değilim; lisans değerlendirmeleri genel bilgi amaçlıdır, ürün ticarileştiğinde bir uzmana teyit ettir — Doküman 1'in bu uyarısına katılıyorum.)*
