# Ürün Sözleşmesi

**Durum:** Active baseline 0.2  
**Son doğrulama:** 2026-09-01  
**Ürün adı:** VectorPDF

## Vizyon

Windows kullanıcılarının belgelerini bir sunucuya yüklemek zorunda kalmadan okuyabildiği, düzenleyebildiği, form doldurabildiği, sayfa işlemleri yapabildiği ve ilerleyen sürümlerde güvenilir profesyonel PDF iş akışlarına erişebildiği ücretsiz bir masaüstü uygulaması üretmek.

“Acrobat alternatifi” uzun vadeli yönü tarif eder; ilk sürüm vaadi değildir. Her sürüm yalnız kanıtlanmış iş akışlarını ilan eder.

## Birincil kullanıcılar

- Günlük PDF okuma, arama, açıklama ve form doldurma ihtiyacı olan Windows kullanıcıları
- Sayfa birleştirme/bölme/döndürme/sıralama yapan ofis kullanıcıları
- Türkçe ve İngilizce taranmış belgelerde çevrimdışı OCR isteyen kullanıcılar
- Belgesini üçüncü taraf buluta göndermek istemeyen kişi ve küçük ekipler

Profesyonel baskı öncesi, hukukî nitelikli imza otoritesi, CAD/BIM incelemesi ve kurumsal DMS ilk kullanıcı profili değildir; ileride ayrı doğrulama ve ürün kararı gerektirir.

## Ürün ilkeleri

1. **Belge güvenliği önce gelir.** Veri kaybı, sessiz bozulma veya sahte güven göstergesi bir özellik eksiğinden daha ağır kusurdur.
2. **Çevrimdışı varsayılan.** Açma, görüntüleme, düzenleme, form ve temel OCR yerelde çalışır. Ağ gerektiren özellik açık rıza ve ayrı politika ister.
3. **Doğru adlandırma.** Overlay, gerçek içerik düzenleme, görsel imza, dijital imza ve redaction birbirine karıştırılmaz.
4. **Hazır sistemi akıllıca kullan.** Parser/renderer sıfırdan yazılmaz; fakat upstream iddiaları kendi corpus'umuzla sınanır.
5. **Kayıpsızlık görünürdür.** Bir işlem korunamayan özelliğe dokunacaksa uygulama uyarır, engeller veya açık dönüşüm raporu üretir.
6. **Erişilebilirlik sonradan ek değildir.** Uygulama UI'si ve üretilen PDF'nin erişilebilirliği ayrı gereksinimlerdir.
7. **Marka ve bağımsızlık.** Ürün adı, görsel kimlik, güncelleme kanalı ve destek politikası bize ait olur; upstream telif/lisans/trademark yükümlülükleri korunur.

## İlk sürümün başarı tanımı

Kullanıcı desteklenen bir PDF'yi güvenle açar; hızlı ve net görüntüler; arar/seçer; sayfaları düzenler; temel annotation ve AcroForm alanlarını işler; undo/redo kullanır; çökme halinde oturumunu kurtarır; orijinali bozmadan kaydeder ve çıktı yeniden açılıp doğrulanır.

## İlk sürümde kapsam dışı

- Word benzeri paragraf reflow ile genel PDF metin düzenleme
- PAdES-LT/LTA veya “AB nitelikli imza” garantisi
- XFA düzenleme/çalıştırma
- PDF JavaScript çalıştırma
- Bulut hesabı, eşzamanlı ortak düzenleme ve plugin marketplace
- “Her PDF standardını tam destekler” iddiası
- Kullanıcı belgelerini varsayılan telemetri veya eğitim verisi olarak toplama

## Kabul edilen başlangıç rotası

- **Lisans:** Resmî çekirdek ve bu depo MIT; uygulama ücretsiz. Permissive rota `ADR-0001` ile Accepted.
- **Baseline:** PDF4QT v1.6.0.0 yalnız Gate 1'i geçerse fork edilir; geçmezse PDFium + qpdf + Tesseract tabanlı modüler yedek rota. `ADR-0002` Gate 1 kanıtını bekler.
- **Platform:** Release tarihinde Microsoft desteğindeki Windows 11 x64; Windows 10 desteklenmez, ARM64 kanıt üretilene kadar deferred. `ADR-0003` bağlayıcıdır.
- **Dağıtım:** İlk kanal Microsoft Store/MSIX; doğrudan kanal ve özel updater ayrı kabul kararı ister.
- **Gizlilik:** Offline/account-free temel ürün; otomatik telemetry upload, reklam ve belge verisi satışı yok. `ADR-0004` bağlayıcıdır.
- **Sürdürülebilirlik:** Bağış/sponsorluk, ücretli destek/entegrasyon ve ileride ayrı değerlendirilen OEM/kurumsal hizmetler; temel ücretsiz çekirdek ağ/abonelik kilidine çevrilmez.

## Açık ürün kararları

- Nihai ürün adı **VectorPDF** olarak product owner tarafından belirlendi; M0 rebrand öncesi GitHub/domain/trademark ön taraması tamamlanacak.
- Tüzel publisher/display identity ve Store hesabı M0/release hazırlığında kesinleşecek.
- İleride ayrı ücretli/kapalı modül veya cloud servis önerilirse yeni lisans, privacy ve data-flow ADR'si gerekecek.

Gemini veya başka bir ajan açık kararları kendiliğinden varsayamaz; Accepted ADR olmadan ürüne gömemez.
