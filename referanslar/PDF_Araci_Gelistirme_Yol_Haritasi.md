# Profesyonel Masaüstü PDF Aracı Geliştirme ve Mimari Yol Haritası

Bu doküman, Windows ortamında çalışacak, tamamen özelleştirilebilir, modern arayüze sahip, ücretsiz ve ilerleyen süreçte **Adobe Acrobat Pro** seviyesinde yetenekler kazanabilecek bağımsız bir PDF masaüstü yazılımı inşa etmek için hazırlanmış kapsamlı teknik rehberdir.

---

## 1. Stratejik Karar Matrisi: Sıfırdan vs. Fork vs. Modüler Hibrit

PDF (Portable Document Format) spesifikasyonu (ISO 32000), binlerce sayfalık karmaşık bir çizim ve nesne ağacı standardıdır. Sıfırdan bir PDF render veya parse motoru yazmak on binlerce saatlik mühendislik gerektirir. Bu nedenle izlenebilecek üç ana strateji bulunmaktadır:

| Yaklaşım | Geliştirme Hızı | Özelleştirme & UI Kontrolü | Bakım / Genişletilebilirlik | Tavsiye Durumu |
| :--- | :--- | :--- | :--- | :--- |
| **A) Tam Hazır Bir Masaüstü Projesini Forklamak** *(Örn: PDF4QT, SumatraPDF)* | Çok Hızlı (1-2 Hafta) | Düşük - Orta (C++/Qt veya Win32 GUI koduna bağımlı kalınır, modern web arayüzü zordur) | Zor (Mevcut monolitik C++ mimarisini bozmadan yeni özellik eklemek ileri düzey C++ ister) | Hızlı prototip için iyi, modern UI için kısıtlayıcı. |
| **B) Tamamen Sıfırdan İnşa Etmek** | Çok Yavaş (Yıllar) | %100 Serbest | Çok Zor (Font gömme, xref tabloları, stream sıkıştırmaları gibi yüzlerce standardı tek tek yazmak gerekir) | **Kesinlikle Önerilmez.** |
| **C) Modüler Hibrit Mimari (Tavsiye Edilen)** *(Tauri + PDF.js + Rust/Python Sidecar)* | Hızlı - Dengeli (2-4 Hafta) | **%100 Serbest** (React, Tailwind, modern Fluent/Glassmorphism tasarım) | **Çok Kolay** (Görüntüleme, sayfa işleme, OCR ve form motorları birbirinden bağımsız modüllerdir) | **EN İYİ STRATEJİ** |

---

## 2. Derinlemesine İncelenmesi Gereken Açık Kaynak GitHub Projeleri

Projenize doğrudan dahil edebileceğiniz veya mimarisini modelleyebileceğiniz en güçlü açık kaynaklı projeler:

### A. Masaüstü Uygulama Repoları (Doğrudan Fork Adayları)
1. **PDF4QT (`JurajKoval/PDF4QT`) - C++ / Qt / Poppler & PDFium**
   * **Neden Önemli?** Açık kaynak dünyasındaki en gelişmiş "Adobe Acrobat benzeri" masaüstü editörüdür.
   * **Hazır Yetenekler:** Form doldurma, dijital imzalama, sayfa kırpma/döndürme/silme, şifreleme, anotasyonlar (vurgulama, not, çizim) ve eklenti desteği.
   * **Lisans:** LGPLv3 (Özelleştirip dağıtmak için uygundur).
   * **Dezavantaj:** Arayüz klasik Qt Widgets stilindedir; modern minimalist bir UI'a dönüştürmek ciddi C++ Qt QML bilgisi gerektirir.

2. **Stirling-PDF (`Stirling-Tools/Stirling-PDF`) - Java / Spring / Web UI**
   * **Neden Önemli?** PDF ile ilgili akla gelebilecek her aracı (OCR, bölme, birleştirme, watermark, form, karşılaştırma, dark mode) tek çatı altında toplayan devasa bir araç setidir.
   * **Nasıl Kullanılır?** Doğrudan bir masaüstü uygulaması olmasa da, backend'i gömülü bir binary olarak Windows'a paketlenip Tauri/Electron arayüzü ile masaüstü "Hepsi-Bir-Arada PDF Çantası" yapılabilir.
   * **Lisans:** GPLv3.

3. **PDF Arranger (`pdfarranger/pdfarranger`) - Python / GTK / pikepdf**
   * **Neden Önemli?** Sayfaları görsel olarak sürükle-bırak yöntemiyle sıralama, bölme, döndürme, kırpma ve birleştirmede dünyanın en stabil araçlarındandır.
   * **Faydası:** Sayfa manipülasyon algoritmasını ve `pikepdf` (QPDF C++ kütüphanesi sarmalayıcısı) kullanımını referans almak için mükemmeldir.

4. **SumatraPDF (`sumatrapdfreader/sumatrapdf`) - C / C++ / Win32**
   * **Neden Önemli?** Dünyanın en hafif ve ultra hızlı Windows PDF/e-kitap okuyucusudur. Çekirdeğinde MuPDF kullanır.
   * **Faydası:** Saf hız ve bellek yönetimi optimizasyonlarını incelemek için bir başvuru kaynağıdır.

---

### B. Çekirdek Kütüphaneler ve Motorlar (Modüler Mimari Bileşenleri)
1. **Mozilla PDF.js (`mozilla/pdf.js`) - JavaScript / Canvas / SVG**
   * Tarayıcı standardıdır. Dahili olarak metin katmanı, çizim (freehand ink), metin kutusu ekleme, resim damgası ve **AcroForm doldurma** yeteneklerine sahiptir.
   * *Lisans:* Apache-2.0 (Ticari ve açık kaynakta tamamen serbest).

2. **PDFium (`Google / Chromium PDF Engine`) - C++**
   * Google Chrome'un içinde çalışan ultra hızlı, endüstri standardı render motoru.
   * *Kullanım Alanı:* Masaüstü yerel katmanında yüksek kaliteli sayfa işleme ve dönüştürme için en kararlı motordur.

3. **PDF-Lib (`Hopding/pdf-lib`) - TypeScript / JavaScript**
   * İstemci tarafında sıfırdan PDF oluşturma, mevcut PDF'e sayfa ekleme/çıkarma, form doldurma ve font gömme için mükemmeldir. Harici bir C++ veya Python derleyicisi gerektirmez.

4. **PyMuPDF / MuPDF (`pymupdf/PyMuPDF`) - Python / C**
   * Metin çıkarma, PDF içi doğrudan metin/koordinat arama, yüksek çözünürlüklü rasterizasyon ve gelişmiş anotasyonlar için en hızlı kütüphanedir.

5. **QPDF (`qpdf/qpdf`) - C++**
   * Bozuk PDF'leri onarma, doğrusallaştırma (Fast Web View), şifre çözme/şifreleme ve sayfa dönüşümleri için endüstri standardıdır.

---

## 3. Önerilen En Modern & Ölçeklenebilir Sistem Mimarisi

Uzun vadede Adobe Acrobat seviyesinde bir araç geliştirmek için hem UI esnekliği hem de donanım performansı sunan **Tauri v2 + Web Teknolojileri + Rust/C++ Motoru** mimarisi tavsiye edilir.

```
┌────────────────────────────────────────────────────────────────────────┐
│                      KULLANICI ARAYÜZÜ (FRONTEND)                       │
│  React 18/19 + Tailwind CSS + Lucide Icons + Radix UI (Fluent Design)  │
│  - Çoklu Sekme Yöneticisi (Multi-tab Document Viewer)                  │
│  - Sol Panel: Küçük Resimler (Thumbnails), Katmanlar, Yer İmleri      │
│  - Üst Panel: Acrobat Tarzı Dinamik Araç Çubuğu (Viewer/Edit/Forms)   │
│  - Merkez Alan: Virtualized Canvas / SVG Overlay (Mozilla PDF.js)      │
└───────────────────────────────────┬────────────────────────────────────┘
                                    │ IPC (Tauri IPC Bridge / JSON-RPC)
┌───────────────────────────────────▼────────────────────────────────────┐
│                    MASAÜSTÜ ÇEKİRDEĞİ (TAURI / RUST)                   │
│  - Windows WebView2 Entegrasyonu (<20 MB Kurulum Boyutu, <60 MB RAM)  │
│  - Yerel Dosya Sistemi Erişimi, Hızlı I/O, Bellek Eşlemeli Dosyalar    │
│  - Windows Shell Entegrasyonu (Sağ tık, Dosya İlişkilendirme)          │
└───────────────────────────────────┬────────────────────────────────────┘
                                    │ Native C FFI / Binary Sidecar
┌───────────────────────────────────▼────────────────────────────────────┐
│                  ARKA PLAN PDF İŞLEME MOTORU (ENGINES)                 │
│  1. PDFium / MuPDF (Hızlı Rasterizasyon, Vektör Render)               │
│  2. QPDF / PDF-Lib (Sayfa Yapısı, Birleştirme, Şifreleme)              │
│  3. Tesseract OCR (Taranmış Dokümanları Aranabilir PDF Yapma)          │
│  4. OpenSSL / Cryptography (PAdES / PKCS#7 Dijital İmza)               │
└────────────────────────────────────────────────────────────────────────┘
```

### Neden Electron Değil de Tauri?
* **Electron:** Kurulum dosyası ~120-150 MB, boşta RAM kullanımı ~150-250 MB'tır. PDF gibi büyük dosyalar açıldığında çökmeler ve bellek şişmeleri yaşanabilir.
* **Tauri (v2):** Windows'un kendi içinde gelen `WebView2` çekirdeğini kullanır. Kurulum dosyası ~10-18 MB, RAM tüketimi ~35-60 MB civarındadır. Rust sayesinde yerel C/C++ PDF kütüphanelerine (PDFium, QPDF) doğrudan bağlanabilir.

---

## 4. Adobe Acrobat Özellik Seviyeleri ve Adım Adım Yol Haritası

Projeyi adım adım inşa ederek her aşamada çalışan bir sürüm elde etmek en doğru yaklaşımdır:

### Faz 1: Temel Görüntüleyici ve Sayfa Yönetim Paneli (1-2 Ay)
* [x] **Çoklu Belge Desteği:** Sekmeli (Tabs) arayüz ile birden fazla PDF açabilme.
* [x] **Görüntüleme Motoru:** Sayfa yakınlaştırma (Zoom in/out, fit width, fit page), sayfa döndürme, sürekli kaydırma (continuous scroll).
* [x] **Sol Panel (Thumbnail View):** Sayfaların küçük önizlemeleri, sürükle-bırak ile sayfaların sırasını değiştirme.
* [x] **Sayfa İşlemleri:** Sayfa silme, yeni boş sayfa ekleme, başka PDF'ten sayfa çekip ekleme, sayfaları dışa aktarma (Extract Pages).
* [x] **Birleştirme / Bölme (Merge & Split):** Birden fazla PDF'i tek dosyada toplama veya sayfalarına göre bölme.

### Faz 2: Açıklama, Çizim ve Notlandırma Araçları (Annotation Layer) (2-3 Ay)
* [x] **Metin Vurgulama:** Metin seçip Vurgu (Highlight), Altı Çizili (Underline), Üstü Çizili (Strikethrough) yapma.
* [x] **Serbest Çizim ve Şekiller:** Kalem aracı, dikdörtgen, daire, ok, çizgi ve bulut şekilleri ekleme.
* [x] **Yapışkan Notlar:** Tıklanabilir yorum ikonları ve kenar çubuğu yorum paneli.
* [x] **Damga (Stamps):** "Onaylandı", "Taslak", "Gizli" gibi hazır veya özel görsel damgalar basabilme.

### Faz 3: İnteraktif Formlar (AcroForms) ve Dijital İmza (3-4 Ay)
* [x] **Form Alanı Okuma ve Doldurma:** Metin kutuları, onay kutuları (checkbox), radyo butonları, açılır menüler (dropdown).
* [x] **Form Oluşturma Modu:** Kullanıcının PDF üzerine yeni form alanları sürükleyip bırakabilmesi (AcroForms designer).
* [x] **Form Düzleştirme (Flattening):** Doldurulan formu salt okunur kalıcı bir PDF'e dönüştürme.
* [x] **İmza Modülü:**
  * *Basit İmza:* Ekrana fare/kalemle imza çizip veya resim yükleyip sayfaya yapıştırma.
  * *Kriptografik İmza (PAdES):* `.pfx` / `.p12` sertifikası ile SHA-256 tabanlı, belgenin değiştirilmediğini garanti eden dijital imza.

### Faz 4: Doğrudan Metin ve İçerik Düzenleme (In-Place Direct Edit) (4-6 Ay)
* *Bu aşama PDF geliştirmenin en zor kısmıdır.*
* [x] **Metin Bloklarını Algılama:** PDF akışındaki metin gliflerini ve koordinatlarını bloklar halinde gruplama (Text Flow Reconstruction).
* [x] **Metin Düzenleme:** Mevcut yazı tipini (Embedded Font) tespit ederek aynı fontla metin değiştirme, silme veya ekleme.
* [x] **Görsel Değiştirme / Yeniden Boyutlandırma:** PDF içindeki gömülü XObject görselleri seçip taşıma, silme veya yenisiyle değiştirme.

### Faz 5: Gelişmiş Pro Araçları (OCR, Dönüştürme, Güvenlik) (6+ Ay)
* [x] **OCR (Optik Karakter Tanıma):** Tesseract OCR motoru ile taranmış resim PDF'leri seçilebilir ve aranabilir hale getirme (Sandwich PDF tekniği).
* [x] **Dönüştürücüler:** PDF -> Word (.docx), PDF -> Excel (.xlsx), PDF -> Resim (.png, .jpeg).
* [x] **Sansürleme (Redaction):** Hassas verileri sadece siyah bantla kapatmak yerine PDF kodundan kalıcı olarak kazıyarak silme (güvenlik standardı).
* [x] **Güvenlik & Şifreleme:** AES-256 bit kullanıcı ve sahip parolası atama, kopyalama/yazdırma izinlerini kısıtlama.

---

## 5. Kritik Teknik Tuzaklar ve Çözüm Yolları

Geliştirme sürecinde karşılaşacağınız en büyük teknik engeller ve bunların endüstri standardı çözümleri:

### 1. "PDF Bir Kelime İşlemci Belgesi Değildir" Yanılgısı
* **Tuzak:** PDF dosyalarında "paragraf", "satır sonu" veya "otomatik kaydırma" kavramı yoktur. Her harf `(x, y)` koordinatında birer çizim komutudur. Kullanıcı bir kelimeyi sildiğinde sağdaki kelimeler otomatik olarak sola kaymaz.
* **Çözüm:** Metin düzenleme modunda, tıklanan cümlenin etrafındaki karakterleri birleştirip bir sanal `contenteditable` kutusu oluşturulur. Düzenleme bitince eski PDF metin akışı silinir ve yeni metin aynı koordinata yeni bir stream olarak yazılır.

### 2. Font Subsetting ve Gömülmemiş Fontlar
* **Tuzak:** PDF'ler dosya boyutunu küçültmek için fontların yalnızca kullanılan harflerini dosyaya gömer (Subsetting). Örneğin metinde "z" harfi hiç geçmediyse font dosyasında "z" glifi bulunmaz. Kullanıcı "z" yazmaya çalıştığında karakter bozuk görünür veya hata verir.
* **Çözüm:** Standart sistem fontları (Arial, Times New Roman, Roboto vb.) ile eşleştirme (Font Fallback) mekanizması kurulmalı ve yeni metin eklendiğinde fontun tamamı (Full Font Embedding) PDF'e entegre edilmelidir.

### 3. Yüksek Sayfa Sayılı Dosyalarda RAM Şişmesi
* **Tuzak:** 500 sayfalık bir mimari çizim veya taranmış kitap açıldığında tüm sayfalar aynı anda canvas'a çizilirse RAM 2-3 GB'a fırlar ve uygulama çöker.
* **Çözüm:** **Virtual Scrolling (Sanal Kaydırma)** uygulanmalıdır. Yalnızca ekranda görünen aktif 3-4 sayfa render edilir; ekrandan çıkan sayfaların canvas bellekleri `destroy()` edilerek RAM boşaltılır.

### 4. Lisanslama Riskleri (Açık Kaynak Mayın Tarlası)
* **Tuzak:** `MuPDF` kütüphanesi varsayılan olarak **AGPLv3** lisansına sahiptir. Eğer projenizi tamamen açık kaynak yapmayacaksanız veya ileride kısmen kapalı/özel bir modele geçecekseniz AGPL yasal sorun yaratabilir.
* **Çözüm:** 
  * Görüntüleme için: **PDFium** (Apache 2.0) veya **PDF.js** (Apache 2.0).
  * Manipülasyon için: **PDF-Lib** (MIT), **QPDF** (Apache 2.0) veya **pikepdf** (MPL-2.0).
  * Bu kütüphanelerle tamamen güvende olursunuz.

---

## 6. Windows Entegrasyonu ve Dağıtım Stratejisi

Programınızı standart bir web sitesi veya basit bir `.exe` olmaktan çıkarıp profesyonel bir Windows yazılımına dönüştürecek özellikler:

1. **Varsayılan PDF Okuyucu Olarak Ayarlama:**
   * Windows Registry anahtarlarını (`HKEY_CLASSES_ROOT\.pdf`) NSIS kurulum betiği üzerinden kaydederek "Birlikte Aç" ve varsayılan uygulama entegrasyonu sağlamak.
2. **Windows Explorer Küçük Resim Önizleme (Thumbnail Provider):**
   * Windows Gezgini'nde PDF dosyalarının ilk sayfasını simge olarak gösteren bir C++ Shell Extension DLL'i eklemek.
3. **Windows Sağ Tık Menüsü (Context Menu):**
   * Dosyaya sağ tıklandığında *"Bu PDF'i Böl"*, *"PDF'leri Birleştir"*, *"PDF'i Küçült"* gibi doğrudan kısayol komutları sunmak.
4. **Otomatik Güncelleme (Auto-Updater):**
   * Tauri'nin dahili Ed25519 imzalı otomatik güncelleme altyapısını kullanarak kullanıcıya tek tıkla güncelleme ulaştırmak.
5. **Kurulum Paketi:**
   * NSIS veya WiX (MSI) tabanlı temiz, imzalanmış kurulum sihirbazı.

---

## 7. Önerilen Teknoloji Yığını ve Klasör Mimarisi

```
my-pdf-suite/
├── src-tauri/                     # Masaüstü Katmanı (Rust)
│   ├── Cargo.toml
│   ├── src/
│   │   ├── main.rs               # Pencere yönetimi, menüler, sistem tepsisi
│   │   ├── pdf_engine.rs         # PDFium / Native C++ FFI köprüsü
│   │   ├── commands/             # Frontend'in çağırdığı Rust fonksiyonları
│   │   │   ├── file_io.rs        # Hızlı dosya okuma/yazma
│   │   │   ├── page_ops.rs       # Merge, split, rotate komutları
│   │   │   └── security.rs       # Şifreleme ve dijital imza
│   │   └── sidecars/             # Harici yardımcı binary'ler (Tesseract OCR vb.)
│
├── src/                          # Kullanıcı Arayüzü (React + TypeScript)
│   ├── components/
│   │   ├── viewer/               # PDF.js Canvas görüntüleyici ve sanal liste
│   │   ├── toolbar/              # Üst dinamik araç çubuğu (Acrobat tarzı)
│   │   ├── sidebar/              # Sol panel (Thumbnails, Yer İmleri, Katmanlar)
│   │   ├── annotations/          # Çizim, not, vurgu katman bileşenleri
│   │   ├── forms/                # AcroForm render ve tasarım araçları
│   │   └── editor/               # In-place metin ve resim düzenleyici
│   ├── hooks/
│   │   ├── usePdfDocument.ts     # Belge yükleme ve durum yönetimi
│   │   └── useVirtualPages.ts    # Bellek tasarrufu için sanal sayfa kaydırma
│   ├── lib/
│   │   ├── pdfjs-custom/         # Özelleştirilmiş PDF.js motoru
│   │   └── pdf-lib-adapter.ts    # İstemci tarafı sayfa manipülasyonları
│   ├── App.tsx
│   └── main.tsx
├── package.json
└── tailwind.config.js
```

---

## 8. Hemen Başlamak İçin Eyleme Geçilebilir İlk Adımlar

1. **Geliştirme Ortamını Hazırlayın:**
   * Node.js (v20+), Rust & Cargo, Visual Studio C++ Build Tools ve VS Code kurulumlarını tamamlayın.
2. **Temel Tauri + React Şablonunu Başlatın:**
   ```bash
   npm create tauri-app@latest my-pdf-app -- --template react-ts
   cd my-pdf-app
   npm install pdfjs-dist pdf-lib lucide-react clsx tailwindcss
   ```
3. **İlk Prototipi Çalıştırın:**
   * `pdfjs-dist` paketini entegre ederek local bir PDF dosyasını okuyun, zoom ve sayfa geçiş kontrollerini bağlayın.
4. **Sayfa Düzenleme Modülünü Ekleyin:**
   * `pdf-lib` kullanarak seçilen sayfayı 90 derece döndürüp yeni dosya olarak kaydeden ilk backend/frontend köprüsünü oluşturun.
5. **Form ve Not Katmanlarını Aktif Edin:**
   * PDF.js'in dahili `AnnotationEditorParams` ve form katmanlarını devreye alarak Acrobat benzeri çizim ve metin kutusu ekleme özelliklerini test edin.