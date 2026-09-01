# VectorPDF Dönüştürme, Dışa Aktarma ve PDF Oluşturma Destek Matrisi

**Belge Sürümü:** 1.0.0  
**Tarih:** 2026-09-01  
**Kanonik Konum:** `docs/PDF_CONVERSION_SUPPORT_MATRIX.md`  
**İlgili Modül:** `VectorPDFConversion` (`vectorpdf::conversion`)

---

## 1. Genel Bakış

VectorPDF Conversion & Export Engine; PDF belgelerini modern ofis formatlarına, görüntülere ve arşiv standartlarına dönüştüren, aynı zamanda çeşitli kaynaklardan (Ofis, Markdown, HTML, Görseller, Pano, Ekran Alıntısı, Klasörler) yüksek kaliteli PDF oluşturan bağımsız, çevrimdışı ve tam güvenlikli bir C++ alt sistemidir.

---

## 2. PDF'den Dışa Aktarma (Export) Matrisi

| Hedef Format | Uzantı | Motor / Altyapı | Doğruluk / Sadakat Modları | Harici Bağımlılık | Doğrulama & Garanti |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **Microsoft Word** | `.docx` | Yerleşik `DocxPackageWriter` + `zlib` | • Reflow (Akan/Düzenlenebilir Metin)<br>• Visual Fidelity (Birebir Sayfa Görüntüsü) | **Yok** (Tamamen yerleşik pure C++) | OOXML OPC ZIP, `[Content_Types].xml`, `word/document.xml` yapısı doğrulanır. |
| **Microsoft Excel** | `.xlsx` | Yerleşik `XlsxPackageWriter` + `TableDetector` + `zlib` | • Algılanan Tablolar (Detected Tables)<br>• Sayfa Izgarası (Page Layout Grid) | **Yok** (Tamamen yerleşik pure C++) | SpreadsheetML, `xl/workbook.xml`, `xl/worksheets/sheet1.xml` doğrulanır. |
| **Microsoft PowerPoint** | `.pptx` | Yerleşik `PptxPackageWriter` + `zlib` | • Visual Slide (Yüksek Çözünürlüklü Slayt)<br>• Düzenlenebilir Metin Kutuları Önizlemesi | **Yok** (Tamamen yerleşik pure C++) | PresentationML, `ppt/presentation.xml`, `ppt/slides/slide1.xml` doğrulanır. |
| **PNG Görüntü** | `.png` | `pdf::PDFRenderer` + `QImageWriter` | 72 - 1200 DPI, Şeffaf/Beyaz Arka Plan | **Yok** (Qt6 / LibPNG yerleşik) | PNG Magic bytes (`89 50 4E 47 0D 0A 1A 0A`) ve `QImageReader` boyutu. |
| **JPEG Görüntü** | `.jpg` | `pdf::PDFRenderer` + `QImageWriter` | 72 - 1200 DPI, %1-100 Kalite | **Yok** (Qt6 / LibJPEG yerleşik) | JPEG SOI/EOI (`FF D8 ... FF D9`) kontrolü. |
| **TIFF Görüntü** | `.tiff` | `pdf::PDFRenderer` + `QImageWriter` | Çok sayfalı tek dosya veya Sayfa başına ayrı TIFF | **Yok** (Qt6 TIFF yerleşik) | TIFF Header (`II` / `MM` + `00 2A`) kontrolü. |
| **WebP Görüntü** | `.webp` | `pdf::PDFRenderer` + `QImageWriter` | Kayıpsız / Kayıplı sıkıştırma, 72-600 DPI | **Yok** (Qt6 WebP yerleşik) | WebP RIFF header kontrolü. |
| **BMP Görüntü** | `.bmp` | `pdf::PDFRenderer` + `QImageWriter` | 72 - 600 DPI | **Yok** (Qt6 BMP yerleşik) | BMP `BM` magic bytes kontrolü. |
| **PDF/A-1b, 2b, 3b, 4** | `.pdf` | `PDFDocumentBuilder` + `PDFDocumentSanitizer` | Arşiv Uyumluluğu, Yasaklı Eylemlerin Temizlenmesi | **İsteğe Bağlı:** veraPDF CLI (Bağımsız Denetim) | PDF Yapısı + veraPDF doğrulama raporu. |
| **Monochrome PDF** | `.pdf` | `ImageProcessing::toMonochrome` + `QPdfWriter` | • Otsu Otomatik Binarizasyon<br>• Sabit Eşik (128)<br>• Aranabilir Metin Katmanı | **Yok** (Yerleşik C++) | PDF Header ve sayfa sayısı doğrulanır. |
| **XFDF Form Verisi** | `.xfdf` | `XfdfExporter` + `QXmlStreamWriter` | Adobe Standart XML XFDF | **Yok** (Qt6 XML yerleşik) | XML Sözdizimi ve `xfdf/fields` etiketleri doğrulanır. |
| **FDF Form Verisi** | `.fdf` | `FdfExporter` | Standart Adobe FDF-1.2 formatı | **Yok** (Yerleşik C++) | FDF Sözdizimi (`%FDF-1.2 ... %%EOF`) doğrulanır. |

---

## 3. PDF Oluşturma (Create PDF) Matrisi

| Kaynak Türü | Giriş Formatları | Motor / İşleyici | Desteklenen Seçenekler & Özellikler |
| :--- | :--- | :--- | :--- |
| **Tek Dosya** | `.docx`, `.doc`, `.html`, `.md`, `.png`, `.jpg`, vb. | İlgili Creator İşleyicisi | Otomatik MIME tespiti, sayfa boyutu, kenar boşluğu |
| **Çoklu Görüntü** | `.png`, `.jpg`, `.jpeg`, `.tif`, `.tiff`, `.webp`, `.bmp` | `ImagePdfCreator` | Otomatik A4 veya Doğal DPI boyutu, ölçekleme ve ortalama |
| **Pano (Clipboard)** | Sistem Panosu Görüntü Verisi | `QClipboard` + `ImagePdfCreator` | Geçici kum havuzunda binarizasyon ve doğrudan PDF |
| **Ekran Görüntüsü** | Tam Ekran veya Seçili Bölge (Snip) | `PDFScreenshotDialog` + `ImagePdfCreator` | Çift monitör desteği, hassas piksel bölge seçimi |
| **Markdown** | `.md`, `.markdown` | `MarkdownPdfCreator` | **Temalar:** Clean (Kurumsal), Academic (Serif/Times), Compact (Teknik), Dark-on-Light. GitHub tabloları, kod blokları, yerel resimler. |
| **HTML Belgesi** | `.html`, `.htm` | `HtmlPdfCreator` | Yerel kaynak çözümleme, güvenli çevrimdışı işleme (uzak URL'ler engellenir). |
| **Ofis Belgeleri** | `.docx`, `.doc`, `.odt`, `.rtf` | `OfficePdfCreator` + `LibreOfficeWorker` | Kum havuzunda profil yalıtımlı arkaplan `soffice` işlemi. |
| **Klasör / Toplu** | Karışık formatlı klasör ağacı | `FolderPdfCreator` + `PDFDocumentManipulator` | Yinelemeli tarama, doğal sıralama, dosya adı yer imleri, hata izolasyonu. |

---

## 4. Güvenlik ve Gizlilik Garantileri

1. **Çevrimdışı İlk (Offline-First):** Dönüştürme motoru hiçbir harici ağ, telemetri veya bulut sunucusuna bağlanmaz.
2. **Kum Havuzu ve Atomik Kayıt:** Tüm ara dosyalar `TempFileGuard` tarafından `vpdf_` ön ekli güvenli geçici dizinlerde oluşturulur. Orijinal dosyaya doğrudan yazılmaz; işlem başarılı olunca `atomicCommit` ile hedef yola taşınır.
3. **Orijinal Dosyayı Koruma:** Çıktı hedefi kaynak dosya ile aynıysa veya var olan dosyanın üzerine yazma izni yoksa `FilenamePolicy` otomatik numara ekler (`dosya (1).ext`).
4. **Veri Sızıntısını Önleme:** Günlüklerde (log) kullanıcı belgesinin ham metni veya hassas baytları yer almaz; yalnızca aşama, süre ve dosya boyutu gibi teknik metrikler tutulur.
