# VectorPDF Dönüştürme, Dışa Aktarma ve PDF Oluşturma Destek Matrisi

**Belge Sürümü:** 1.1.0  
**Tarih:** 2026-09-01  
**Kanonik Konum:** `docs/PDF_CONVERSION_SUPPORT_MATRIX.md`  
**İlgili Modül:** `VectorPDFConversion` (`vectorpdf::conversion`)

---

## 1. Genel Bakış

VectorPDF Conversion & Export Engine; PDF belgelerini modern ofis formatlarına, görüntülere ve arşiv standartlarına dönüştüren, aynı zamanda çeşitli kaynaklardan (Ofis, Markdown, HTML, Görseller, Pano, Ekran Alıntısı, Tarayıcı/WIA, Klasörler) yüksek kaliteli PDF oluşturan bağımsız, çevrimdışı ve tam güvenlikli bir C++ alt sistemidir.

---

## 2. PDF'den Dışa Aktarma (Export) Matrisi

| Hedef Format | Uzantı | Motor / Altyapı | Doğruluk / Sadakat Modları | Harici Bağımlılık | Doğrulama & Garanti |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **Microsoft Word** | `.docx` | Yerleşik `DocxPackageWriter` + `zlib` | • Reflow (Düzenlenebilir Metin, en iyi çaba)<br>• Visual Fidelity (Birebir Sayfa Görüntüsü, yüksek sadakat) | **Yok** (Tamamen yerleşik pure C++) | `OoxmlPackageValidator` ile `[Content_Types].xml`, `_rels/.rels`, `word/document.xml` ve XML sözdizimi doğrulanır. |
| **Microsoft Excel** | `.xlsx` | Yerleşik `XlsxPackageWriter` + `TableDetector` + `zlib` | • Algılanan Tablolar (Semantik hücre çıkarımı)<br>• Sayfa Izgarası (Bütünleşik düzen ızgarası) | **Yok** (Tamamen yerleşik pure C++) | `OoxmlPackageValidator` ile `xl/workbook.xml`, `xl/worksheets/sheet*.xml` ve XML doğrulanır. |
| **Microsoft PowerPoint** | `.pptx` | Yerleşik `PptxPackageWriter` + `zlib` | • Visual Slide (Yüksek Çözünürlüklü Slayt)<br>• Düzenlenebilir Metin Kutuları Önizlemesi | **Yok** (Tamamen yerleşik pure C++) | `OoxmlPackageValidator` ile `ppt/presentation.xml`, `ppt/slides/slide*.xml` doğrulanır. |
| **PNG Görüntü** | `.png` | `pdf::PDFRenderer` + `QImageWriter` | 72 - 1200 DPI, Şeffaf/Beyaz Arka Plan | **Yok** (Qt6 / LibPNG yerleşik) | PNG Magic bytes (`89 50 4E 47 0D 0A 1A 0A`) ve `QImageReader` boyutu. |
| **JPEG Görüntü** | `.jpg` | `pdf::PDFRenderer` + `QImageWriter` | 72 - 1200 DPI, %1-100 Kalite | **Yok** (Qt6 / LibJPEG yerleşik) | JPEG SOI/EOI (`FF D8 ... FF D9`) kontrolü. |
| **TIFF Görüntü** | `.tiff` | `MultiPageTiffWriter` (Yerleşik C++) | Çok sayfalı tek TIFF veya Sayfa başına ayrı TIFF (72-1200 DPI) | **Yok** (Yerleşik C++ IFD zincirleme) | `MultiPageTiffWriter::countDirectories` ile dizin sayısı = sayfa sayısı doğrulanır. |
| **WebP Görüntü** | `.webp` | `pdf::PDFRenderer` + `QImageWriter` | Kayıpsız / Kayıplı sıkıştırma, 72-600 DPI | **Yok** (Qt6 WebP yerleşik) | WebP RIFF header kontrolü ve runtime codec probing. |
| **BMP Görüntü** | `.bmp` | `pdf::PDFRenderer` + `QImageWriter` | 72 - 600 DPI | **Yok** (Qt6 BMP yerleşik) | BMP `BM` magic bytes kontrolü. |
| **PDF/A-1b, 2b, 3b, 4** | `.pdf` | `PdfAConformanceTransformer` + `VeraPdfWorker` | • Mod A: Vektör Koruyarak Dönüştürme<br>• Mod B: Arşivsel Raster Güvencesi | **İsteğe Bağlı:** veraPDF CLI (Bağımsız Denetim) | veraPDF mevcutsa kesin `PASS/FAIL` doğrulaması; veraPDF yoksa `UNVALIDATED` uyarısı. |
| **Monochrome PDF** | `.pdf` | `ImageProcessing::toMonochrome` + `QPdfWriter` | • Otsu Otomatik Binarizasyon<br>• Sabit Eşik (128)<br>• Aranabilir Metin Katmanı | **Yok** (Yerleşik C++) | PDF Header, sayfa sayısı ve aranabilir metin roundtrip kontrolü. |
| **XFDF Form Verisi** | `.xfdf` | `XfdfExporter` + `QXmlStreamWriter` | Adobe Standart XML XFDF | **Yok** (Qt6 XML yerleşik) | XML Sözdizimi ve `xfdf/fields` etiketleri doğrulanır. |
| **FDF Form Verisi** | `.fdf` | `FdfExporter` | Standart Adobe FDF-1.2 formatı | **Yok** (Yerleşik C++) | FDF Sözdizimi (`%FDF-1.2 ... %%EOF`) doğrulanır. |

---

## 3. PDF Oluşturma (Create PDF) Matrisi

| Kaynak Türü | Giriş Formatları | Motor / İşleyici | Desteklenen Seçenekler & Özellikler |
| :--- | :--- | :--- | :--- |
| **Tek Dosya** | `.docx`, `.doc`, `.html`, `.md`, `.png`, `.jpg`, vb. | İlgili Creator İşleyicisi | Otomatik MIME tespiti, sayfa boyutu, kenar boşluğu |
| **Çoklu Görüntü** | `.png`, `.jpg`, `.jpeg`, `.tif`, `.tiff`, `.webp`, `.bmp` | `ImagePdfCreator` | Otomatik A4 veya Doğal DPI boyutu, ölçekleme ve ortalama |
| **Pano (Clipboard)** | Sistem Panosu Görüntü Verisi | `QClipboard` + `ImagePdfCreator` | Geçici kum havuzunda binarizasyon ve doğrudan PDF, iş bitiminde otomatik temp temizliği |
| **Ekran Görüntüsü** | Tam Ekran (Tüm Monitörler / Tek Monitör) veya Seçili Bölge (Snip) | `PDFScreenshotDialog` + `ImagePdfCreator` | Çoklu ekran / Sanal masaüstü (Virtual Desktop) birleşik geometri desteği |
| **Tarayıcı (Scanner)** | WIA 2.0 Uyumlu Tarayıcılar / ADF / Düz Yatak | `PDFScanBroker` (`WiaScannerBackend` / `FakeScannerBackend`) | Cihaz algılama, DPI (72-1200), Renkli/Gri, Çift Taraflı (Duplex), sayfa sıralama ve silme |
| **Markdown** | `.md`, `.markdown` | `MarkdownPdfCreator` | **Temalar:** Clean (Kurumsal), Academic (Serif/Times), Compact (Teknik), Dark-on-Light. GitHub tabloları, kod blokları, yerel resimler, Türkçe Unicode desteği. |
| **HTML Belgesi** | `.html`, `.htm` | `HtmlPdfCreator` | Yerel kaynak çözümleme, güvenli çevrimdışı işleme (uzak URL'ler engellenir). |
| **Ofis Belgeleri** | `.docx`, `.doc`, `.odt`, `.rtf` | `OfficePdfCreator` + `LibreOfficeWorker` | İşlem yalıtımlı (Process-isolated) profil ve zaman aşımlı arkaplan `soffice` işlemi. |
| **Klasör / Toplu** | Karışık formatlı klasör ağacı | `FolderPdfCreator` + `PDFDocumentManipulator` | Yinelemeli tarama, doğal sayısal sıralama (`QCollator`), dosya adı yer imleri, hata izolasyonu. |

---

## 4. Güvenlik ve Gizlilik Garantileri

1. **Çevrimdışı İlk (Offline-First):** Dönüştürme motoru hiçbir harici ağ, telemetri veya bulut sunucusuna bağlanmaz.
2. **Kum Havuzu ve Atomik Kayıt:** Tüm ara dosyalar `TempFileGuard` tarafından `vpdf_` ön ekli güvenli geçici dizinlerde oluşturulur. Orijinal dosyaya doğrudan yazılmaz; işlem başarılı olunca `atomicCommit` ile hedef yola taşınır.
3. **Sahip Olunan Geçici Dosyalar (Owned Temp Artifacts):** Pano, ekran alıntısı veya tarama ara dosyaları `ownedTemporaryInputPaths` listesine eklenir ve iş tamamlandığında (`Success`, `Failed`, `Cancelled`, `Unsupported`) anında silinir.
4. **Orijinal Dosyayı Koruma:** Çıktı hedefi kaynak dosya ile aynıysa veya var olan dosyanın üzerine yazma izni yoksa `FilenamePolicy` otomatik numara ekler (`dosya (1).ext`).
5. **Veri Sızıntısını Önleme:** Günlüklerde (log) kullanıcı belgesinin ham metni veya hassas baytları yer almaz; yalnızca aşama, süre ve dosya boyutu gibi teknik metrikler tutulur.
