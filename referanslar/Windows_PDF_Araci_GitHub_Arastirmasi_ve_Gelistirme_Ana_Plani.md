# Windows Masaüstü PDF Uygulaması — Teknik Strateji, GitHub Repo Araştırması ve Geliştirme Ana Planı

> **Araştırma tarihi:** 31 Ağustos 2026  
> **Hedef:** Windows 10/11 üzerinde çalışan, tamamen kullanıcının markasıyla dağıtılabilen, ücretsiz temelli, çevrimdışı çalışabilen, zamanla Adobe Acrobat sınıfına yaklaşabilecek bir PDF masaüstü uygulaması geliştirmek.  
> **Temel prensip:** PDF motorunu, renderer'ı, OCR'ı ve tüm belge işleme algoritmalarını sıfırdan yazmak yerine; lisansı, mimarisi ve kalite seviyesi uygun açık kaynak projeleri kontrollü biçimde temel almak.

---

## 0. Yönetici Özeti — Ben olsam hangi yoldan giderdim?

Bu projede en büyük hata, ilk gün "hangi butonu nereye koyacağız?" diye başlamaktır. İlk karar **lisans ve ürün sahipliği modeli** olmalıdır.

### Karar D0 — Ürünün gelecekteki lisans modeli

Üç olası yol var:

| Yol | Ne demek? | Önerilen temel | Sonuç |
|---|---|---|---|
| **A — Sonsuza kadar ücretsiz + açık kaynak** | Dağıtılan masaüstü uygulaması GPL kaynak koduyla birlikte açık kalabilir | **KillerPDF v1.8.2 fork** | En hızlı ürünleşme yolu |
| **B — Bugün ücretsiz, yarın kapalı/ticari sürüm ihtimali var** | Kaynak kodunu ileride kapatabilmek, OEM/kurumsal lisans satabilmek istenebilir | **PDF4QT MIT + Qt lisans denetimi** veya kendi WPF kabuğun + izin verici lisanslı bileşenler | Daha fazla geliştirme işi, daha fazla stratejik özgürlük |
| **C — Community + ticari/enterprise çift model** | Ücretsiz sürüm devam eder, fakat bazı kurumsal modüller kapalı olabilir | Başlangıçtan itibaren **kendi telif hakkına sahip, permissive/özel çekirdek mimarisi** | En esnek ama en disiplinli başlangıcı gerektirir |

### Benim mevcut şartlara göre birinci önerim

Eğer "uygulama ücretsiz olacak ve kaynak kodunun da açık olması benim için sorun değil" kararı kesin ise:

> **KillerPDF v1.8.2'yi sabit bir başlangıç noktası olarak fork et.**  
> Önce çalışan ürünü koru; marka, kalite, mimari ve test altyapısını güçlendir. Sonra özellik ekle.

Bunun nedeni KillerPDF'nin Ağustos 2026 itibarıyla yalnızca bir viewer olmaktan çıkmış olmasıdır. Güncel repo; WPF/.NET 10 masaüstü uygulamasının yanında ayrı `KillerPdf.Engine` kütüphanesi, PDF 2.0/PDF-A/PDF-UA altyapısı, form, imza, OCR, redaction, sayfa işlemleri, onarım, karşılaştırma, batch ve CLI işlevleri içeriyor.

Fakat bu seçim **GPLv3 kilidi** getirir. KillerPDF README'si bunu açıkça söylüyor: fork edip dağıttığın türev sürümün de GPLv3 kaynak kodu erişilebilir olmalıdır. "Şimdilik GPL ile başlayayım, ileride istersem aynı kodu kapatırım" güvenli bir strateji değildir.

Eğer gelecekte "kaynağı kapatırım", "kurumsal müşteriye proprietary lisans veririm", "OEM yaparım" gibi ihtimallerin %10 bile varsa, KillerPDF ürün kodu üzerinde büyümek yerine PDF4QT veya başka izin verici lisanslı bileşenler üstünde **kendi ürün katmanını** kurmak daha doğru olur.

---

# 1. Araştırmanın en önemli sonucu

İlk bakışta PDF uygulaması şu parçalar gibi görünür:

- PDF aç
- sayfayı göster
- yakınlaştır
- metin ekle
- imza at
- form doldur
- kaydet

Gerçekte ise profesyonel bir PDF uygulaması en az şu bağımsız problemleri çözer:

1. PDF syntax/parser
2. Cross-reference/object stream yönetimi
3. Şifreli PDF açma
4. Font çözümleme
5. CMap/ToUnicode
6. Sayfa rendering
7. Metin çıkarma
8. Metin seçim geometrisi
9. Annotation
10. Content stream düzenleme
11. Form appearance stream
12. Sayfa import/export
13. Incremental save
14. Full rewrite
15. Dijital imza ByteRange koruması
16. PDF/A
17. Tagged PDF / erişilebilirlik
18. OCR
19. Görüntü işleme
20. Redaction
21. Compression
22. Print pipeline
23. Scanner
24. Arama/index
25. Autosave/recovery
26. Zararlı PDF'lere karşı güvenlik
27. Windows installer/update/signing
28. UI/UX
29. Lokalizasyon
30. Regression corpus

Bu nedenle "sıfırdan PDF motoru yazmak" bu projenin ticari avantajı değildir. Ticari/ürün avantajı şuralarda olmalıdır:

- iyi Windows deneyimi,
- hızlılık,
- güvenilir kaydetme,
- kaliteli metin ve sayfa düzenleme,
- temiz araç mantığı,
- çevrimdışı çalışma,
- Türkçe dahil iyi OCR,
- doğru form/imza,
- anlaşılır hata mesajları,
- dosya bozmayan işlemler,
- iyi güncelleme mekanizması,
- kaliteli test süreci.

---

# 2. GitHub Repo Araştırması — Ana adaylar

## 2.1 KillerPDF — şu anda en hızlı "çalışan ürün" tabanı

**Repo:** https://github.com/SteveTheKiller/KillerPDF  
**Lisans:** GPL-3.0  
**Ana dil/arayüz:** C# / WPF  
**Güncel ana hedef:** .NET 10 / `net10.0-windows`  
**İncelenen güncel sürüm:** v1.8.2 — 31 Ağustos 2026  
**Repo yaşı:** 2026 Nisan'da oluşturulmuş; yani oldukça genç.

### Güncel durumda neden ciddi aday?

KillerPDF README ve engine dokümantasyonunda şu yetenekler bulunuyor:

- PDFium tabanlı rendering
- sekmeli belgeler
- split view
- Single / Continuous / Two-Page / Grid görünüm
- annotation
- inline text editing
- text box
- freehand drawing
- lines/highlights/images/stamps
- undo/redo
- OCR
- sayfa merge/split/insert/rotate/crop/extract/delete/reorder
- page image export
- deskew
- perspective correction
- levels / siyah-beyaz işleme
- AcroForm doldurma
- dijital imza
- çizilmiş/görsel imza
- print preview
- flatten
- text search
- text selection
- password protected PDF açma
- repair
- CLI
- batch operasyonlar
- watermark
- sanitize
- redaction
- compare
- localization
- portable ve installer dağıtımı

v1.8 ile ayrıca ayrı bir **KillerPDF.Engine** oluşmuş. Engine UI'dan bağımsız .NET kütüphanesi olarak şu alanları hedefliyor:

- parsing
- deterministic full rewrite
- incremental update
- authoring
- page structural editing
- annotations
- AcroForms
- signatures
- encryption
- tagged PDF / PDF-UA
- PDF/A
- redaction/sanitization
- validation
- security limits

Güncel engine README'si ana branch için 1.436 engine testi ve 2.907 dosyalık corpus kapılarından söz ediyor. KillerPDF ana README'si de binlerce veraPDF örneğiyle zero-regression gate kullandığını söylüyor.

**Önemli:** Bunlar projenin kendi beyanlarıdır. "Bağımsız güvenlik/uyumluluk sertifikası" olarak kabul edilmemelidir. Fork'ta ayrıca doğrulanmalıdır.

### KillerPDF'nin en büyük avantajları

#### 1. Windows için zaten ürün gibi davranıyor

Sıfırdan şu problemleri çözmek zorunda kalmazsın:

- WPF window shell
- tab sistemi
- render viewport
- zoom
- page cache
- drag/drop
- print UI
- toolbar
- theme
- localization
- installer/portable
- form overlay
- OCR UI
- page panel

Bu tek başına büyük bir avantajdır.

#### 2. .NET/C# geliştirme hızına uygun

Windows masaüstünde C++/Qt'ye kıyasla UI geliştirmesi, servis katmanı, test, JSON ayarları, updater, Windows entegrasyonu ve bakım daha kolay olabilir.

#### 3. .NET 10 LTS

Microsoft'un 31 Ağustos 2026 itibarıyla güncel destek tablosunda .NET 10 LTS ve destek sonu **14 Kasım 2028** olarak görünüyor.

Bu nedenle .NET 10 seçimi güncel ve mantıklı. Yine de proje .NET major sürümüne körü körüne bağlı kalmamalı; runtime güncelleme planı olmalı.

#### 4. Ayrı PDF engine oluşmuş

Bu mimari açıdan önemli. UI ve PDF belge modelini ayırabilmek, ileride:

- CLI,
- batch,
- başka bir GUI,
- test harness,
- headless service

gibi kullanımları kolaylaştırır.

### KillerPDF'nin riskleri

#### 1. GPLv3

En büyük stratejik risk budur.

KillerPDF README açıkça şunu belirtiyor: fork/modify/redistribute edilen sürüm GPLv3 ve kaynak kodu erişilebilir olmalıdır.

Bu durumda:

- marka senin olabilir,
- exe adı senin olabilir,
- UI senin olabilir,
- ücretsiz veya ücretli dağıtabilirsin,
- fakat dağıttığın türev GPL kodunu proprietary yapamazsın.

**Sonradan fikir değiştirirsen maliyeti yüksek olur.**

#### 2. Çok genç proje

Repo Nisan 2026'da açılmış ve birkaç ay içinde 1.8.x seviyesine gelmiş.

Bu iki şekilde yorumlanmalı:

**Pozitif:**
- hızlı gelişiyor,
- bakım aktif,
- ciddi test yatırımı yapılıyor.

**Negatif:**
- API ve mimari hâlâ hareketli olabilir,
- hızlı feature eklenmesi regression üretebilir,
- uzun yıllar gerçek dünyada olgunlaşmış bir PDF motoru değil,
- key-person/maintainer riski yüksek.

Aynı gün açılmış güncel issue'larda örneğin highlighter davranışı ve dosya boyutu optimizasyonu gibi konular raporlanmış durumda. Bu kötü bir işaret olmak zorunda değildir; fakat "repo testli olduğu için hatasızdır" denmemelidir.

#### 3. Uygulama katmanında bazı büyük dosyalar var

Repo ağacında büyük WPF/code-behind ve overlay sınıfları görülüyor. Bu, uygulamaya onlarca yeni özellik eklenmeden önce:

- command yapısının,
- document session state'in,
- ViewModel sınırlarının,
- tool state machine'in,
- save pipeline'ın

netleştirilmesi gerektiğini gösteriyor.

Ama hata şu olur:

> Forktan hemen sonra bütün projeyi yeniden yazmaya çalışmak.

Bunun yerine çalışan davranışı koruyarak kademeli refactor yapılmalıdır.

### KillerPDF için karar

**Açık kaynak GPL ürün kabul ediliyorsa:** en güçlü başlangıç adayı.  
**Proprietary gelecek isteniyorsa:** ürünün ana kod tabanı yapılmamalı.

---

# 3. PDF4QT — uzun vadeli lisans esnekliği için ana alternatif

**Repo:** https://github.com/JakubMelka/PDF4QT  
**Lisans:** MIT (27 Nisan 2025'ten beri)  
**Dil:** C++20  
**UI:** Qt 6  
**Platform:** Windows / Linux  
**Repo başlangıcı:** 2019  
**31 Ağustos 2026 anlık GitHub verisi:** yaklaşık 1.460 star / 141 fork  
**İncelenen son stabil release:** 1.6.0.0 — Haziran 2026

### Özellikler

PDF4QT README'sine göre:

- renderer
- viewer
- editor
- multithreading
- encryption
- color management
- optional content/layers
- text layout analysis
- signature validation
- annotation
- form filling
- text-to-speech
- content editing
- attachments
- optimization/compression
- command line
- internal structure inspector
- PDF comparison
- static XFA (sınırlı/read-only)
- digital signing
- public-key security

Windows için MSI, MSIX ve portable release'ler de yayımlanıyor.

### Avantajları

#### 1. Proje çekirdeği MIT

Bu, GPL'ye göre daha fazla gelecekteki ürün özgürlüğü verir.

Fakat şu cümle yanlış olur:

> "PDF4QT MIT, dolayısıyla closed-source üründe lisans sorunu sıfır."

Hayır.

PDF4QT README üçüncü taraf bağımlılıklar listesinde Qt'yi LGPL olarak belirtiyor. Ayrıca FreeType, OpenJPEG, OpenSSL, LittleCMS, zlib, Blend2D vb. bileşenler bulunuyor.

Qt'nin güncel resmi lisans sayfaları şunu özellikle vurguluyor:

- Qt'nin büyük bölümü LGPLv3/GPL seçenekleriyle gelir,
- bazı modüller open-source kullanıcı için yalnız GPL olabilir,
- proprietary uygulama LGPL modüllerle mümkün olabilir ancak LGPL yükümlülükleri eksiksiz uygulanmalıdır,
- dinamik link genellikle daha uygun yaklaşımdır,
- kullanıcı Qt kütüphanesini değiştirebilme/relink edebilme hakkına sahip olmalıdır,
- gerekli lisans metinleri ve Qt kaynak koduna erişim yükümlülükleri vardır,
- static linking ayrıca dikkat ister.

Dolayısıyla PDF4QT seçilecekse **"Qt Module License Matrix"** oluşturulmalıdır.

#### 2. Daha uzun proje geçmişi

2019'dan beri geliştiriliyor ve KillerPDF'ye göre daha uzun gerçek dünya geçmişi var.

#### 3. PDF editörünü C++ seviyesinde kontrol etme imkânı

Performans, native integration ve engine üzerinde doğrudan kontrol avantajdır.

### Riskleri

#### 1. C++/Qt geliştirme maliyeti

WPF/C#'a kıyasla:

- build chain,
- native debugging,
- ABI,
- vcpkg,
- Qt deploy,
- CMake,
- MSVC runtime,
- third-party native library

bakımı daha ağır olabilir.

#### 2. "Editor var" = "Acrobat seviyesinde editor var" değildir

Güncel açık issue'lar bu konuda öğretici.

Örneğin:

- Issue #337: editörde sayfaya image ekledikten sonra sayfadaki metnin kaybolduğu bir senaryo raporlanmış.
- Issue #410: edit modundan çıkıldığında eski metnin ters çevrilmesi raporu var.

Bu PDF4QT'yi kötü proje yapmaz. Tam tersine PDF content editing'in ne kadar zor olduğunu gösterir.

### PDF4QT için karar

**Gelecekte lisans esnekliği çok önemliyse:** güçlü aday.  
**En hızlı Windows ürününü çıkarmak istiyorsan:** KillerPDF daha pratik olabilir.  
**PDF4QT seçilecekse:** Qt lisansını ve content-editor regresyonlarını birinci sınıf risk olarak ele al.

---

# 4. Stirling PDF — kod tabanından çok "özellik araştırma laboratuvarı"

**Repo:** https://github.com/Stirling-Tools/Stirling-PDF

31 Ağustos 2026 itibarıyla çok büyük bir proje ve PDF araç katalogu açısından son derece değerli.

Fakat lisans yapısı artık basit değil.

Repo kökündeki LICENSE:

- genel olarak MIT metni içeriyor,
- ama belirli klasörleri ayrı lisanslara yönlendiriyor.

Özellikle `engine/LICENSE` şu anda **Stirling PDF User License** içeriyor ve production/distribution üzerinde kısıtlar tanımlıyor.

Bu nedenle:

> Stirling'in repo kökü MIT görünüyor diye `engine/` kodunu doğrudan kendi ürününe kopyalamak kesinlikle yapılmamalı.

### Stirling'i nasıl kullanmalısın?

Şunlar için:

- özellik listesi benchmarking,
- tool isimleri ve workflow analizi,
- batch işlem fikirleri,
- OCR flow,
- organize/convert/security araç katalogu,
- kullanıcıların hangi küçük PDF araçlarına ihtiyaç duyduğu

gibi **ürün araştırması**.

Kod alınacaksa:

1. dosyanın gerçek yolu,
2. o klasörün lisansı,
3. git geçmişindeki lisans,
4. bağımlılık lisansı

tek tek incelenmeli.

**Önerim:** Stirling kodunu KillerPDF/PDF4QT ürününün içine rastgele taşıma. İlk aşamada yalnızca davranış/UX benchmark'ı olarak kullan.

---

# 5. Yardımcı repo ve kütüphaneler

## 5.1 qpdf

**Repo:** https://github.com/qpdf/qpdf  
**Lisans:** Apache-2.0  
**Son stabil sürüm (araştırma tarihinde):** 12.3.2

qpdf bir renderer değildir. Asıl gücü:

- PDF yapısal kontrolü,
- object/xref işlemleri,
- encryption,
- linearization,
- repair/transform,
- structural validation

tarafındadır.

### Önerim

qpdf'yi ilk günden "ürünün engine'i" yapmaya çalışma.

Bunun yerine:

> **CI ve Save Validation Gate** olarak kullan.

Her önemli kaydetme senaryosunda:

```text
Our App Save
      ↓
qpdf structural check
      ↓
reopen in our app
      ↓
render smoke test
      ↓
text/form/signature invariants
```

çok değerlidir.

---

## 5.2 Tesseract OCR

**Repo:** https://github.com/tesseract-ocr/tesseract  
**Lisans:** Apache-2.0  
**Güncel release:** 5.5.3 — 24 Temmuz 2026

Tesseract şu anda OCR için en mantıklı ücretsiz omurgalardan biridir.

### Ürün stratejisi

Installer'ın içine 30-100 dil paketini gömmek yerine:

- Türkçe
- İngilizce

varsayılan olabilir.

Diğer `.traineddata` dosyaları **isteğe bağlı indirilebilir**.

KillerPDF 1.8.2 zaten dil paketlerini isteğe bağlı indirme yönüne geçmiş. Bu doğru ürün kararıdır.

### OCR yalnızca "Tesseract çalıştır" değildir

İyi sonuç için pipeline:

```text
PDF page / scan
    ↓
render
    ↓
orientation detection
    ↓
deskew
    ↓
denoise
    ↓
contrast / levels
    ↓
adaptive binarization (gerektiğinde)
    ↓
OCR
    ↓
word bounding boxes
    ↓
invisible searchable text layer
    ↓
PDF save
    ↓
search/select validation
```

olmalıdır.

---

## 5.3 PdfPig

**Repo:** https://github.com/UglyToad/PdfPig  
**Lisans:** Apache-2.0  
**Platform:** .NET

Güçlü olduğu alan:

- text extraction
- letter geometry
- images
- forms/annotations read
- layout analysis için veri sağlama
- metadata/internal structure

KillerPDF'nin güncel README'si text extraction için PdfPig kullandığını belirtiyor.

### Karar

Eğer KillerPDF tabanı kullanırsan mevcut çözümü koru.  
Ekstra bir extraction kütüphanesi sırf "belki daha iyi olur" diye ekleme.

Bir PDF programında aynı işi yapan üç farklı engine olması ileride bug üretir.

---

# 6. Benchmark olarak takip edilmesi gereken uygulamalar

## PDFsam Basic

**Site:** https://pdfsam.org/pdfsam-basic/  
**Amaç:** merge/split/rotate/extract UX benchmark'ı.

PDFsam Basic 2006'dan beri sayfa seviyesi workflow konusunda olgun. Tam editor değildir; ama şunları nasıl sade sunacağına bakmaya değer:

- merge
- split
- page range
- bookmark split
- rotate
- extract
- mix/interleave

Not: güncel resmi site Basic için AGPL lisansını belirtiyor. Kod kopyalama yerine UX benchmark'ı olarak düşün.

---

## PDF Arranger

**Repo:** https://github.com/pdfarranger/pdfarranger  
**Lisans:** GPL-3.0

Çok iyi bir page organizer referansı:

- thumbnail
- drag/drop
- crop
- rotate
- reorder
- merge/split

Özellikle "sayfa panelinin nasıl rahat hissettirilmesi gerektiği" açısından incelemeye değer.

---

## SumatraPDF

**Repo:** https://github.com/sumatrapdfreader/sumatrapdf

Asıl gücü editor değil:

- hızlı startup,
- minimal viewer,
- Windows hissi,
- klavye kullanımı,
- büyük belgelerde sade deneyim.

Senin ürünün feature-heavy olabilir ama **viewer performansı Sumatra zihniyetiyle** ölçülmeli.

---

## Xournal++ / Okular

Bunlar doğrudan ürün tabanı olmak zorunda değil.

Bakılacak alanlar:

- ink/pen UX
- annotation seçimi
- eraser
- stylus
- pressure
- form ve signature kullanıcı deneyimi
- keyboard/accessibility

---

# 7. Bilerek uzak durulması gereken lisans tuzakları

## MuPDF

MuPDF resmi sitesi açıkça:

> open-source embedding için AGPL; proprietary ürün için commercial license

modeli sunuyor.

Bu yüzden:

- açık kaynak GPL/AGPL stratejinde değerlendirilebilir,
- proprietary ihtimal varsa "ücretsiz kütüphane" gibi düşünülmemeli.

## Ghostscript

Benzer şekilde güçlü ama lisans modeli sebebiyle ürün içine "gizli converter" olarak eklenmeden önce lisans incelemesi yapılmalıdır.

## Poppler

Güçlü PDF teknolojisi ancak copyleft lisans etkisi nedeniyle permissive/closed-source hedefli ürünlerde rastgele bundle edilmemeli.

### Kural

Bir GitHub repo bulduğunda sadece:

> "Ücretsiz mi?"

diye sorma.

Şunları sor:

```text
1. Lisansı ne?
2. Bütün repo aynı lisans mı?
3. Alt klasörlerde başka lisans var mı?
4. Native binary'nin lisansı ne?
5. Model/data dosyalarının lisansı ne?
6. Static/dynamic link fark yaratıyor mu?
7. Ticari dağıtım mümkün mü?
8. Kaynak kodu sunma zorunluluğu var mı?
9. Trademark/branding kısıtı var mı?
10. Güncellemede lisans değişebilir mi?
```

---

# 8. Tavsiye edilen ürün mimarisi

İster KillerPDF fork, ister daha bağımsız ürün seç; nihai hedef monolitik `MainWindow` olmamalı.

## Önerilen mantıksal katman

```text
Product.Desktop
│
├── Presentation
│   ├── Shell
│   ├── Tabs
│   ├── Toolbar
│   ├── Sidebar
│   ├── Dialogs
│   └── Commands
│
├── Application
│   ├── DocumentSession
│   ├── CommandBus
│   ├── UndoRedo
│   ├── Jobs
│   ├── FeatureFlags
│   └── Settings
│
├── Pdf
│   ├── DocumentEngine
│   ├── Renderer
│   ├── TextExtraction
│   ├── Editing
│   ├── Pages
│   ├── Forms
│   ├── Signatures
│   ├── Security
│   └── Validation
│
├── Features
│   ├── Annotate
│   ├── Edit
│   ├── Organize
│   ├── OCR
│   ├── Scan
│   ├── Compare
│   ├── Convert
│   ├── Compress
│   ├── Redact
│   ├── Watermark
│   ├── Batch
│   └── Print
│
├── Infrastructure
│   ├── FileSystem
│   ├── Recovery
│   ├── Update
│   ├── Download
│   ├── Logging
│   └── WindowsIntegration
│
└── Native
    ├── PDFium
    ├── OCR
    ├── Scanner
    └── Printing
```

### Temel interface'ler

Uzun vadede şu soyutlamaların bulunması çok faydalı:

```csharp
IPdfDocumentEngine
IPdfRenderer
IPdfTextExtractor
IPdfSavePipeline
IPdfValidationService
IOcrService
IScannerService
ISignatureService
ICertificateStore
IDocumentRecoveryService
IUpdateService
```

Ama KillerPDF fork'larsan **ilk committe bütün sistemi bunlara dönüştürme.**

İlk hedef:

> çalışan davranışı bozmadan bağımlılık sınırlarını kademeli güçlendirmek.

---

# 9. DocumentSession — uygulamanın kalbi

Her açık tab için tek bir `DocumentSession` düşün.

```text
DocumentSession
├── SourceFile
├── WorkingCopy
├── ParsedDocument
├── RenderCache
├── Selection
├── ToolState
├── DirtyState
├── UndoStack
├── RedoStack
├── FormState
├── AnnotationState
├── SearchIndex
└── SaveState
```

Bu neden önemli?

Çünkü kullanıcı:

- iki PDF açabilir,
- birinde pen açık,
- diğerinde form dolduruyor olabilir,
- split view'da iki belgeyi karşılaştırabilir,
- bir belgeyi kaydedip diğerini kaydetmeyebilir.

Global değişkenlerle kurulan UI kısa sürede bozulur.

---

# 10. Tool System — kalem, highlight, shape, text nasıl tasarlanmalı?

Araçlar sadece toolbar button olmamalı.

Örneğin Pen:

```text
PenTool
├── Enabled
├── Color
├── Thickness
├── Opacity
├── LineStyle
├── PressureMode
├── Smoothing
└── EraserMode
```

Toolbar'daki kalem butonunun yanında küçük açılır ok:

```text
✎ Pen  ▼
        ├─ Color
        ├─ Thickness
        ├─ Opacity
        ├─ Solid / Dash
        ├─ Smoothing
        ├─ Pressure
        └─ Eraser
```

Aynı model:

### Line

- stroke color
- thickness
- dashed/solid
- arrow start/end
- opacity

### Shape

- fill
- stroke
- thickness
- radius
- opacity
- rectangle/ellipse/polygon

### Highlight

- color
- opacity
- freehand vs text highlight
- erase highlight

### Text

- font
- size
- bold/italic/underline
- color
- background
- border
- alignment
- line spacing

### Signature

- draw
- type
- image
- certificate
- saved signature

**Önemli UX prensibi:**

Kullanıcının aktif araç özellikleri ekranın başka bir yerinde gizli olmamalı.  
Araç seçildiğinde contextual toolbar görünmeli.

---

# 11. "Edit PDF" özelliğinde en büyük teknik tuzak

PDF üzerinde yeni bir text box çizmek ile mevcut PDF metnini düzenlemek aynı şey değildir.

## PDF metni neden Word gibi değildir?

Bir PDF sayfasında metin şu şekilde tutulabilir:

```text
BT
/F7 10 Tf
1 0 0 1 172 540 Tm
[(H) 8 (e) -2 (l) ...] TJ
ET
```

ve kullanılan font:

- subset olabilir,
- gerçek font adı kayıp olabilir,
- Unicode mapping sorunlu olabilir,
- glyph ID tabanlı olabilir,
- harfler ayrı ayrı konumlandırılmış olabilir.

Bu yüzden:

> "Metne tıkla, paragrafı Word gibi yeniden akıt"

Acrobat sınıfı bir problemi ifade eder.

## Benim önerdiğim aşamalı text editing modeli

### Seviye 1 — Overlay Text

En güvenli.

- Text box ekle
- annotation/burn-in
- font/size/color

### Seviye 2 — Existing Text Object Replacement

Belirli bir text run seçilir.

- eski text object bulunur
- font çözümlenir
- aynı box içerisinde değiştirilir
- overflow olursa kullanıcı uyarılır

### Seviye 3 — Line-aware editing

- satır geometrisi
- font metrics
- kerning
- word spacing
- wrap

### Seviye 4 — Paragraph reflow

En zor seviye.

- paragraf tanıma
- font fallback
- line breaking
- neighboring object collision
- images/floating objects
- RTL
- CJK
- ligatures

İlk sürümde "tam Word benzeri editör" iddiasına girme.

Daha kaliteli ürün davranışı:

> "Bu PDF'deki metin yapısı güvenli yeniden akıtmayı desteklemiyor. Metni aynı alan içinde düzenleyebilir veya yeni bir metin alanı ekleyebilirsin."

demektir.

---

# 12. Font sistemi ayrı bir proje gibi ele alınmalı

İyi PDF editor:

- embedded fontu tanır,
- subset fontu tanır,
- font bulunamazsa fallback seçer,
- Unicode/glyph map korur,
- ToUnicode üretir,
- gerektiğinde yeni subset embed eder.

Test seti:

- Latin
- Turkish
- Cyrillic
- Arabic
- Hebrew
- Greek
- CJK
- emoji
- ligatures
- combining marks

Türkçe özellikle:

```text
İ ı Ş ş Ğ ğ Ç ç Ö ö Ü ü
```

için regression testi olmalı.

---

# 13. Sayfa işlemleri

Bunlar kolay göründüğü için kullanıcı en az hata toleransını burada gösterir.

Desteklenecekler:

- add
- insert
- remove
- extract
- reorder
- duplicate
- rotate
- crop
- resize
- split
- merge
- interleave
- reverse
- odd/even
- page labels
- page boxes

### En önemli kural

Sayfa import ederken sadece `/Page` dictionary kopyalama.

Kaynak bağımlılıklarını güvenli taşı:

- fonts
- images
- resources
- annotations
- form widgets
- destinations
- structure tree
- optional content

Engine bu işi yapıyorsa yeniden yazma; corpus testiyle doğrula.

---

# 14. Form sistemi

İlk hedef **AcroForm** olmalı.

## Alan türleri

- text
- multiline text
- checkbox
- radio
- combo
- list
- push button
- signature
- comb fields

## Sık yapılan hata

Form control ekranda doğru görünür ama kaydedilen PDF başka viewer'da boş görünür.

Neden?

**Appearance stream**.

Her form kaydetme testinde:

1. bizim uygulamada aç
2. kaydet
3. kapat
4. tekrar aç
5. başka bir renderer ile render et
6. field value + appearance uyumunu doğrula

### PDF JavaScript

Form JavaScript'i saldırı yüzeyidir.

Başlangıçta:

- otomatik JS çalıştırma = kapalı
- Launch action = kapalı
- harici executable = kapalı
- URI = kullanıcı onayı

olmalı.

### XFA

Tam dynamic XFA çok pahalı ve legacy bir alan.

İlk ürün için:

- AcroForm tam destek
- XFA detect
- mümkünse read-only
- kullanıcıya açık "bu form türü tam desteklenmiyor" mesajı

daha doğru.

---

# 15. İmza — iki ayrı ürün özelliğidir

## A. Görsel imza

- mouse/stylus ile çiz
- image yükle
- saved signature
- transparent PNG
- page üzerine yerleştir

Bu kriptografik güven sağlamaz.

## B. Dijital imza

- certificate
- private key
- CMS
- ByteRange
- incremental update
- timestamp
- trust chain
- certificate expiry
- revocation

gerektirir.

### UI'da asla karıştırma

Menü:

```text
Sign
├── Add Visual Signature
└── Digitally Sign with Certificate
```

olsun.

### Gelecekte PAdES

Kurumsal hedef varsa:

- RFC3161 timestamp
- OCSP
- CRL
- long-term validation
- PAdES profile

ayrı roadmap olmalı.

---

# 16. Redaction — en tehlikeli özelliklerden biri

Bir metnin üstüne siyah rectangle çizmek **redaction değildir**.

Gerçek redaction:

- alttaki text object'i silmeli,
- ilgili image bölgesini sanitize etmeli/rasterize etmeli,
- annotation/metadata/hidden content kontrol edilmeli,
- save sonrası extract edilen textte veri bulunmamalı.

## Redaction Verification Gate

Redact sonrası otomatik test:

```text
1. save
2. reopen
3. extract text
4. original sensitive string search
5. object scan
6. annotation scan
7. metadata scan
8. attachment scan
9. visual render
```

Fail olursa:

> "Redaction tamamlandı"

denmemeli.

Bu özellik yanlış uygulanırsa kullanıcı gizli bilgisinin açığa çıkmasına neden olabilir.

---

# 17. OCR stratejisi

## Varsayılan diller

Hedef pazar Türkiye ise:

- `tur`
- `eng`

installer ile.

Diğerleri:

```text
Settings
→ OCR Languages
→ Download
```

### Dil paketi güvenliği

Downloaded `.traineddata` için:

- HTTPS
- fixed version manifest
- SHA-256
- temp download
- hash verify
- atomic install

kullan.

### OCR modes

```text
OCR Current Page
OCR Selected Region
OCR Whole Document
Make Searchable PDF
Copy OCR Text
```

### OCR kalite seçeneği

- Fast
- Balanced
- Accurate

gibi preset kullanılabilir.

Altında gerçek parametreler değişir.

---

# 18. Scanner

Windows odaklı uygulamada ilk olarak **WIA** değerlendirilebilir.

Sonra:

- TWAIN
- duplex
- ADF
- feeder
- DPI
- color/gray/bw
- blank page removal
- auto rotate
- deskew
- crop
- OCR after scan

### Scanner workflow

```text
Scan
↓
Preview
↓
Auto Crop
↓
Deskew
↓
Rotate
↓
Reorder
↓
OCR optional
↓
Save PDF
```

"Scan to PDF" kullanıcı için tek feature'dır ama teknik olarak image acquisition + processing + PDF authoring + OCR pipeline'dır.

---

# 19. Convert özellikleri — burada pazarlama hatası yapma

## Image → PDF

Kolay ve yüksek kalite yapılabilir.

## PDF → Image

Kolay; renderer üzerinden.

## Office → PDF

Kalite için:

- Microsoft Office kuruluysa Office automation,
- LibreOffice tabanlı opsiyon,
- başka converter

gibi yollar değerlendirilebilir.

Ama bunların deployment/lisans/uyumluluğu ayrı incelenmeli.

## PDF → Word / Excel / PowerPoint

Bu çok daha zordur.

PDF'den DOCX:

```text
text extraction
+ reading order
+ paragraph inference
+ fonts
+ tables
+ columns
+ images
+ headers/footers
+ layout reconstruction
```

gerektirir.

İlk sürümde bunu "mükemmel Word dönüştürme" diye pazarlama.

Ücretsiz açık kaynak çözümlerin zorlandığı alanlardan biri budur.

Gelecekte bu feature gerçekten kritik olursa:

- kendi layout analysis,
- AI destekli reconstruction,
- opsiyonel commercial SDK

değerlendirilebilir.

---

# 20. PDF/A, PDF/UA ve standartlar

KillerPDF.Engine şu anda PDF/A-4, PDF/A-4e, PDF/A-4f ve PDF/UA-2 alanında yetenekler iddia ediyor.

Bunu direkt pazarlama metnine çevirmeden önce bağımsız test et.

## CI'da

- veraPDF
- qpdf
- kendi golden fixtures

kullan.

### Kural

PDF header:

```text
%PDF-2.0
```

yazdığı için belge PDF 2.0 compliant sayılmaz.

Benzer şekilde metadata'ya PDF/A etiketi koymak compliance değildir.

---

# 21. Kaydetme mimarisi — dosya asla rastgele overwrite edilmemeli

En önemli kalite kurallarından biri:

> Kullanıcının orijinal PDF'si bozulmamalı.

## Save Pipeline

```text
Current Document
      ↓
Create Temp Output
      ↓
Write
      ↓
Flush
      ↓
Validation
      ├─ parser reopen
      ├─ qpdf check
      ├─ page count
      ├─ critical invariants
      └─ signature rules
      ↓
Atomic Replace
      ↓
Backup cleanup
```

### Asla

```text
File.Open(original, Truncate)
→ write
→ crash halfway
```

yapma.

### Save As varsayılanı

Riskli operasyonlarda:

- sanitize
- flatten
- OCR
- redact
- convert
- optimize

ilk kullanımda **Save As** daha güvenlidir.

---

# 22. Autosave ve crash recovery

Profesyonel ürün farkı burada çıkar.

```text
Recovery/
  Session-UUID/
    session.json
    command-journal
    temp-output.pdf
```

Tut.

App crash ederse:

```text
We found an unsaved document recovery.
[Recover] [Discard]
```

göster.

### Recovery içine kullanıcı içeriğini sonsuza kadar bırakma

- belirli süre sonra temizle
- app exit'te temizle
- hassas belgelerde ayar sun
- temp dosya isimlerinde belge adını açıkça kullanmamayı düşün

---

# 23. Undo / Redo

Undo sadece annotation için değil:

- pages
- crop
- rotation
- form change
- text edit
- images
- shapes
- redaction marks
- reorder

için tutarlı olmalı.

## Command pattern

```text
IDocumentCommand
├── Execute()
├── Undo()
├── Redo()
└── Description
```

Örnek:

```text
RotatePagesCommand
MovePagesCommand
SetFormValueCommand
AddAnnotationCommand
EditTextCommand
CropPagesCommand
```

Bu ileride:

- macro
- history
- repeat action
- automation

özelliklerine de temel olur.

---

# 24. PDF güvenlik tehdit modeli

PDF dosyası **trusted input değildir**.

Zararlı/bozuk PDF:

- sonsuz object recursion
- decompression bomb
- huge image dimensions
- malformed xref
- recursive page trees
- crafted fonts
- Javascript
- Launch action
- malicious attachment names
- excessive nesting
- signature parser abuse

içerebilir.

## Güvenlik limitleri

Engine seviyesinde:

- maximum object count
- maximum stream decoded size
- maximum nesting
- maximum image dimensions
- maximum page dimension
- timeout/cancellation
- memory budget

olmalı.

### Harici action'lar

Varsayılan:

```text
PDF JavaScript        OFF
Launch executable     BLOCK
Open local file       ASK
Open web link          ASK/allowed by policy
Submit form externally OFF
```

### Attachment extraction

Şunu engelle:

```text
..\..\AppData\...
C:\Windows\...
```

Attachment filename normalize edilmeli.

---

# 25. Plugin sistemi — erken yapma

"İleride eklenti olsun" iyi fikir.

"İlk sürümde plugin API yapalım" genellikle hata.

Önce internal architecture stabil olsun.

Sonra:

```text
Plugin Manifest
├── id
├── version
├── minimumAppVersion
├── permissions
├── commands
└── signature
```

### Güvenlik

Mümkünse üçüncü taraf plugin:

- ayrı process,
- restricted IPC,
- capability permission

ile çalışsın.

In-process plugin tüm PDF'lere ve kullanıcı hesabına uygulama kadar erişir.

---

# 26. UI/UX ürün stratejisi

Adobe Acrobat'ın her şeyini kopyalamaya çalışma.

Ama kullanıcıların mental modelini kullan.

## Ana çalışma alanları

Ben şu yapıyı öneririm:

```text
Home
View
Edit
Annotate
Pages
Forms
Sign
Protect
Convert
OCR
Tools
```

### Home

- Open PDF
- Recent
- Merge
- Scan
- Create from images
- Compress
- Quick tools

### Sol sidebar

Context'e göre:

- pages
- bookmarks
- attachments
- comments
- layers
- signatures

### Sağ/context panel

Seçili nesnenin properties'i.

### Toolbar

Sık kullanılanlar:

```text
Open Save Print
Undo Redo
Select Hand
Zoom
Text
Highlight
Pen
Shape
Signature
```

---

# 27. "Her özellik var" yerine "her özellik anlaşılır"

Kötü PDF uygulaması:

- 150 button
- birbirine benzeyen ikonlar
- neyin kaydedeceği belli değil
- seçili tool belli değil

İyi uygulama:

- tool group
- selected state
- hover tooltip
- keyboard shortcut
- contextual properties
- destructive action confirmation
- progress/cancel

### Command Search

İleride:

```text
Ctrl+K
> split pages
> redact
> OCR
```

yazınca tool açılması çok iyi kalite hissi verir.

---

# 28. Accessibility

En başta düşün.

- keyboard navigation
- focus indicator
- screen reader names
- high contrast
- Windows scaling
- 125/150/200% DPI
- color alone ile durum anlatmama
- shortcut customization
- minimum target size

PDF/UA desteklemek isteyip kendi uygulamasını erişilemez yapmak tutarsız olur.

---

# 29. Lokalizasyon

UI stringleri kod içinde hardcode etme.

İlk gün:

- English
- Turkish

tam kalitede olsun.

Sonra community translation.

### PDF metadata dili ≠ UI dili

Ayrı tut:

```text
AppLanguage
DocumentLanguage
OCRLanguage
SpellcheckLanguage
```

---

# 30. Windows entegrasyonu

İyi Windows uygulaması:

- `.pdf` Open With
- file association
- shell open
- drag/drop
- recent files
- printer support
- high DPI
- clipboard
- scanner
- certificate store
- crash recovery

ile düzgün bütünleşmeli.

### Default PDF app

Kullanıcı isterse Windows Settings üzerinden default seçmesini yönlendir.  
Registry hack ile sessizce default'u zorlamaya çalışma.

---

# 31. Installer stratejisi

İki dağıtım mantıklı:

## Standard installer

- daha küçük
- .NET Desktop Runtime gerekli olabilir
- per-user varsayılan
- machine-wide opsiyon

## Portable

- runtime dahil
- tek klasör
- registry minimum
- USB/kurumsal kullanım

KillerPDF halihazırda benzer modeli kullanıyor.

### Installer testleri

- clean Windows
- upgrade
- downgrade block
- repair
- uninstall
- file association
- portable coexistence
- path with Turkish characters
- non-admin user

---

# 32. Code signing ve SmartScreen

Bu "sonra bakarız" konusu değil.

Microsoft'un güncel SmartScreen dokümantasyonu:

- imzasız binary her release'te kötü kullanıcı deneyimi yaratabilir,
- OV/EV imza bile yeni binary için anında SmartScreen bypass garantisi vermez,
- EV artık eski yıllardaki gibi anında reputation sağlamaz,
- Microsoft Store dağıtımı SmartScreen tarafında en temiz yollardan biridir,
- Microsoft Artifact Signing da CI/CD için bir seçenek olarak sunuluyor.

## Release pipeline

```text
Build
↓
Tests
↓
SBOM
↓
Malware scan
↓
Sign binaries
↓
Sign installer
↓
Timestamp
↓
Hash
↓
GitHub Release / Store
```

---

# 33. Update sistemi

Auto update yapacaksan güvenli yap.

## Update manifest

```json
{
  "version": "...",
  "url": "...",
  "sha256": "...",
  "signature": "..."
}
```

### Kural

Sadece HTTPS yetmez.

Manifest/binary doğrulaması:

- code signature
- hash
- signed release metadata

ile yapılmalı.

### Rollback

Yeni sürüm startup crash loop yaparsa kullanıcıya eski sürüme dönüş olanağı sağlamak değerlidir.

---

# 34. Offline-first strateji

Bu ürünün güçlü farklılaştırıcısı olabilir.

Varsayılan:

> **PDF içeriği cihazdan çıkmaz.**

Network kullanan alanlar:

- update check
- OCR language download
- optional dictionaries

ile sınırlı olabilir.

### Telemetry

İlk sürümde hiç telemetry kullanmamak gayet iyi bir ürün vaadi olabilir.

İleride crash reporting eklenirse:

- opt-in
- file content gönderme
- PDF adı/path sanitize
- attachment yok
- page text yok

olmalı.

---

# 35. Logging

Log faydalı ama hassas olabilir.

Yaz:

- operation type
- app version
- duration
- error type
- page count
- engine version

Yazma:

- PDF text content
- form values
- password
- signature private key
- certificate key
- full user path (gerekmedikçe)

---

# 36. Performans hedefleri

Feature eklerken ölçülmeyen uygulama ağırlaşır.

KPI belirle:

| KPI | Ölçüm |
|---|---|
| Cold startup | uygulamanın boş açılışı |
| First page render | dosya seçildikten ilk sayfa |
| Scroll smoothness | continuous scroll |
| Memory | 100/500/1000 sayfalık belge |
| Thumbnail generation | uzun PDF |
| Search index | 1000 sayfa |
| Save | küçük/büyük PDF |
| OCR | sayfa başı |
| Open many tabs | 10+ belge |
| Close/reopen | resource leak |

### Cache

- render tile cache
- thumbnail LRU
- text cache
- font cache

hepsinin **upper bound**'u olmalı.

"RAM varsa kullan" yaklaşımı 4 GB makinede çöker.

---

# 37. Test stratejisi

## Katman 1 — Unit

- parser
- writer
- geometry
- page range parser
- font
- form values
- encryption
- command/undo

## Katman 2 — Integration

```text
open → modify → save → reopen
```

## Katman 3 — Corpus

Gerçek ve problemli PDF'ler:

- malformed
- encrypted
- scanned
- huge
- old PDF 1.x
- PDF 2.0
- CJK
- RTL
- forms
- signatures
- transparency
- layers
- weird fonts
- incremental revisions
- PDF/A

## Katman 4 — Golden render

Önceki beklenen screenshot ile yeni render karşılaştır.

Pixel-perfect her zaman doğru olmayabilir; perceptual threshold kullan.

## Katman 5 — UI automation

WPF için uygun desktop automation (ör. FlaUI sınıfı araçlar) değerlendirilebilir.

Senaryolar:

```text
open
zoom
select text
highlight
undo
redo
save
reopen
```

## Katman 6 — Fuzzing

PDF parser saldırı yüzeyidir.

Random input değil sadece; structure-aware mutation yapılmalı.

## Katman 7 — Installer smoke

Clean VM'de kur, aç, PDF aç, uninstall.

---

# 38. Minimum Golden Corpus

Kendi kalıcı corpus'un olsun.

Önerilen kategori:

```text
corpus/
├── basic/
├── scanned/
├── forms/
├── signatures/
├── encryption/
├── fonts/
│   ├── turkish/
│   ├── arabic/
│   ├── cjk/
│   └── subsets/
├── transparency/
├── layers/
├── pdfa/
├── tagged/
├── damaged/
├── large/
└── security/
```

Her bug için:

> bug'ı oluşturan PDF corpus'a eklenmeden issue kapatılmasın.

Bu tek prensip ürün kalitesini yıllar içinde ciddi şekilde artırır.

---

# 39. Definition of Done

Feature yalnızca "ekranda çalışıyor" diye bitmiş sayılmasın.

Örnek: **Rotate Page**

DONE için:

- UI
- keyboard/accessibility
- undo
- redo
- save
- reopen
- encrypted document test
- page boxes test
- annotation geometry
- form widget geometry
- qpdf validation
- crash test
- localization
- unit/integration test

gerekir.

Bu zihniyet olmadan 100 feature olur ama hiçbiri güvenilir olmaz.

---

# 40. Rebrand nasıl yapılmalı?

KillerPDF fork seçilirse ilk hedef "string replace KillerPDF → YeniAd" değildir.

## Rebrand envanteri

- product display name
- assembly name
- executable
- package id
- installer
- publisher
- application icon
- file icons
- window title
- splash
- About
- help URLs
- GitHub URLs
- crash log directory
- AppData
- registry keys
- protocol handlers
- file association
- update channel
- user agent varsa
- CLI command name
- source archive name
- release asset names

### Central ProductInfo

Marka bilgisi tek yerde tutulmalı.

```text
ProductName
CompanyName
AppId
PackageId
ExecutableName
Website
SupportUrl
RepositoryUrl
Copyright
```

### İlk gün namespace'leri komple rename etme

KillerPDF ile upstream sync yapacaksan:

- UI brand'i hemen değiştir,
- internal namespace rename'i kontrollü yap.

Yoksa her upstream merge gereksiz conflict üretir.

---

# 41. Upstream fork yönetimi

KillerPDF'yi fork edip `main` branch'teki her commit'i anında alma.

## Remote

```text
origin   = senin repo
upstream = KillerPDF
```

## Branch mantığı

```text
upstream/main
vendor/killerpdf-1.8.2
main
develop
release/*
feature/*
```

### İlk taban

**Tag/release sabitle.**

Örneğin:

```text
v1.8.2
```

### Upstream update süreci

```text
New upstream release
↓
Read changelog
↓
License diff
↓
Dependency diff
↓
Engine diff
↓
Merge into isolated branch
↓
Corpus
↓
UI tests
↓
Manual smoke
↓
main
```

"Upstream main'de güzel commit gördüm, direkt aldım" yapma.

---

# 42. Kod kalitesi kuralları

## Boyut limitleri

Kesin sayı kutsal değildir; fakat şunlar alarm olmalı:

- 5.000+ satırlık UI class
- 100 farklı sorumluluğu olan service
- static global application state
- UI event içinde PDF byte manipulation
- catch-all exception

### UI event kötü örnek

```csharp
private void SaveButton_Click(...)
{
    // parser
    // PDF writer
    // file IO
    // dialog
    // validation
    // telemetry
}
```

Doğru:

```text
SaveButton
  ↓ command
SaveDocumentCommand
  ↓
SavePipeline
  ↓
Engine
```

---

# 43. Dependency politikası

Yeni NuGet/npm/native package ekleme şartları:

```text
□ Gerçekten gerekiyor mu?
□ Mevcut dependency aynı işi yapıyor mu?
□ Lisansı?
□ Güncel mi?
□ Maintainer aktif mi?
□ CVE geçmişi?
□ Native binary mi?
□ ARM64 var mı?
□ Installer boyutuna etkisi?
□ SBOM'a girdi mi?
```

### Bir özellik = yeni package

mantığından kaçın.

PDF uygulaması zaten native bağımlılıklar yüzünden karmaşık olur.

---

# 44. SBOM ve lisans uyumluluğu

Her release'te:

- direct dependencies
- transitive dependencies
- native DLL
- OCR data
- font/dictionary
- embedded assets

listelenmeli.

Format:

- SPDX veya
- CycloneDX

olabilir.

Repo:

```text
licenses/
THIRD_PARTY_NOTICES.md
sbom/
```

barındırmalı.

### GPL fork ise

Her binary release için **corresponding source** erişilebilir olmalı.

Sadece:

> "GitHub main burada"

demek yerine o binary'nin üretildiği exact source/tag korunmalı.

---

# 45. Supply-chain güvenliği

GitHub Actions'ta:

- action versions pin
- Dependabot
- CodeQL
- dependency review
- release protection
- branch protection
- required CI
- signed release tags
- artifact hashes

kullan.

### Native DLL

Download sırasında "latest DLL" alma.

Version + SHA256 sabitlenmeli.

---

# 46. Dijital imza ve updater secrets

GitHub repo içine:

- PFX
- certificate password
- cloud signing token
- API key

asla koyma.

CI:

- OIDC
- secure signing service
- protected environment

ile yap.

---

# 47. Release kanalları

```text
Nightly/Internal
Beta
Stable
```

mantığı kullan.

### Stable

Sadece:

- corpus pass
- installer pass
- no blocker
- save integrity pass

sonrası.

Feature sayısı release kriteri olmasın.

---

# 48. Versiyonlama

App ve engine version ayrılabilir.

```text
App 1.3.0
Engine 1.8.x-derived
```

kullanmak ileride daha yönetilebilir olabilir.

Fork sürümünde upstream versiyonunu kendi product versiyonun gibi sürdürmek kafa karıştırabilir.

---

# 49. GitHub repo kalite dosyaları

İlk ürünle beraber:

```text
README.md
LICENSE
CONTRIBUTING.md
SECURITY.md
PRIVACY.md
THIRD_PARTY_NOTICES.md
CHANGELOG.md
RELEASE.md
BUILDING.md
ARCHITECTURE.md
SUPPORT.md
CODE_OF_CONDUCT.md
```

oluştur.

### SECURITY.md

Şunu açıkla:

- güvenlik açığı nasıl private raporlanır,
- hangi sürümler desteklenir,
- PDF sample nasıl güvenli gönderilir.

---

# 50. Marka ve isim

Uygulama adını seçerken:

- GitHub organization/repo boş mu?
- domain uygun mu?
- Microsoft Store adı var mı?
- winget package id uygun mu?
- trademark riski?
- başka PDF şirketiyle karışıyor mu?

araştır.

### Kaçın

- Adobe benzeri isim
- Acrobat türevi
- başka projenin ikonuna yakın ikon
- "Official PDF" gibi yanıltıcı isim

Kendi bağımsız markan olmalı.

---

# 51. Ücretsiz ürün nasıl sürdürülebilir?

Ücretsiz demek sürdürülemez demek değildir.

Olası modeller:

### GPL/community yolu

- GitHub Sponsors
- donations
- paid support
- enterprise deployment assistance
- custom integrations
- sponsored feature development
- consulting

### Permissive/proprietary opsiyonu korunursa

- free desktop
- paid enterprise policy pack
- managed updater
- MSI enterprise deployment
- advanced signing integration
- enterprise support
- SDK/API
- optional cloud services

Ama lisans modelini **ilk gün** buna göre seç.

---

# 52. Ürünün farklılaştırıcı pozisyonu

Adobe'yi "özellik sayısıyla" yenmeye çalışma.

Daha iyi positioning:

> **Hızlı, özel verileri internete göndermeyen, abonelik istemeyen, Windows-native, gerçek günlük PDF işlerinin büyük bölümünü ücretsiz yapan editor.**

Özellikle:

- offline
- no account
- no subscription
- Turkish-first quality
- OCR
- scan
- form
- edit
- organize
- sign
- redact
- batch

iyi kombinasyondur.

---

# 53. Özellik önceliği

## P0 — Asla bozulmaması gereken çekirdek

- open
- render
- zoom
- search
- select/copy
- save
- save as
- print
- page operations
- undo/redo
- recovery

## P1 — Günlük kullanıcı değeri

- annotate
- text box
- image
- highlight
- pen
- form fill
- visual sign
- merge/split
- OCR
- compress

## P2 — Profesyonel

- true text edit
- create forms
- cryptographic signing
- redaction
- compare
- scan
- batch
- metadata
- watermark/header/footer
- Bates numbering

## P3 — Uzman/enterprise

- advanced preflight
- PDF/A conversion
- PDF/UA tooling
- certificate policies
- PAdES/LTV
- color separations
- print production
- plugin SDK
- automation API
- enterprise policy

Bu sırayı bozma.

---

# 54. Büyük hata listesi

## Hata 1 — İlk günden 100 özellik

Sonuç:

- çok buton
- yarım feature
- save bug
- UI karmaşası

**Doğru:** her milestone kalite kapısından geçsin.

---

## Hata 2 — "Open source = istediğimi yaparım"

Yanlış.

GPL, AGPL, LGPL, MIT, Apache, custom source-available lisans birbirinden farklıdır.

---

## Hata 3 — Mevcut engine varken yenisini yazmak

PDF syntax öğrenmek değerli ama ürün avantajın parser yazmak değil.

---

## Hata 4 — PDF'yi kaydederken orijinali doğrudan truncate etmek

Dosya kaybının en kötü sebeplerinden biri.

---

## Hata 5 — Black rectangle = redact

Gizlilik felaketi olabilir.

---

## Hata 6 — Görsel imza = dijital imza

Kullanıcıyı yanıltır.

---

## Hata 7 — PDF→Word için "100% aynı" sözü

Gerçekçi değil.

---

## Hata 8 — Sadece kendi 10 PDF'inle test

PDF ekosistemi çok heterojendir.

---

## Hata 9 — UI'ı tamamen Adobe kopyalamak

Hem ürün kimliğini bozar hem hukuki/trademark/trade-dress riskleri doğurabilir.

---

## Hata 10 — Upstream main'i sürekli merge etmek

Fork stabilitesi kaybolur.

---

## Hata 11 — Tüm namespace/repo yapısını ilk gün rename etmek

Upstream merge cehennemi.

---

## Hata 12 — Plugin sistemi çok erken

Stabil olmayan architecture'ı public API haline getirirsin.

---

## Hata 13 — Telemetry ile PDF içeriği toplamak

Gizlilik pozisyonunu yok eder.

---

## Hata 14 — Unsigned installer yayımlamak

Windows güven deneyimi zayıflar.

---

## Hata 15 — Update binary'sini yalnız URL'den indirip çalıştırmak

Supply-chain saldırısına kapı açar.

---

# 55. Tavsiye edilen aşamalı ana plan

Aşağıdaki plan "takvim" değil **milestone** bazlıdır. Bir aşama kabul kriterleri geçmeden sonrakine geçilmemelidir.

---

## AŞAMA 0 — Ürün ve lisans kararı

### Amaç

Kod yazmadan stratejik kilidi çözmek.

### Kararlar

- GPL kabul mü?
- Source her zaman açık mı?
- Commercial/OEM ihtimali var mı?
- Windows only mi?
- Cloud kesinlikle yok mu?
- Telemetry var mı?
- App adı/publisher kim?

### STOP GATE

Şu cümlelerden biri imzalanmış ürün kararı olmalı:

**A**
> "Bu ürün GPLv3 açık kaynak kalacaktır. KillerPDF fork'u kullanabiliriz."

veya

**B**
> "Proprietary/ticari esnekliği koruyacağız. KillerPDF GPL kodunu ürün tabanı yapmayacağız."

Bu karar yoksa geliştirme başlama.

---

## AŞAMA 1 — Teknik baseline

### KillerPDF seçildiyse

- v1.8.2 tag sabitle
- exact commit kaydet
- build
- installer
- portable
- tests
- sample corpus
- Windows VM smoke

### PDF4QT seçildiyse

- 1.6.0.0 stabil release baseline
- Windows MSI/portable test
- build from source
- Qt module/license inventory
- editor bug regression test

### Çıktılar

```text
BASELINE.md
DEPENDENCIES.md
LICENSE_MATRIX.md
KNOWN_ISSUES.md
```

### STOP GATE

- clean machine build
- app opens
- 100+ representative PDFs crash-free
- save/reopen
- installer uninstall
- dependency/license list complete

---

## AŞAMA 2 — Rebrand ve ürünleştirme

### Yap

- product name
- icon
- package id
- exe
- AppData
- About
- installer
- file association
- localization
- settings
- update URL
- source/repo attribution

### Yapma

- engine rewrite
- devasa namespace rename
- 20 yeni feature

### STOP GATE

Kullanıcının görebildiği hiçbir yerde upstream ürün adı yanlışlıkla kalmamalı; lisans/attribution kısmında ise yasal atıflar korunmalı.

---

## AŞAMA 3 — Quality/Security Foundation

### Yap

- atomic save
- recovery
- qpdf gate
- golden corpus
- render regression
- UI smoke
- fuzz plan
- logging privacy
- updater signing
- SBOM
- CodeQL/Dependabot

### STOP GATE

"PDF açmak/kaydetmek kullanıcı verisini bozmaz" güveni feature sayısından önce sağlanmış olmalı.

---

## AŞAMA 4 — Viewer + annotation mükemmelleştirme

### Yap

- viewport
- smooth scrolling
- zoom
- page modes
- thumbnails
- search
- selection
- pen/highlight/shapes/text
- contextual toolbar
- keyboard
- undo/redo
- stylus

### STOP GATE

Günlük viewer deneyimi stabil değilse "advanced editor" başlamasın.

---

## AŞAMA 5 — Pages + forms

### Pages

- merge
- split
- reorder
- crop
- rotate
- extract
- insert
- duplicate

### Forms

- fill
- appearance
- create fields
- edit fields
- tab order
- export/import data (opsiyonel)

### STOP GATE

Adobe Reader, browser PDF viewer ve ürünün kendi reopen testlerinde kaydedilen field değerleri/appearance tutarlı görünmeli.

---

## AŞAMA 6 — Gerçek text/image editing

### Kapsamı kontrollü başlat

- existing text select
- same-box text replace
- font match
- font fallback
- image select/replace/move
- content object properties

### Sonra

- line reflow
- paragraph reflow
- complex scripts

### STOP GATE

Edit edilen sayfa başka içerikleri kaybetmemeli. PDF4QT issue #337/#410 tipi regressions için doğrudan test oluştur.

---

## AŞAMA 7 — OCR + Scan

- tur/eng default
- on-demand packs
- preprocessing
- page/region OCR
- searchable PDF
- WIA
- ADF/duplex
- deskew/crop
- scan profile

### STOP GATE

OCR text layer görünmez ama search/copy/selection geometrisi makul olmalı; scan pipeline memory leak yapmamalı.

---

## AŞAMA 8 — Protect + Sign + Redact

- password
- permissions
- sanitize
- visual signature
- certificate sign
- signature validation
- redaction
- metadata removal

### STOP GATE

Redaction verified; digital signature save incremental ve valid; password handling log'a düşmüyor.

---

## AŞAMA 9 — Professional/Automation

- batch
- compare
- CLI
- watch folder
- automation recipes
- PDF/A
- PDF/UA
- advanced preflight
- Bates
- header/footer
- enterprise deployment
- plugin API en son

---

# 56. Benim tavsiye ettiğim ilk repo düzeni

KillerPDF fork ise upstream yapıyı anında dağıtma.

Önce ekle:

```text
/docs
  PRODUCT.md
  ARCHITECTURE.md
  LICENSE_MATRIX.md
  SECURITY_MODEL.md
  QUALITY_GATES.md
  ROADMAP.md
  UPSTREAM.md

/tests
  /Corpus
  /Golden
  /UI
  /Security

/tools
  /validation
  /release
```

Sonra refactor oldukça `src/` yapısına geçilebilir.

---

# 57. İlk 30 teknik iş

1. Lisans modeli kararını kilitle.
2. Ürün adını geçici kod adıyla belirle.
3. KillerPDF v1.8.2 ve PDF4QT 1.6.0.0 binary'lerini aynı test dosyalarıyla kıyasla.
4. Seçilen upstream tag/commit'i sabitle.
5. `upstream` remote politikasını yaz.
6. Dependency inventory çıkar.
7. License matrix çıkar.
8. SBOM üretimini kur.
9. Clean Windows build doğrula.
10. Installer smoke test yap.
11. Portable smoke test yap.
12. 100+ PDF başlangıç corpus'u oluştur.
13. qpdf validation ekle.
14. veraPDF testlerini uygun standart dosyalarında kullan.
15. Save temp + atomic replace kur.
16. Crash recovery tasarla.
17. ProductInfo/branding'i merkezileştir.
18. UI hardcoded string avı yap.
19. Türkçe ve İngilizce resource sistemini sağlamlaştır.
20. DocumentSession state sınırını tanımla.
21. Tool state'i global UI state'ten ayır.
22. Undo/redo command modelini standardize et.
23. PDF JavaScript/Launch policy'sini güvenli varsayılana çek.
24. Attachment extraction path traversal testleri ekle.
25. OCR language package hash verification ekle.
26. Release signing planı oluştur.
27. Update manifest/signature formatını tanımla.
28. CodeQL/Dependabot/branch protection aç.
29. `SECURITY.md` oluştur.
30. İlk stabil branded baseline tag'ini oluştur.

---

# 58. Repo seçim puanlaması

Bu puanlar **objektif benchmark değil**, bu ürün hedefi için karar desteğidir.

5 = çok iyi.

| Kriter | KillerPDF | PDF4QT | Stirling PDF | PDFsam Basic |
|---|---:|---:|---:|---:|
| Windows masaüstü başlangıç hızı | **5** | 3 | 2 | 3 |
| Viewer hazır oluşu | **5** | **5** | 3 | 2 |
| Editor yönü | **4** | **4** | 3 | 1 |
| Forms | **4** | **4** | 3 | 1 |
| OCR | **5** | 2-3 | **5** | 1 |
| Signature | **4** | **4** | 4 | 1 |
| Page tools | **5** | **5** | **5** | **5** |
| Windows-native geliştirme rahatlığı | **5 (C#/WPF)** | 3 (C++/Qt) | 2 | 2-3 |
| Proprietary gelecek esnekliği | **1** | **4*** | Değişken/klasöre bağlı | 1 |
| Upstream olgunluk geçmişi | 2-3 | **4** | 4 | **5** |
| Mevcut ürün kapsamı | **5** | 4 | **5** | 2 |
| Fork/rebrand kolaylığı | **4** | 3 | 1-2 | 2 |

\* PDF4QT core MIT olsa da Qt ve tüm üçüncü taraf bileşenlerinin lisans şartları ayrıca sağlanmalıdır.

---

# 59. Kesin tavsiyem

## Sen "ücretsiz ve açık kaynak kalacak" diyorsan

**KillerPDF v1.8.2 ile başla.**

Ama şu şartlarla:

1. `main` yerine tagged release tabanı.
2. GPL yükümlülüklerini bilinçli kabul et.
3. Upstream source'u rastgele değiştirme.
4. Önce branding + quality foundation.
5. Kendi corpus'unu kur.
6. Save integrity'yi kırmızı çizgi yap.
7. PDF engine'e doğrudan müdahaleleri test olmadan yapma.
8. Büyük WPF sınıflarını kademeli refactor et.
9. Feature sayısı değil güvenilirlik metriği tut.
10. Ürünün farklılaştırıcısını offline/privacy + kolaylık + kalite yap.

## Sen "belki ileride proprietary olur" diyorsan

**KillerPDF'yi ürün kodu olarak kullanma.**

PDF4QT'yi ciddi aday yap fakat:

1. exact Qt modules çıkar,
2. LGPL/GPL matrix hazırla,
3. dynamic linking/deployment modelini doğrula,
4. content editor açık bug'ları corpus'a koy,
5. C++/Qt geliştirme maliyetini kabul et.

Alternatif olarak kendi Windows WPF kabuğunu; PDFium + izin verici lisanslı yardımcı bileşenler + kendi/başka uygun document engine ile inşa etmek daha uzun ama marka/telif kontrolünü artırabilir.

---

# 60. "Adobe Acrobat gibi" hedefini doğru tanımlama

Acrobat tek ürün değil, aslında şu ürünlerin birleşimidir:

```text
Viewer
+ Annotation App
+ PDF Content Editor
+ Page Organizer
+ Form Designer
+ OCR Scanner
+ Converter
+ Digital Signature Client
+ Security Tool
+ Print Production Tool
+ Standards Preflight Tool
+ Automation Platform
```

Bunların hepsini aynı anda yapmaya çalışma.

### Başarı ölçütü

İlk büyük hedef:

> "Bir kullanıcının günlük Acrobat işlerinin %80'ini güvenilir biçimde, ücretsiz ve offline yapabilmek."

Sonra uzman özellikleri.

---

# 61. Ürün kalite ilkeleri — README'nin en üstüne yazılacak kadar önemli

Ben bu proje için şu ilkeleri sabitlerdim:

1. **Never corrupt the original file.**
2. **Offline by default.**
3. **No hidden uploads.**
4. **No fake redaction.**
5. **Visual signature and digital signature are distinct.**
6. **Unsupported PDF behavior is reported, not silently destroyed.**
7. **Every fixed PDF bug becomes a regression test.**
8. **Every release is reproducible enough to trace back to exact source.**
9. **All bundled code/data has known licensing.**
10. **Features do not bypass quality gates.**
11. **Accessibility is a feature, not polish.**
12. **Performance is continuously measured.**
13. **The UI reveals the active tool and its properties.**
14. **Dangerous operations have previews and safe defaults.**
15. **No marketing promise that exceeds verified capability.**

---

# 62. Kaynaklar — 31 Ağustos 2026'da kontrol edilen başlıca kaynaklar

## KillerPDF

- Repository:  
  https://github.com/SteveTheKiller/KillerPDF
- README:  
  https://github.com/SteveTheKiller/KillerPDF/blob/main/README.md
- Engine README:  
  https://github.com/SteveTheKiller/KillerPDF/blob/main/engine/README.md
- Application changelog:  
  https://github.com/SteveTheKiller/KillerPDF/blob/main/CHANGELOG.md
- Engine changelog:  
  https://github.com/SteveTheKiller/KillerPDF/blob/main/engine/CHANGELOG.md
- Releases:  
  https://github.com/SteveTheKiller/KillerPDF/releases
- Issues:  
  https://github.com/SteveTheKiller/KillerPDF/issues

## PDF4QT

- Repository:  
  https://github.com/JakubMelka/PDF4QT
- README / license history / third-party dependencies:  
  https://github.com/JakubMelka/PDF4QT/blob/master/README.md
- Releases:  
  https://github.com/JakubMelka/PDF4QT/releases
- Content-editor issue #337:  
  https://github.com/JakubMelka/PDF4QT/issues/337
- Content-editor issue #410:  
  https://github.com/JakubMelka/PDF4QT/issues/410

## Stirling PDF

- Repository:  
  https://github.com/Stirling-Tools/Stirling-PDF
- Root LICENSE:  
  https://github.com/Stirling-Tools/Stirling-PDF/blob/main/LICENSE
- Engine-specific LICENSE:  
  https://github.com/Stirling-Tools/Stirling-PDF/blob/main/engine/LICENSE

## Supporting projects

- qpdf:  
  https://github.com/qpdf/qpdf
- qpdf releases:  
  https://github.com/qpdf/qpdf/releases
- Tesseract OCR:  
  https://github.com/tesseract-ocr/tesseract
- Tesseract 5.5.3:  
  https://github.com/tesseract-ocr/tesseract/releases/tag/5.5.3
- PdfPig:  
  https://github.com/UglyToad/PdfPig
- PDF Arranger:  
  https://github.com/pdfarranger/pdfarranger
- SumatraPDF:  
  https://github.com/sumatrapdfreader/sumatrapdf
- PDFsam Basic:  
  https://pdfsam.org/pdfsam-basic/
- MuPDF licensing/release note:  
  https://mupdf.com/releases

## Platform/licensing references

- Qt open-source licensing obligations:  
  https://www.qt.io/development/open-source-lgpl-obligations
- Qt 6 licensing documentation:  
  https://doc.qt.io/qt-6/licensing.html
- Microsoft .NET support policy:  
  https://dotnet.microsoft.com/en-us/platform/support/policy
- Microsoft SmartScreen reputation for app developers:  
  https://learn.microsoft.com/en-us/windows/apps/package-and-deploy/smartscreen-reputation
- Microsoft MSIX signing:  
  https://learn.microsoft.com/en-us/windows/msix/package/signing-package-overview

---

# 63. Araştırma notu ve hukuki uyarı

Bu belge teknik/ürün stratejisi ve açık kaynak lisans risklerinin mühendislik açısından değerlendirmesidir; hukuki danışmanlık değildir.

Özellikle:

- GPL türev eser,
- LGPL linking/relinking,
- Qt modül lisansları,
- trademark,
- üçüncü taraf binary/data dağıtımı,
- ticari/OEM dağıtım

kararı verilmeden önce gerçek dağıtım modeline göre lisans kontrolü yapılmalıdır.

Lisans dosyaları zaman içinde değişebileceği için **her upstream güncellemesinde lisans diff'i CI/release checklist'inin parçası olmalıdır.**

---

# 64. Sonuç

Bu projeyi başarıya götürecek yol "en çok PDF özelliğini en kısa sürede eklemek" değildir.

Doğru sıra:

```text
Lisans kararı
    ↓
Doğru çalışan upstream
    ↓
Sabit baseline
    ↓
Rebrand
    ↓
Save/Recovery/Security
    ↓
Regression corpus
    ↓
Viewer UX
    ↓
Daily tools
    ↓
True editor
    ↓
Forms/OCR/Scan
    ↓
Sign/Redact
    ↓
Professional features
    ↓
Automation/ecosystem
```

### Bugünkü en önemli karar

**GPL açık kaynak ürün kabul ediyorsan KillerPDF v1.8.2 en mantıklı hızlı taban.**

Fakat KillerPDF'nin hızlı gelişmesi seni yanıltmasın. Onu "hazır olduğu için hiç dokunmadan marka değiştirip yayınlanacak ürün" değil, **çok güçlü bir başlangıç platformu** olarak düşün.

Senin gerçek ürün değerini şu katman oluşturmalı:

- daha temiz UX,
- daha iyi tool property sistemi,
- daha güvenli save,
- daha iyi recovery,
- daha sıkı regression test,
- daha iyi Turkish/English deneyimi,
- daha anlaşılır edit davranışı,
- güvenilir form/imza/redaction,
- kaliteli Windows dağıtımı,
- güçlü privacy/offline ilkesi.

Bunlar doğru yapılırsa "başka bir açık kaynak fork" olmaktan çıkar ve kendi kimliği, kalite standardı ve kullanıcı kitlesi olan gerçek bir PDF ürünü haline gelir.
