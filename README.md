# VectorPDF

**VectorPDF**, Windows 11 için tasarlanmış modern, hızlı, yüksek güvenlikli, çevrimdışı (offline-first) ve tam Türkçe destekli açık kaynak bir masaüstü PDF görüntüleme ve düzenleme uygulama paketidir.

---

## 🚀 Uygulamalar

VectorPDF paketi, günlük ihtiyaçlardan profesyonel belge yönetimine kadar uzanan 5 temel uygulamadan oluşur:

1. **VectorPDF Viewer (Görüntüleyici):** Hızlı açılış, bellek dostu sayfa sanallaştırma, tek/çift sayfa ve sürekli kaydırma düzenleri, Regex destekli arama ve metin kopyalama.
2. **VectorPDF Editor (Düzenleyici):** Belge içi metin bloklarını doğrudan düzenleme, 15+ açıklama aracı (vurgulama, altı çizili, mürekkep çizimi, serbest metin kutusu, damgalar), etkileşimli AcroForm doldurma/tasarımı ve kalıcı karartma (True Redaction).
3. **VectorPDF PageMaster (Sayfa Yöneticisi):** Sürükle-bırak sayfa sıralama, döndürme, silme, bölme (split), birleştirme (merge) ve harici görsellerden sayfa ekleme.
4. **VectorPDF Diff (Belge Karşılaştırma):** İki PDF belgesi arasındaki metin, görsel ve nesne farklarını piksel hassasiyetinde karşılaştırıp renklendirerek gösterme.
5. **VectorPDF LaunchPad (Başlatıcı):** Tüm VectorPDF araçlarına ve son açılan belgelere tek ekrandan hızlı erişim.

---

## 🛡️ Güvenlik ve Gizlilik Kırmızı Çizgileri

- **Atomik Kayıt Hattı:** Dosyalar asla doğrudan üzerine yazılmaz. Önce geçici dosyaya yazılır (`QSaveFile`), doğrulanır ve atomik olarak değiştirilir.
- **Dijital İmza Koruması:** İmzalı belgeler açıldığında doğrudan üzerine yazma engellenir; imza bütünlüğünü korumak için yeni bir dosya olarak kaydedilir.
- **Gerçek Kalıcı Redaction:** Hassas veriler yüzeysel siyah dikdörtgenle kapatılmaz; içerik akışındaki metin glifleri ve görsel pikselleri düzeyinde kalıcı silinir.
- **Varsayılan Deny Kalkanı:** PDF JavaScript'leri, Launch action ve güvensiz dış bağlantılar varsayılan olarak engellenir.
- **%100 Çevrimdışı:** Belgeleriniz asla internete veya harici sunuculara aktarılmaz.

---

## 🛠️ Hızlı Derleme (Windows 11 x64)

### 1. Önkoşullar
- **Derleyici:** Visual Studio 2022 (MSVC C++20)
- **Framework & Araçlar:** Qt 6.5+, CMake 3.24+, vcpkg

### 2. Tek Tıkla Derleme ve Test
PowerShell üzerinden doğrudan:
```powershell
.\tools\build_vectorpdf.ps1
```

### 3. Manuel CMake Komutları:
```powershell
# 1. vcpkg bağımlılıkları:
vcpkg install tbb openssl lcms zlib openjpeg freetype libjpeg-turbo libpng blend2d --triplet x64-windows

# 2. CMake Yapılandırması:
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake" -DPDF4QT_QT_ROOT="$env:Qt6_DIR" -DPDF4QT_BUILD_TESTS=ON

# 3. Derleme:
cmake --build build --config Release -j8

# 4. Birim Testleri & Korpus Doğrulaması:
ctest --test-dir build -C Release --output-on-failure
.\tests\run_smoke_tests.ps1
```

---

## 📚 Belgeler

- **Kullanıcı Rehberi:** [`docs/USER_GUIDE_TR.md`](docs/USER_GUIDE_TR.md)
- **Mimari ve Tasarım:** [`docs/ARCHITECTURE.md`](docs/ARCHITECTURE.md)
- **Güncel Durum:** [`STATUS.md`](STATUS.md)
- **Yol Haritası:** [`ROADMAP.md`](ROADMAP.md)
- **Güvenlik ve Gizlilik:** [`docs/SECURITY_AND_PRIVACY.md`](docs/SECURITY_AND_PRIVACY.md)
- **Sürüm Notları:** [`CHANGELOG.md`](CHANGELOG.md)

---

## 📄 Lisans

VectorPDF, **MIT Lisansı** altında dağıtılan özgür ve açık kaynaklı bir yazılımdır.

