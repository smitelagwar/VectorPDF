# Masaüstü PDF Aracı — Nihai Karar ve Geliştirme Rehberi

> **Hazırlayan:** Antigravity (Claude Opus 4.6 Thinking) — 31 Ağustos 2026  
> **Amaç:** GPT 5.6 Sol Ultra moduna verilecek bağımsız referans dokümanı. Bu dosya, üç farklı AI'dan (Claude, GPT, Gemini) alınan önerilerin sentezi + bağımsız doğrulama + ek araştırma bulgularını içerir.  
> **Bu dosyanın rolü:** Nihai planı üreten AI'a giriş bağlamı ve karar desteği sağlamak — son kararlar kullanıcıya ve o AI'a aittir.

---

## BÖLÜM 0 — ÖNCEKİ REFERANSLARIN ÖZETİ VE ELEŞTİRİSİ

Kullanıcı üç farklı AI'dan şu dosyaları aldı:

### Dosya 1: "PDF Uygulaması Yol Haritası" (Claude)
- **Önerdiği yol:** Tauri v2 + React + pdf.js + pdf-lib + tesseract.js
- **Güçlü yanları:** Lisans değerlendirmesi çok iyi, overlay düzenleme gerçekçiliği doğru, SmartScreen/code signing bilgisi güncel.
- **Zayıf yanları:** `pdf-lib`'in bakım durumunu araştırmamış (son release 2021). Tauri ile masaüstü PDF editörü yapma deneyimi sektörde çok sınırlı — kanıtlanmış örnek az.

### Dosya 2: "Windows PDF Aracı GitHub Araştırması ve Ana Plan" (GPT)
- **Önerdiği yol:** KillerPDF v1.8.2 fork (GPLv3 kabul edilirse) VEYA PDF4QT (MIT esneklik istenirse)
- **Güçlü yanları:** En kapsamlı ve gerçekçi doküman. KillerPDF'nin var olduğunu doğruladım (GitHub'da gerçek, aktif proje). 64 bölümlük derinliği mükemmel. DocumentSession, save pipeline, font sistemi, redaction tehlikeleri, upstream fork yönetimi gibi konularda sektör bilgisi yüksek.
- **Zayıf yanları:** 3264 satır — pratikte bir AI'ın tamamını bağlam penceresinde tutması zor. Tauri seçeneğini hiç değerlendirmemiş.

### Dosya 3: "Profesyonel Masaüstü PDF Aracı Geliştirme ve Mimari Yol Haritası" (Gemini/başka AI)
- **Önerdiği yol:** Tauri v2 + React + pdf.js + PDFium/QPDF sidecar
- **Güçlü yanları:** Mimari diyagramı net, klasör yapısı somut, faz açıklamaları kısa ve öz.
- **Zayıf yanları:** PDF4QT lisansını yanlış vermiş (LGPLv3 demiş, aslında Nisan 2025'ten beri MIT). "KillerPDF" gibi güçlü bir adayı hiç görmemiş/değerlendirmemiş. `[x]` işaretleriyle tüm fazları "yapıldı" gibi göstermiş ki bu yanıltıcı.

> [!IMPORTANT]
> **Halüsinasyon Uyarısı:** "KillerPDF" projesinin varlığını bağımsız olarak doğruladım — gerçek bir GitHub reposu, aktif geliştirme, GPLv3 lisanslı, WPF/.NET tabanlı. Ancak Nisan 2026'da oluşturulmuş genç bir proje. Üç dosyadan sadece biri (Dosya 2) bunu biliyordu.

---

## BÖLÜM 1 — DOĞRULANMIŞ GERÇEKLER (31 Ağustos 2026)

Aşağıdaki her madde bağımsız web araştırmasıyla doğrulanmıştır:

### 1.1 Başlıca Açık Kaynak Adaylar

| Proje | Dil/Framework | Lisans | Son Sürüm | Doğrulanmış Durum |
|---|---|---|---|---|
| **KillerPDF** | C# / WPF / .NET 10 | **GPLv3** | v1.8.2 (Ağustos 2026) | ✅ Gerçek, aktif, kapsamlı özellik seti |
| **PDF4QT** | C++20 / Qt 6 | **MIT** (Nisan 2025'ten beri) | v1.6.0.0 (Haziran 2026) | ✅ Gerçek, ~1460 star, content editor var ama bug'lı |
| **Stirling-PDF** | Java / Spring Boot | **Karışık** (kök MIT, engine/ özel lisans) | Aktif | ✅ Gerçek, 50+ araç, web tabanlı, masaüstü desteği var ama doğrudan fork riskli |
| **SumatraPDF** | C/C++ / Win32 | GPLv3 | Aktif | ✅ Gerçek, yalnızca viewer, düzenleme yok |
| **PDF Arranger** | Python / GTK | GPLv3 | Aktif | ✅ Gerçek, yalnızca sayfa yönetimi |

### 1.2 Kütüphaneler ve Motorlar

| Kütüphane | Lisans | Son Güncelleme | Durum |
|---|---|---|---|
| **pdf.js** (Mozilla) | Apache 2.0 | Aktif (2026) | ✅ Endüstri standardı viewer |
| **pdf-lib** (Hopding) | MIT | ⚠️ **Son release: Kasım 2021** | ⚠️ Maintenance-mode, güncelleme yok, güvenlik yamaları yok |
| **tesseract.js** | Apache 2.0 | Aktif | ✅ WASM OCR motoru |
| **Tesseract OCR** (native) | Apache 2.0 | v5.5.3 (Temmuz 2026) | ✅ Aktif |
| **PDFium** (Google) | Apache 2.0 | Aktif (Chromium ile) | ✅ En hızlı rendering motoru |
| **qpdf** | Apache 2.0 | v12.3.2 | ✅ Yapısal doğrulama için mükemmel |
| **PdfPig** | Apache 2.0 | Aktif | ✅ .NET text extraction |
| **PdfSharp** | MIT | Aktif | ✅ .NET PDF oluşturma |

> [!WARNING]
> **`pdf-lib` Kritik Uyarı:** Bu kütüphane 2021'den beri güncellenmemiş. Yeni bir projede ana bağımlılık olarak kullanmak risklidir. Güvenlik yamaları yok, modern build tool uyumsuzlukları olabilir. Tauri + pdf-lib yaklaşımını öneren referanslar (Dosya 1 ve 3) bu riski değerlendirmemiş.

### 1.3 Framework Karşılaştırması (Masaüstü Kabuğu)

| Kriter | WPF (.NET) | Tauri v2 | Electron |
|---|---|---|---|
| Windows native hissi | ⭐⭐⭐⭐⭐ | ⭐⭐⭐ | ⭐⭐ |
| Kurulum boyutu | ~30-80 MB (.NET runtime) | ~3-10 MB (WebView2 OS'ta) | ~120-200 MB |
| RAM kullanımı | ~80-150 MB | ~40-80 MB | ~150-400 MB |
| PDF ekosistemi kalitesi | ⭐⭐⭐⭐⭐ (PDFium, PdfPig, PdfSharp) | ⭐⭐⭐ (pdf.js, pdf-lib ⚠️) | ⭐⭐⭐⭐ (pdf.js + Node.js) |
| AI ile geliştirme kolaylığı | ⭐⭐⭐⭐ (C# iyi bilinen dil) | ⭐⭐⭐⭐ (React/TS iyi bilinen) | ⭐⭐⭐⭐ |
| Hazır çalışan ürün tabanı | ✅ KillerPDF, PDF4QT | ❌ Hiç yok | Sınırlı |
| Cross-platform potansiyeli | ❌ (Avalonia ile mümkün ama ayrı iş) | ✅ (Win/Mac/Linux/Mobil) | ✅ |

---

## BÖLÜM 2 — BENİM BAĞIMSIZ ANALİZİM VE TAVSİYELERİM

### 2.1 İki Ana Strateji Yolu

Bu projenin önünde iki temel yol var. Her ikisinin de artıları ve eksileri farklı:

#### YOL A — "Hazır Motor, Kendi Marka" (KillerPDF Fork)

```
Mevcut çalışan ürün (KillerPDF v1.8.2)
    ↓
Fork + Rebrand
    ↓
Kalite katmanı ekle (save, recovery, test)
    ↓
UX iyileştir
    ↓
Kendi özelliklerini ekle
    ↓
Kendi markan altında dağıt
```

**Avantajları:**
- **Anında çalışan ürün.** Viewer, editor, form, OCR, imza, batch — hepsi var.
- **WPF/.NET ekosistemi.** C# ile AI'ların en rahat çalıştığı dillerden biri.
- **Ayrı PDF engine.** `KillerPDF.Engine` kütüphanesi UI'dan bağımsız.
- **Windows-native.** Sağ tık, dosya ilişkilendirme, print — hepsi hazır.
- **PDFium renderer.** Endüstri standardı rendering zaten entegre.
- **Test altyapısı.** 1400+ engine testi, 2900+ dosyalık corpus.

**Riskleri:**
- **GPLv3 kilidi.** Fork dağıtırsan kodun da açık kalmalı. İleride kapatılamaz.
- **Genç proje.** Nisan 2026'da açılmış. API değişebilir, olgunlaşmamış olabilir.
- **Upstream bağımlılığı.** Tek maintainer riski, upstream değişikliklerle sync maliyeti.

#### YOL B — "Sıfırdan Modern Kabuk" (Tauri + React + Mevcut Kütüphaneler)

```
Boş Tauri + React projesi
    ↓
pdf.js entegre et (viewer)
    ↓
Rust backend ile PDFium/qpdf bağla (sayfa işlemleri)
    ↓
Annotation katmanı kur
    ↓
Form desteği ekle
    ↓
OCR ekle
    ↓
Kendi markan altında dağıt
```

**Avantajları:**
- **Tam UI kontrolü.** Modern, güzel arayüz (React + Tailwind/Fluent).
- **Lisans esnekliği.** MIT/Apache kütüphanelerle istediğin lisansı seçersin.
- **Cross-platform potansiyeli.** Tauri v2 ile mobil de mümkün.
- **Küçük kurulum.** ~3-10 MB.

**Riskleri:**
- **Sıfırdan inşa.** Viewer dışında her şey sıfırdan yazılmalı.
- **pdf-lib ölü.** Ana manipulation kütüphanesi 2021'den beri güncellenmemiş.
- **PDF editing çok zor.** Content stream düzenleme, font subsetting, appearance stream — bunların hiçbiri hazır gelmiyor.
- **Kanıtlanmış yol var ama sınırlı:** Open PDF Studio (Tauri 2 + React + PDFium) var ama ~720 star ile nispeten küçük bir proje. İncelenmeye değer.
- **Tahmini süre:** MVP (sadece viewer + temel sayfa işlemleri) bile 2-4 ay, Acrobat seviyesine yaklaşmak yıllar.

### 2.2 Benim Net Tavsiyem

> [!IMPORTANT]
> **Eğer hızla çalışan bir ürün istiyorsan ve GPL sorun değilse: YOL A (KillerPDF fork) açık ara en mantıklı seçim.**

Gerekçem:

1. **"Ücretsiz PDF aracı" hedefi zaten GPL ile uyumlu.** Ücretsiz dağıtacaksan GPL'nin ana kısıtı (kaynak kodu açık tutma) zaten amacınla çelişmiyor.

2. **KillerPDF'nin Ağustos 2026 özellik seti, Tauri ile sıfırdan 1 yılda yazabileceğinin çok ötesinde.** Inline text editing, OCR, dijital imza, form filling, batch operations, compare, redaction — bunlar yılların birikimidir.

3. **"Acrobat gibi" hedefine en yakın açık kaynak ürün budur.** PDF4QT de güçlü ama C++/Qt geliştirme maliyeti çok daha yüksek.

4. **pdf-lib'in ölü olması Tauri yolunu ciddi şekilde zayıflatıyor.** pdf.js bir viewer; oluşturma/düzenleme için güvenilir bir JS kütüphanesi yok. Rust'ta PDFium wrapper var ama bunlarla Acrobat seviyesine ulaşmak çok uzun sürer.

5. **AI ile C# geliştirme çok verimli.** WPF/XAML kodu modern AI'lar tarafından iyi anlaşılıyor.

> [!NOTE]
> **İleride lisans değiştirmek istersen:** KillerPDF kodunu ürün tabanı yapmadan önce bunu kesin kararla kabul etmelisin. "Şimdi GPL ile başlayıp sonra kapatırım" mümkün değildir. Eğer %10 bile proprietary ihtimal varsa, PDF4QT (MIT) veya kendi WPF kabuğun + permissive kütüphaneler yoluna git.

### 2.3 Alternatif Hibrit Yol (Eğer GPL Kabul Edilmezse)

```
Kendi WPF (.NET) masaüstü kabuğu
    ↓
PDFium (Apache 2.0) — rendering
    ↓
PdfPig (Apache 2.0) — text extraction
    ↓
PdfSharp (MIT) — PDF oluşturma/manipülasyon
    ↓
qpdf (Apache 2.0) — yapısal doğrulama
    ↓
Tesseract (Apache 2.0) — OCR
```

Bu yol daha yavaş ama tüm bileşenler permissive lisanslı — ileride ne yapmak istersen yapabilirsin.

---

## BÖLÜM 3 — AI OTURUM YÖNETİMİ İÇİN MD DOSYA MİMARİSİ

Projenin ilerideki AI oturumlarında tutarlı kalması için şu MD dosya yapısını öneriyorum:

### 3.1 Dosya Hiyerarşisi

```
proje-koku/
│
├── AGENTS.md                    # AI çalışma kuralları (her oturumda ilk okunan)
├── PROJECT.md                   # Projenin "kimlik kartı" — ne, neden, nasıl
├── DECISIONS.md                 # Alınan kararlar ve gerekçeleri (değişmez tarihçe)
│
├── .agents/
│   ├── rules/
│   │   ├── coding-standards.md  # Kod yazım kuralları
│   │   ├── pdf-engine.md        # PDF motoruyla ilgili kurallar
│   │   ├── save-pipeline.md     # Kaydetme güvenliği kuralları
│   │   ├── licensing.md         # Lisans kuralları ve kırmızı çizgiler
│   │   └── ui-ux.md             # Arayüz tasarım kuralları
│   │
│   ├── context/
│   │   ├── architecture.md      # Mimari bağlam
│   │   ├── feature-map.md       # Özellik haritası ve durum
│   │   └── tech-stack.md        # Teknoloji yığını detayları
│   │
│   └── memory/
│       ├── changelog.md         # Yapılan değişikliklerin kronolojik kaydı
│       ├── lessons-learned.md   # Öğrenilen dersler (hata → çözüm)
│       └── blocked-patterns.md  # "Bunu yapma" listesi (kanıtlanmış kötü kararlar)
│
├── docs/
│   ├── ROADMAP.md               # Faz bazlı yol haritası
│   ├── QUALITY_GATES.md         # Her fazın "bitti" tanımı
│   ├── LICENSE_MATRIX.md        # Tüm bağımlılıkların lisans tablosu
│   ├── SECURITY_MODEL.md        # Güvenlik tehdit modeli
│   ├── UPSTREAM.md              # Fork sync politikası (fork seçilirse)
│   └── BUILDING.md              # Derleme talimatları
│
├── tests/
│   └── corpus/                  # Altın test PDF'leri
│       ├── basic/
│       ├── forms/
│       ├── fonts/turkish/
│       ├── scanned/
│       ├── encrypted/
│       ├── damaged/
│       └── large/
│
└── CHANGELOG.md
```

### 3.2 Her MD Dosyasının Amacı ve Biçimi

#### `AGENTS.md` — AI İşletim Kılavuzu
```markdown
Uzunluk: Maks ~150 satır
İçerik:
- Projenin tek cümlelik tanımı
- Derleme/test komutları
- Dokunulmaması gereken dosyalar/dizinler
- Commit kuralları
- Kalite kapıları
- Lisans kırmızı çizgileri
```

**Neden kısa:** AI bağlam penceresi kısıtlıdır. AGENTS.md her oturumda tam okunur — şişerse verimsizleşir.

#### `PROJECT.md` — Proje Kimlik Kartı
```markdown
İçerik:
- Proje adı ve vizyonu
- Hedef platform ve kullanıcı
- Lisans modeli kararı
- Teknoloji yığını özeti
- Ana mimari diyagram
- Mevcut durum (hangi faz, neyi bekliyor)
- İletişim ve karar yetkisi
```

**Neden gerekli:** AI'lar her oturumda "bu proje ne?" sorusunu sormadan çalışabilsin.

#### `DECISIONS.md` — Değişmez Karar Günlüğü
```markdown
Format:
## [Tarih] — [Karar Başlığı]
**Durum:** Kabul / Reddedildi / Beklemede
**Bağlam:** Neden bu karar verildi?
**Karar:** Ne kararlaştırıldı?
**Sonuçlar:** Bu kararın etkileri neler?
**Alternatifler:** Değerlendirilip reddedilen seçenekler
```

**Neden gerekli:** AI'ların "bunu neden böyle yapmıştık?" sorusunu yeniden açmasını önler. Halüsinasyonla eski kararları çelişkili şekilde değiştirmesini engeller.

#### `.agents/memory/lessons-learned.md` — Öğrenilen Dersler
```markdown
Format:
## [Tarih] — [Kısa Başlık]
**Ne oldu:** [Yaşanan sorun]
**Neden oldu:** [Kök neden]
**Çözüm:** [Nasıl düzeltildi]
**Kural:** [Bundan sonra uygulanacak kural]
```

**Neden gerekli:** Aynı hatanın tekrarlanmasını önler. Her AI oturumu önceki oturumların hatalarını bilir.

#### `.agents/memory/blocked-patterns.md` — Yasak Kalıplar
```markdown
Format:
## YAPMA: [Kısa açıklama]
- Neden yapılmamalı
- Alternatif ne yapılmalı
- Referans (hangi oturumda/durumda keşfedildi)
```

**Neden gerekli:** AI'ların "bu daha iyi olur" diye aynı kötü kararı tekrar vermesini önler.

### 3.3 Halüsinasyon Koruması İçin Özel Kurallar

Bu kurallar `AGENTS.md` veya `.agents/rules/` altına konulmalıdır:

```markdown
# Halüsinasyon Önleme Kuralları

1. DOĞRULAMA ZORUNLULUĞU
   - Herhangi bir GitHub reposundan bahsederken URL ver ve var olduğunu doğrula.
   - Bir kütüphanenin "X özelliği var" denmeden önce dokümanından teyit et.
   - Lisans bilgisi verirken repo'daki LICENSE dosyasını kontrol et.

2. BELİRSİZLİK BİLDİRİMİ
   - Emin olmadığın bilgiyi "⚠️ Doğrulanmadı:" önekiyle işaretle.
   - "Muhtemelen", "sanırım", "olabilir" gibi ifadelerle kesinlik ima etme.

3. KAYNAK GÖSTERİMİ
   - Her teknik iddia için kaynak (URL, dosya yolu, doküman bölümü) göster.
   - "Genel bilgi" olarak sunulan iddiaları bile mümkünse kaynak ile destekle.

4. GERİYE DÖNÜK UYUMLULUK
   - DECISIONS.md'deki kararları okumadan çelişen öneri verme.
   - lessons-learned.md'deki kalıpları okumadan aynı hataya düşme.
   - blocked-patterns.md'deki yasakları okumadan yasak işlem önerme.

5. DEĞIŞIKLIK LOGLAMA
   - Her oturumda yapılan değişiklikler changelog.md'ye eklenmeli.
   - "Neyi, neden, nasıl değiştirdim" formatında.
```

---

## BÖLÜM 4 — DOĞRULANMIŞ GITHUB KAYNAKLARI

### 4.1 Ürün Tabanı Adayları

| Repo | URL | Lisans | Amaç |
|---|---|---|---|
| KillerPDF | https://github.com/SteveTheKiller/KillerPDF | GPLv3 | Tam ürün fork tabanı |
| PDF4QT | https://github.com/JakubMelka/PDF4QT | MIT | Permissive lisanslı alternatif |
| Stirling-PDF | https://github.com/Stirling-Tools/Stirling-PDF | Karışık (dikkat!) | Özellik listesi referansı |

### 4.2 Kütüphaneler

| Repo | URL | Lisans | Kullanım |
|---|---|---|---|
| pdf.js | https://github.com/mozilla/pdf.js | Apache 2.0 | Web viewer |
| PDFium | https://pdfium.googlesource.com/pdfium | Apache 2.0 | Native rendering |
| qpdf | https://github.com/qpdf/qpdf | Apache 2.0 | Yapısal doğrulama |
| Tesseract | https://github.com/tesseract-ocr/tesseract | Apache 2.0 | OCR |
| tesseract.js | https://github.com/naptha/tesseract.js | Apache 2.0 | WASM OCR |
| PdfPig | https://github.com/UglyToad/PdfPig | Apache 2.0 | .NET text extraction |
| Tauri | https://github.com/tauri-apps/tauri | MIT/Apache 2.0 | Masaüstü çatı (Yol B) |

### 4.3 UX Referansları (Kod Alınmaz, Davranış İncelenir)

| Proje | URL | Bakılacak Alan |
|---|---|---|
| PDF Arranger | https://github.com/pdfarranger/pdfarranger | Sayfa sürükle-bırak UX |
| SumatraPDF | https://github.com/sumatrapdfreader/sumatrapdf | Hız, minimalizm, Windows hissi |
| Okular | KDE projesi | Annotation UX |
| Xournal++ | https://github.com/xournalpp/xournalpp | Çizim/pen UX |
| PDFsam Basic | https://pdfsam.org | Merge/split workflow |

### 4.4 Yeni Keşif: Open PDF Studio (Tauri 2 ile Gerçek PDF Editörü!)

> [!TIP]
> Araştırma sırasında keşfedilen önemli proje — **Tauri 2 ile masaüstü PDF editörü yapan çalışan bir örnek var!**

| Proje | URL | Lisans | Detay |
|---|---|---|---|
| **Open PDF Studio** | https://github.com/OpenAEC-Foundation/open-pdf-studio | LGPL-3.0 | Tauri 2 + React + Rust + PDFium. MS Office tarzı ribbon UI, yerel OCR, redaction, dijital imza, ölçüm araçları. ~720 star. |

Bu proje, "Tauri ile PDF editörü yapılamaz" iddiasını çürütüyor ve Yol B (Tauri) seçilirse **doğrudan referans olarak incelenebilir**.

### 4.5 pdf-lib Alternatifi: Aktif Fork'lar

pdf-lib'in kendisi ölü olsa da aktif fork'ları var:

| Fork | URL | Durum |
|---|---|---|
| **@pdfme/pdf-lib** | pdfme projesi altında | Aktif — bug fix ve dependency güncellemeleri |
| **cantoo-scribe/pdf-lib** | cantoo-scribe fork'u | Aktif — şifreleme desteği eklenmiş |
| **LibPDF** | Documenso projesi | Yeni TypeScript PDF kütüphanesi, pdf-lib'den ilham almış |

### 4.6 Diğer Kayda Değer Projeler

| Proje | Dil | Stars | Lisans | Özellik |
|---|---|---|---|---|
| **Sioyek** | C++/Qt/MuPDF | ~15.9k | GPL-3.0 | Akademik PDF okuyucu |
| **Xournal++** | C++/GTK3 | ~15.0k | GPL-2.0 | El yazısı/kalem annotation |
| **Every PDF** | Electron+Next.js | Aktif | MIT | %100 offline masaüstü |
| **pdf-chisel** | Electron+PDFium | Aktif | MIT | In-place metin düzenleme |
| **pdfme** | TypeScript/React | ~4.8k | MIT | WYSIWYG şablon tasarımcısı |
| **pdfcpu** | Go | ~8.7k | Apache 2.0 | Hızlı CLI + Go kütüphanesi |
| **PDFsharp** | C#/.NET | ~3.2k | MIT | .NET PDF oluşturma |

### 4.7 Lisans Tehlikeli Bölge (AGPL / Copyleft)

| Kütüphane | Lisans | Risk |
|---|---|---|
| MuPDF | AGPL-3.0 / Ticari | Kullanırsan uygulamanı açık kaynak yapmak zorundasın |
| Ghostscript | AGPL / Ticari | Aynı risk |
| Poppler | GPL-2.0+ | Copyleft etkisi |
| PyMuPDF | AGPL | MuPDF'in Python wrapper'ı, aynı risk |

> [!CAUTION]
> Bir kütüphanenin "ücretsiz" olması "istediğin gibi kullanabilirsin" demek değildir. AGPL, uygulamanı dağıttığın anda tüm kaynak kodunu da açık yapmanı gerektirebilir — ve bu senin kendi yazdığın kodu da kapsar.

---

## BÖLÜM 5 — AŞAMALI YOL HARİTASI (MILESTONE BAZLI)

Bu yol haritası KillerPDF fork senaryosu için yazılmıştır. PDF4QT veya Tauri seçilirse adaptasyon gerekir.

### Aşama 0 — Stratejik Kararlar (Kod Yazılmaz)
**Amaç:** Tek bir satır kod yazmadan önce tüm stratejik kilitleri çözmek.

**Kararlar:**
- [ ] Uygulama adı seçildi mi? (domain, GitHub org, TÜRKPATENT, Microsoft Store kontrolü)
- [ ] GPL kabul mü? Proprietary ihtimal var mı?
- [ ] Windows-only mı? Cross-platform ileride olabilir mi?
- [ ] Telemetri olacak mı?
- [ ] Gelir modeli nedir? (Tamamen ücretsiz? Freemium? Sponsorship?)

**Çıktılar:** `DECISIONS.md` ilk kayıtları, `PROJECT.md` taslağı

**STOP GATE:** Bu kararlar alınmadan geliştirme başlamaz.

---

### Aşama 1 — Teknik Baseline
**Amaç:** Seçilen ürün tabanını çalışır halde doğrulamak.

**İşler:**
- [ ] KillerPDF v1.8.2 tag/commit'ini sabitle
- [ ] Clean Windows'ta build et
- [ ] Installer ve portable sürümü test et
- [ ] 100+ PDF ile crash-free açma testi
- [ ] Mevcut testleri çalıştır
- [ ] Dependency inventory çıkar
- [ ] License matrix oluştur

**Çıktılar:** `BASELINE.md`, `DEPENDENCIES.md`, `LICENSE_MATRIX.md`, `KNOWN_ISSUES.md`

**STOP GATE:** Clean build + 100 PDF crash-free + tüm bağımlılık lisansları belgelenmiş.

---

### Aşama 2 — Rebrand ve Ürünleştirme
**Amaç:** Kullanıcının gördüğü her yerde kendi markan.

**İşler:**
- [ ] Product name, icon, package ID, exe adı değiştir
- [ ] About ekranı, splash, help URL'leri güncelle
- [ ] AppData/registry yollarını güncelle
- [ ] File association (.pdf) yapılandır
- [ ] Türkçe + İngilizce lokalizasyon doğrula
- [ ] Open source attribution sayfası kur

**YAPMA:** Engine rewrite, namespace rename (upstream sync bozulur), 20 yeni feature ekleme.

**STOP GATE:** Kullanıcının görebildiği hiçbir yerde upstream adı yok; lisans/attribution kısmında yasal atıflar korunuyor.

---

### Aşama 3 — Kalite ve Güvenlik Temeli
**Amaç:** "PDF açmak/kaydetmek veri bozmaz" güvencesini sağlamak.

**İşler:**
- [ ] Atomic save pipeline kur (temp → validate → atomic replace)
- [ ] Crash recovery mekanizması
- [ ] qpdf validation gate
- [ ] Golden corpus oluştur (100+ PDF kategorize)
- [ ] Render regression test altyapısı
- [ ] PDF JavaScript/Launch action güvenlik politikası (varsayılan kapalı)
- [ ] SBOM oluştur
- [ ] CodeQL/Dependabot aç

**STOP GATE:** Hiçbir save işlemi orijinal dosyayı bozamaz. Recovery çalışıyor.

---

### Aşama 4 — Viewer + Annotation Mükemmelleştirme
- Viewport, zoom, scroll, page modes
- Thumbnail panel (sürükle-bırak)
- Text search, selection
- Pen, highlight, shapes, text box
- Undo/redo (command pattern)
- Keyboard shortcuts

**STOP GATE:** Günlük viewer deneyimi stabil.

---

### Aşama 5 — Pages + Forms
- Merge, split, reorder, crop, rotate, extract, insert
- AcroForm fill, appearance stream
- Form field create/edit
- Flatten

**STOP GATE:** Kaydedilen form Adobe Reader'da doğru görünüyor.

---

### Aşama 6 — Text/Image Editing (Dikkatli)
- Existing text object seç ve değiştir
- Font match / font fallback
- Image select/replace/move
- Overlay text (güvenli yol)

> **"Tam Word benzeri editör" iddiasına girme.** Gerçekçi sınırları kullanıcıya göster.

**STOP GATE:** Edit edilen sayfa diğer içerikleri kaybetmiyor.

---

### Aşama 7 — OCR + Scan
- Tesseract OCR (tur + eng varsayılan)
- İsteğe bağlı dil paketi indirme
- Preprocessing pipeline (deskew, denoise, binarize)
- Searchable PDF oluşturma
- WIA scanner entegrasyonu

---

### Aşama 8 — Protect + Sign + Redact
- AES-256 şifreleme
- Görsel imza (çizim/resim)
- Dijital imza (sertifika ile)
- Gerçek redaction (content stream'den silme)
- Metadata removal/sanitize

---

### Aşama 9 — Professional/Automation
- Batch operations
- Document compare
- CLI
- PDF/A, PDF/UA
- Watermark, header/footer
- Plugin API (en son!)

---

## BÖLÜM 6 — UI/UX TAVSİYELERİ

### 6.1 Ana Çalışma Alanları
```
Home | View | Edit | Annotate | Pages | Forms | Sign | Protect | Convert | OCR | Tools
```

### 6.2 Acrobat'tan Öğrenilecek Kalıplar
- **Contextual toolbar:** Seçili araca göre üst çubuğun değişmesi
- **Sol sidebar:** Sayfalar, yer imleri, eklentiler, yorumlar, katmanlar, imzalar
- **Command search:** Ctrl+K ile araç arama (VSCode benzeri)
- **Tool state açıklığı:** Aktif araç her zaman görünür

### 6.3 Acrobat'tan Öğrenilmeyecek Şeyler
- Trade dress kopyalamak (hukuki risk)
- 150 butonluk karmaşa
- Abonelik zorunluluğu
- Bulut bağımlılığı

### 6.4 Farklılaştırıcı Pozisyon
> "Hızlı, dosyaları internete göndermeyen, abonelik istemeyen, Windows-native, günlük PDF işlerinin %80'ini ücretsiz yapan editör."

---

## BÖLÜM 7 — KRİTİK TEKNİK TUZAKLAR

### 7.1 PDF Düzenleme Gerçeği
PDF bir kelime işlemci belgesi değildir. Her harf (x,y) koordinatında çizim komutudur. "Kelimeyi sil, sağdakiler kaysın" PDF'te doğal olarak mümkün değildir. Bu gerçeği kullanıcıya açıkça söyle.

### 7.2 Font Subsetting Tuzağı
PDF'ler fontların sadece kullanılan harflerini gömer. Kullanıcı yeni harf yazmak isterse o harf font'ta olmayabilir. Font fallback mekanizması şart.

### 7.3 Büyük Dosya Performansı
500+ sayfalık PDF'lerde virtual scrolling kullan. Sadece görünen 3-5 sayfayı render et. RAM bütçesi koy.

### 7.4 Save Güvenliği
Asla orijinal dosyayı doğrudan truncate etme. Temp → validate → atomic replace pipeline'ı kullan.

### 7.5 Redaction Tehlikesi
Siyah dikdörtgen çizmek redaction değildir. Alttaki text object silinmezse "Ctrl+A → Copy" ile gizli metin hâlâ alınabilir. Redaction sonrası doğrulama testi zorunlu.

### 7.6 Görsel İmza ≠ Dijital İmza
Bu iki kavramı UI'da asla karıştırma. Biri görsel süs, diğeri kriptografik güvence.

### 7.7 Türkçe Karakter Testi
```
İ ı Ş ş Ğ ğ Ç ç Ö ö Ü ü
```
Her font işleminde, OCR'da, text extraction'da bu karakterlerin regression testi olmalı.

---

## BÖLÜM 8 — WINDOWS DAĞITIM STRATEJİSİ

### 8.1 Code Signing (2026 Durumu)
- **EV sertifikası artık SmartScreen'i anında geçirmiyor** (Mart 2024 değişikliği)
- **En pratik yol:** Microsoft Store (MSIX) — Store kendi imzalar, SmartScreen uyarısı yok
- **Alternatif:** Azure Artifact Signing (~\$9.99/ay)
- **Gerçekçi beklenti:** İlk sürümlerde bazı kullanıcılar "Windows korudu" uyarısı görecek. Bu normal.

### 8.2 Dağıtım Modelleri
1. **Microsoft Store (MSIX)** — en temiz yol, SmartScreen sorunu yok
2. **Standard installer (MSI/NSIS)** — .NET runtime gerekebilir
3. **Portable** — tek klasör, USB/kurumsal kullanım
4. **GitHub Releases** — imzalı .exe/.msi

### 8.3 Otomatik Güncelleme
- Manifest: version + URL + SHA-256 + signature
- Yalnız HTTPS yetmez — binary doğrulaması zorunlu
- Rollback mekanizması düşün

---

## BÖLÜM 9 — GPT 5.6 SOL ULTRA'YA ÖZEL TAVSİYELERİM

Bu bölüm, nihai planı oluşturacak AI'a doğrudan notlarımdır.

### 9.1 Dikkat Et
1. **KillerPDF'nin varlığını doğruladım** — gerçek bir proje, halüsinasyon değil. Ama Nisan 2026'da açılmış genç bir repo.
2. **pdf-lib (Hopding) ölü** — son release Kasım 2021. Bunu Tauri planlarında ana bağımlılık yapma.
3. **PDF4QT lisansı MIT'e dönmüş** (Nisan 2025) — eski referanslar LGPLv3 diyebilir, güncel bilgi MIT.
4. **Stirling-PDF'in engine/ klasörü MIT değil** — özel lisans. Kopyala-yapıştır yapma.
5. **Tauri ile tam kapsamlı PDF editörü yapan bilinen bir açık kaynak proje yok** — bu yol kanıtlanmamış.

### 9.2 Planında Olması Gerekenler
1. **Lisans kararı — en üstte, en başta.** Bu karar alınmadan hiçbir şey başlamasın.
2. **MD dosya mimarisi.** BÖLÜM 3'teki yapıyı öner — halüsinasyon koruması, geriye dönük log, öğrenilen dersler.
3. **Milestone bazlı yol haritası.** Takvim değil, her aşamanın "bitti" tanımı ile.
4. **Lisans matrisi.** Her bağımlılığın lisansı tablo halinde.
5. **Kalite kapıları.** Her aşamada "bu geçmeden ilerleyemezsin" kuralları.
6. **Test corpus planı.** Kategorize edilmiş altın PDF seti.
7. **Rebrand envanteri.** Fork seçilirse nerelerin değişmesi gerektiğinin tam listesi.

### 9.3 Planında Olmaması Gerekenler
1. **"Her şeyi ilk sürümde yap" vaadi.** Acrobat'ın 25 yıllık özellik setini 6 ayda yazmak gerçekçi değil.
2. **Doğrulanmamış repo isimleri veya URL'ler.** Her GitHub bağlantısını doğrula.
3. **Takvim tahmini.** AI destekli geliştirmede süre tahmini çok yanıltıcı. Milestone bazlı düşün.
4. **Plugin API'si ilk fazlarda.** Stabil olmayan mimariyi public API haline getirmek erken.

---

## BÖLÜM 10 — KAYNAK VE REFERANS LİSTESİ

### Doğrulanmış Kaynaklar
- KillerPDF: https://github.com/SteveTheKiller/KillerPDF
- PDF4QT: https://github.com/JakubMelka/PDF4QT
- Stirling-PDF: https://github.com/Stirling-Tools/Stirling-PDF
- pdf.js: https://github.com/mozilla/pdf.js
- pdf-lib: https://github.com/Hopding/pdf-lib (⚠️ Son release: Kasım 2021)
- tesseract.js: https://github.com/naptha/tesseract.js
- Tesseract OCR: https://github.com/tesseract-ocr/tesseract (v5.5.3)
- qpdf: https://github.com/qpdf/qpdf (v12.3.2)
- PdfPig: https://github.com/UglyToad/PdfPig
- PDFium: https://pdfium.googlesource.com/pdfium
- Tauri v2: https://tauri.app
- SumatraPDF: https://github.com/sumatrapdfreader/sumatrapdf
- PDF Arranger: https://github.com/pdfarranger/pdfarranger

### Platform Referansları
- Qt lisanslama: https://www.qt.io/development/open-source-lgpl-obligations
- Microsoft SmartScreen: https://learn.microsoft.com/en-us/windows/apps/package-and-deploy/smartscreen-reputation
- Microsoft MSIX signing: https://learn.microsoft.com/en-us/windows/msix/package/signing-package-overview
- .NET destek politikası: https://dotnet.microsoft.com/en-us/platform/support/policy

---

## BÖLÜM 11 — SONUÇ

Bu doküman, üç farklı AI'ın önerilerini sentezleyip bağımsız araştırmayla doğruladıktan sonra hazırlanmıştır.

**Özet Tablo — Üç AI'ın Önerisi vs Benim Değerlendirmem:**

| Konu | Claude (Dosya 1) | GPT (Dosya 2) | Gemini (Dosya 3) | Benim Değerlendirmem |
|---|---|---|---|---|
| **Ana strateji** | Tauri + pdf.js + pdf-lib | KillerPDF fork VEYA PDF4QT | Tauri + pdf.js + PDFium sidecar | **KillerPDF fork** (GPL kabul edilirse) |
| **pdf-lib durumu** | Önermiş | Bahsetmemiş | Önermiş | ⚠️ **Ölü — 2021'den beri güncelleme yok** |
| **KillerPDF** | Bilmiyor | Detaylı analiz | Bilmiyor | ✅ Gerçek ve en kapsamlı aday |
| **Lisans analizi** | İyi | Çok iyi | Hatalı (PDF4QT LGPLv3 demiş) | PDF4QT MIT (Nisan 2025'ten beri) |
| **Gerçekçilik** | Orta | Yüksek | Düşük (fazları [x] işaretlemiş) | Yüksek |

---

> **Bu doküman nihai plan değildir.** Nihai planı GPT 5.6 Sol Ultra oluşturacaktır. Bu doküman, o planın girdisi ve kalite kontrolüdür.
