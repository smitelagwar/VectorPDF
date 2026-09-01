# VectorPDF — Kullanıcı Rehberi (v1.0)

VectorPDF; Windows 11 için geliştirilmiş modern, hızlı, yüksek güvenlikli ve Türkçe dil destekli açık kaynak bir PDF görüntüleme ve düzenleme yazılım paketidir.

---

## 1. Uygulamalar ve Özellikler

### 1.1 VectorPDF Görüntüleyici (VectorPDF Viewer)
- **Hızlı Belge Açma:** Büyük boyutlu PDF dosyalarını dahi bellek dostu sanallaştırma ile anında açar.
- **Sayfa Düzenleri:** Tek sayfa, sürekli dikey kaydırma, çift sayfa (kitap görünümü) ve çift sütun modları.
- **Gelişmiş Arama:** Büyük/küçük harf duyarlı, tüm sözcük ve Düzenli İfade (Regex) seçenekleriyle belge içi anlık metin arama ve sarı eşleşme vurguları.
- **Metin Seçimi ve Kopyalama:** Metin bloklarını seçip tek tıkla veya `Ctrl+C` ile panoya aktarma.

### 1.2 VectorPDF Düzenleyici (VectorPDF Editor)
- **Metin Akışı Düzenleme:** Belge içindeki metin bloklarını doğrudan tıklayarak düzenleme, yazı tipi değiştirme ve hizalama.
- **Açıklama (Annotation) Araçları:**
  - Metin Vurgulama (Highlight), Altı Çizili (Underline), Üstü Çizili (Strikeout)
  - Serbest Metin Kutusu (FreeText / Typewriter)
  - Kalem & Mürekkep Çizimi (Ink stylus desteği)
  - Geometrik Şekiller (Dikdörtgen, Daire, Çizgi, Çokgen)
  - Hazır ve Özel Damgalar (Stamps - "Onaylandı", "Gizli" vb.)
  - Yapışkan Notlar (Sticky Notes)
- **AcroForm Doldurma ve Tasarımı:** Etkileşimli form alanlarını (metin kutusu, onay kutusu, radyo düğmesi, açılır liste) doldurma, yeni alanlar ekleme ve sekme sırasını belirleme.
- **Kalıcı Karartma (True Redaction):** Hassas kişisel verileri (TCKN, telefon, isim) içerik akışındaki metin glifleri ve görsel pikselleri düzeyinde kalıcı ve geri döndürülemez biçimde yok etme.

### 1.3 VectorPDF Sayfa Yöneticisi (VectorPDF PageMaster)
- **Sayfa Düzenleme:** Sayfaları fareyle sürükleyip bırakarak yeniden sıralama.
- **Döndürme & Silme:** Seçili sayfaları 90°/180°/270° döndürme veya belgeden çıkarma.
- **Birleştirme & Bölme (Merge & Split):** Birden fazla PDF dosyasını tek bir belgede birleştirme veya sayfaları ayrı dosyalara bölme.
- **Sayfa Ekleme:** Boş sayfa veya harici görsellerden (JPEG, PNG, TIFF, WebP) sayfa oluşturup araya ekleme.

### 1.4 VectorPDF Belge Karşılaştırma (VectorPDF Diff)
- İki farklı PDF belgesini yan yana açarak metin, görsel ve düzen farklarını piksel ve nesne bazında otomatik tespit edip renklendirerek gösterme.

### 1.5 Otomasyon ve Toplu İşlem Kuyruğu (VectorPDF Batch CLI)
- Komut satırından ve arayüzden birden çok PDF dosyasını aynı anda dönüştürme, OCR'dan geçirme, optimize etme, sayfa ayıklama ve PDF/A doğrulama kuyruğuna alma.
- Gerçek zamanlı ilerleme yüzdesi, hata durumunda otomatik yeniden deneme (`retry`) ve JSON yürütme raporu çıktısı.

### 1.6 Eklenti Mimarisi ve Çevrimdışı Yapay Zekâ (Plugins & Local AI)
- **Güvenli Eklentiler:** İzin tabanlı manifest yapısıyla çalışan, yalnızca kullanıcının izin verdiği yetkilere (`ReadDocument`, `ModifyDocument`) erişebilen modüler eklenti sistemi.
- **Çevrimdışı Yerel Yapay Zekâ:** Açık kullanıcı onayıyla çalışan, belgelerinizi internete aktarmadan yerel işlemci üzerinde özet çıkaran, anahtar kavramları listeleyen ve prompt injection güvenlik filtresine sahip AI yardımcı katmanı.

---

## 2. Güvenlik ve Gizlilik Politikası

1. **Atomik Kayıt Hattı:** VectorPDF dosyalarınızı asla doğrudan üzerine yazmaz. Değişiklikler önce geçici bir dosyaya yazılır, doğrulanır ve ancak doğrulama başarılı olursa orijinal dosya güvenle yenilenir.
2. **Dijital İmza Koruması:** Dijital imza içeren belgeler açıldığında doğrudan üzerine kayıt engellenir; imzanın kriptografik bütünlüğünü bozmamak için kullanıcı yeni bir kopya olarak kaydetmeye ("Farklı Kaydet") yönlendirilir.
3. **Zararlı İçerik Kalkanı:** PDF dosyalarındaki JavaScript, harici program başlatma (Launch action) ve güvensiz dış bağlantılar varsayılan olarak engellenir.
4. **Çevrimdışı ve Gizli:** VectorPDF tamamen çevrimdışı (offline) çalışır; kullanıcı belgelerini veya verilerini asla harici bir sunucuya göndermez.

---

## 3. Kısayol Tuşları Listesi

| Kısayol | İşlem |
|---|---|
| `Ctrl + O` | Belge Aç |
| `Ctrl + S` | Belgeyi Kaydet |
| `Ctrl + Shift + S` | Farklı Kaydet |
| `Ctrl + P` | Yazdır |
| `Ctrl + F` | Metin Arama Çubuğunu Aç |
| `Ctrl + Shift + F` | Gelişmiş Arama Paneli (Regex destekli) |
| `F3` / `Shift + F3` | Sonraki / Önceki Eşleşmeye Git |
| `Ctrl + Z` | Geri Al (Undo) |
| `Ctrl + Y` veya `Ctrl + Shift + Z` | Yinele (Redo) |
| `Ctrl + +` / `Ctrl + -` | Yakınlaştır / Uzaklaştır |
| `Ctrl + 0` | Sayfayı Ekrana Sığdır (Fit Page) |
| `Ctrl + 1` | Genişliğe Sığdır (Fit Width) |
| `Ctrl + R` | Sayfayı 90° Sağa Döndür |
| `Ctrl + L` | Sayfayı 90° Sola Döndür |
| `F11` | Tam Ekran Modu |
| `Ctrl + Q` | Uygulamadan Çık |

---

## 4. Sistem Gereksinimleri

- **İşletim Sistemi:** Windows 10 (Sürüm 1809+) veya Windows 11 (x64 / ARM64)
- **Bellek:** En az 2 GB RAM (Büyük taranmış arşivler için 4 GB+ önerilir)
- **Disk Alanı:** 150 MB boş alan
