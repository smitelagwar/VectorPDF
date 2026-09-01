# [Uygulama Adın] — PDF Uygulaması Yol Haritası ve Karar Dokümanı

> Bu doküman, sıfırdan kendi markanla bir Windows masaüstü PDF uygulaması kurman için yapılan araştırmanın, alınan kararların ve gerekçelerinin tam kaydıdır. Amaç, aylar sonra "bunu neden böyle yapmıştık?" diye sormamak.

---

## 1. Yönetici özeti

Hedefin: kendi ismini taşıyan, Windows'ta çalışan, önce ücretsiz ve kişisel kullanım için başlayıp sonra yayınlanacak bir PDF uygulaması. Adobe Acrobat'ın kapsadığı alanı (görüntüleme, düzenleme, form doldurma, imzalama, OCR, dönüştürme) zamanla kapsamayı hedefliyorsun. Kodu AI (Claude Code gibi bir araç) yazacak.

Önerilen yaklaşım tek cümlede: **hazır, izin sorunu olmayan açık kaynak kütüphaneleri (pdf.js, pdf-lib, tesseract.js) kendi arayüzünün altında birleştir; Tauri ile Windows'a paketle; "gerçek metin düzenleme" gibi zor kısımları gerçekçi bir sırayla, hangi kütüphanenin neyi yapamadığını bilerek planla.**

Bu doküman boyunca üç şeyi ayrı ayrı ele alıyorum: **mimari kararlar**, **hukuki/lisans gerçekleri** ve **yaygın hatalar**. Üçü de aynı derecede önemli — çoğu indie masaüstü uygulaması teknik olarak değil, bu üçünden birinde ihmal yaparak başarısız olur.

---

## 2. Karar günlüğü — bu noktaya nasıl geldik

Şeffaflık için: bu konuşma boyunca önerim bir kez değişti ve bunun nedenini burada açıkça yazıyorum.

1. **İlk aşamada** senden "hazır, çalışan bir sistem" istendiği için Stirling PDF'i (GitHub'da açık kaynak, Windows masaüstü sürümü olan, 50+ araçlı bir PDF paketi) önerdim.
2. **Sonra** amacının "kendi markamla kendi ürünümü kurmak" olduğu netleşince, Stirling PDF'in lisansını didik didik inceledim. Sonuç: çekirdek MIT (tamamen serbest) ama `engine/`, `app/proprietary/`, `frontend/editor/src/desktop` gibi klasörler ayrı ve kısıtlayıcı bir lisans altında — ve bu klasörler tam olarak senin istediğin "düzenleme" ve "masaüstü" özellikleriyle örtüşüyor. Bu yüzden "onların kodunu göm" tavsiyesinden "onların özellik listesini referans al, kodunu kendi başına yaz" tavsiyesine geçtim.
3. **Şimdi bu dokümanı hazırlarken** yaptığım ek araştırmada iki şey daha değişti:
   - `pdf-lib` kütüphanesinin, form alanları dışındaki **mevcut metni doğrudan düzenleyemediğini** doğruladım (bkz. Bölüm 6). Bu, "Acrobat gibi düzenleme" hedefini önemli ölçüde etkiliyor.
   - 2026 itibarıyla masaüstü uygulama çatısı tartışmasında (Electron vs Tauri) sektör literatürü büyük çoğunlukla **yeni projeler için Tauri'yi** öne çıkarıyor — daha küçük kurulum dosyası, daha az RAM, daha güçlü güvenlik modeli. Önceki mesajımda Electron önermiştim; bu dokümanda gerekçesiyle birlikte **Tauri'ye** çeviriyorum (bkz. Bölüm 3.1).

Bunu gizlemek yerine yazıyorum çünkü sen de ileride yeni bilgiyle kararını değiştirmen gerektiğinde bunun normal ve sağlıklı bir süreç olduğunu bilmelisin.

---

## 3. Mimari

### 3.1 Neden Tauri, Electron değil

| Kriter | Electron | Tauri v2 |
|---|---|---|
| Kurulum dosyası boyutu | ~120–200 MB (Chromium + Node.js gömülü) | ~3–10 MB (işletim sisteminin kendi WebView'ini kullanır) |
| Boşta RAM kullanımı | ~150–400 MB | ~40–80 MB |
| Arka uç dili | Node.js (JavaScript) | Rust |
| Güvenlik modeli | Node.js'in tam yetkisi; sızdırma riski manuel önlenir | İzin sistemi varsayılan olarak kısıtlı, Rust'ın bellek güvenliği |
| Paketleme/güncelleme olgunluğu | Çok olgun (electron-builder, electron-updater) | Olgun ama daha yeni (Tauri updater eklentisi) |
| Ekosistem / npm paket erişimi | Sınırsız | Sadece arayüz (frontend) tarafında sınırsız; arka uçta Rust ekosistemi |

**Karar:** Tauri. Gerekçe şu üçü:

1. **Güvenlik gerçekten önemli burada.** Bir PDF uygulaması, tanımadığın kaynaklardan gelen dosyaları ayrıştırıyor. PDF ayrıştırma tarihsel olarak güvenlik açıklarına sahne olmuş bir alan (Ghostscript, çeşitli PDF motorlarında geçmişte CVE'ler çıkmıştır). Tauri'nin varsayılan olarak kısıtlı izin modeli ve Rust'ın bellek güvenliği, "kötü niyetli bir PDF dosyası açıldığında ne olur" sorusuna karşı ek bir güvenlik katmanı sağlıyor.
2. **Küçük kurulum dosyası, kullanıcı güvenini artırır.** İlk izlenim önemli; "200 MB'lık tanımadığım bir .exe" ile "8 MB'lık hafif bir kurulum" arasındaki fark, özellikle SmartScreen uyarılarıyla birleşince (bkz. Bölüm 10) kullanıcı tarafında fark yaratıyor.
3. **Arayüz katmanın (pdf.js, pdf-lib, tesseract.js) zaten JavaScript/WASM tabanlı** — bunlar tarayıcı ortamında çalışan kütüphaneler, Tauri'nin WebView'i içinde Electron'daki kadar sorunsuz çalışırlar. Rust'ı sen yazmayacaksın; AI yazacak, ve dosya açma/kaydetme gibi temel işler için Tauri'nin hazır eklentileri (dialog, fs, shell) zaten var — sıfırdan Rust öğrenmene gerek yok.

**Ne zaman Electron'a geri dönmelisin:** Eğer AI destekli geliştirme sürecinde Rust tarafı ciddi bir tıkanıklık yaratırsa (örneğin bir üçüncü parti native kütüphaneyi Tauri'ye bağlamak zorlaşırsa), Electron'a geçmek makul bir yedek plandır — ekosistemi daha büyük, dokümantasyonu daha bol. Bu kararı ilk 2-3 haftalık denemeden sonra netleştir, projenin ortasında değil.

### 3.2 Katmanlar

Önceki mesajda gösterdiğim diyagramdaki üç katman hâlâ geçerli, sadece kabuk (shell) Electron yerine Tauri:

- **Arayüz katmanı** — React (veya Svelte/Vue, farketmez), tamamen senin marka/tasarımın. Tauri'nin WebView'i içinde çalışır.
- **PDF çekirdeği** — `pdf.js` (görüntüleme, metin katmanı, arama) + `pdf-lib` (oluşturma, birleştirme, form doldurma). İkisi de MIT/Apache 2.0, tamamen serbest.
- **Ek modüller** — `tesseract.js` (OCR), ileride dönüştürme motorları.

Rust tarafı (Tauri'nin "backend"i) ince bir katman olarak kalır: dosya sistemi erişimi, native menüler, pencere yönetimi. Ağır PDF işleme mantığı JavaScript/WASM tarafında kalmaya devam eder — bu, Rust bilmeden de projenin %90'ının ilerleyebileceği anlamına gelir.

---

## 4. Teknoloji seçimleri ve gerekçeleri

| İhtiyaç | Kütüphane | Lisans | Neden bu |
|---|---|---|---|
| PDF görüntüleme, metin katmanı, arama | `pdf.js` (Mozilla) | Apache 2.0 | Endüstri standardı; Firefox'un kendi PDF görüntüleyicisi bu. Çok olgun, çok test edilmiş. |
| Oluşturma, birleştirme, bölme, form doldurma, sayfa işlemleri | `pdf-lib` | MIT | En esnek, en izin verici JS PDF kütüphanesi. **Ama bkz. Bölüm 6 — önemli bir sınırı var.** |
| OCR (taranmış belgeyi aranabilir yapma) | `tesseract.js` | Apache 2.0 / MIT | Tesseract OCR motorunun WASM derlemesi; native binary kurulumu gerektirmez, tarayıcı/Tauri içinde çalışır. |
| Masaüstü kabuk | Tauri v2 | MIT / Apache 2.0 (dual) | Bkz. Bölüm 3.1 |
| Paketleme (Windows .msi/.exe) | Tauri bundler (dahili) | — | Tauri projesine gömülü, ayrı bir araca gerek yok |

**Kasıtlı olarak önermediklerim ve neden:**

- **MuPDF / PyMuPDF** — çok daha güçlü bir PDF motoru ama **AGPL-3.0** lisanslı. AGPL, uygulamanı dağıttığın anda kendi kaynak kodunu da açık kaynak yapmanı gerektirebilir (copyleft). Bunu istemiyorsan (ki muhtemelen istemiyorsun, kendi markanı koruyorsun) bu kütüphaneden ya tamamen uzak dur, ya da Artifex'in ticari lisansını satın al.
- **Ghostscript** — sıkıştırma/dönüştürme için popüler ama AGPL/ticari çift lisanslı; aynı risk.
- **Stirling PDF'in kodu** — Bölüm 2'de açıklandığı gibi, karışık lisans.

---

## 5. Lisans ve hukuki değerlendirme (özet)

*(Not: Ben avukat değilim, bu genel bilgi amaçlıdır; ürün gerçekten ticarileştiğinde bir uzmana teyit ettir.)*

- MIT ve Apache 2.0 lisanslı kütüphaneler: kullan, değiştir, kendi markanla dağıt — sorun yok. Tek şart genelde: yazılımının bir yerinde (örneğin "Açık kaynak lisansları" ekranı) orijinal telif/lisans metnini koru.
- AGPL'den uzak dur (yukarıda anlatıldı) — copyleft, seni kendi kodunu da açmaya zorlayabilir.
- **İsim/marka:** "Acrobat", "Adobe" gibi tescilli markalara benzemeyen bir isim seç. Türkiye'de isim/logo seçmeden önce TÜRKPATENT'in çevrimiçi marka sorgulama sisteminden hızlı bir kontrol yapman, ileride isim değiştirmek zorunda kalma riskini azaltır.
- **Domain adı** ve **GitHub organizasyon adı**nı da isim seçerken aynı anda kontrol et — ikisi de tükenebiliyor.

---

## 6. Kritik teknik gerçek: "Acrobat gibi düzenleme" ne kadar zor?

Bunu ayrı bir bölüm yapıyorum çünkü bu, projenin en çok yanlış anlaşılan kısmı.

**Adobe Acrobat'ın "Bir PDF'i Düzenle" özelliği** — bir paragrafa tıklayıp kelimeyi değiştirmen — aslında PDF formatının doğasına aykırı bir şey yapıyor. PDF, metni "içerik akışı" (content stream) denen düşük seviyeli çizim komutları olarak saklar; Word gibi düzenlenebilir bir belge yapısı değildir. Var olan bir kelimeyi değiştirmek için önce o content stream'i ayrıştırıp hangi komutun hangi kelimeye karşılık geldiğini bulman, sonra o komutu değiştirip yazı tipini/boşluğu bozmadan yeniden yazman gerekir. Bu yüzden bu özelliği düzgün yapan ticari SDK'lar (Nutrient/PSPDFKit, Apryse/PDFTron, Foxit SDK) yıllık binlerce dolara satılıyor — kolay bir problem değil.

**`pdf-lib` bunu yapamıyor.** Kütüphanenin kendi dokümantasyonu açıkça şunu söylüyor: form alanları içindeki metni değiştirebilir, ama **sayfa üzerindeki normal metni (form dışı) doğrudan düzenleme veya çıkarma API'si sunmuyor** — bunun zor bir özellik olduğunu ve gelecekte eklenebileceğini belirtiyorlar. `pdf.js` da bir görüntüleyici/render motoru, düzenleme motoru değil.

**Bu senin için ne anlama geliyor — üç seçenek:**

1. **Overlay (üzerine yazma) yaklaşımı — kısa vadede önerdiğim yol.** Eski metnin üzerine beyaz bir dikdörtgen çiz, üstüne yeni metni `pdf-lib` ile yaz. Basit düzeltmeler, tarih/isim değişiklikleri, form benzeri belgeler için gayet işe yarar ve gerçekçi bir MVP hedefidir. Sınırı: yazı tipi/boşluk otomatik eşleşmez, kullanıcı elle ayarlamak zorunda kalabilir.
2. **Kendi content-stream ayrıştırıcını yazmak — orta-uzun vadeli, gerçek "stretch goal".** Mühendislik açısından ciddi bir yatırım. AI ile bile haftalar/aylar sürebilir. Eğer uygulaman büyür ve gerçek gelir getirirse mantıklı bir yatırım olur.
3. **Ticari bir SDK'ya lisans almak (Nutrient, Apryse vb.) — ürün büyüyüp para kazanmaya başladığında.** "Ücretsiz" hedefinle şu an çelişir ama uygulamanın gelecekte bir "Pro" katmanı olursa, bu maliyeti o katman karşılayabilir.

**Önerim:** Yol haritanı buna göre kur (Bölüm 7). MVP'de "gerçek metin düzenleme" iddiasında bulunma; "form doldurma + not/vurgulama + overlay düzeltme" olarak konumlandır. Bu dürüst konumlandırma hem seni hem kullanıcılarını yanıltmaz.

---

## 7. Özellik yol haritası (fazlar)

Süre tahmini vermiyorum çünkü AI destekli geliştirmede hız kişiden kişiye, oturumdan oturuma çok değişir. Bunun yerine **sıra** ve **her fazın "bitti" tanımı** var — bu daha güvenilir bir plan iskeletidir.

**Faz 0 — İskelet**
- Proje adı, Tauri + React iskeleti kurulu, boş pencere açılıyor, native menü var.
- *Bitti tanımı:* Uygulama açılıyor, kapanıyor, çökmüyor.

**Faz 1 — Görüntüleyici (MVP çekirdeği)**
- `pdf.js` entegrasyonu: dosya aç (sürükle-bırak + menü), sayfa kaydırma, yakınlaştırma, metin arama, sağ tık > "[Uygulama Adın] ile Aç" Windows entegrasyonu.
- *Bitti tanımı:* Herhangi bir PDF'i Sumatra PDF kadar rahat görüntüleyebiliyorsun.

**Faz 2 — Temel dosya işlemleri**
- `pdf-lib` ile: birleştir, böl, sayfa sil/ekle/yeniden sırala/döndür, şifre ekle/kaldır, filigran ekle, metadata düzenle.
- *Bitti tanımı:* Stirling PDF'in temel araçlarının çoğunu kendi arayüzünde karşılıyorsun.

**Faz 3 — Form doldurma ve imza**
- AcroForm alanlarını algıla ve doldur, checkbox/dropdown desteği, düz imza (çizim veya yazı ile) alanı ekleme, formu "düzleştirme" (flatten).
- *Bitti tanımı:* Bir vergi formunu veya sözleşmeyi açıp doldurup kaydedebiliyorsun.

**Faz 4 — Not, vurgulama, overlay düzeltme**
- `pdf.js` annotation katmanıyla vurgulama, yapışkan not, serbest çizim; Bölüm 6'daki overlay yaklaşımıyla "metin düzeltme" (tam düzenleme değil, düzeltme).
- *Bitti tanımı:* Bir belgeyi işaretleyip küçük düzeltmeler yapabiliyorsun — Acrobat'ın "yorum yap" deneyimine yakın.

**Faz 5 — OCR ve temel dönüştürme**
- `tesseract.js` ile taranmış PDF'i aranabilir yapma; PDF ↔ görsel dönüşümü (`pdf-lib` görsel çıkarma/oluşturma zaten destekliyor).
- *Bitti tanımı:* Taranmış bir belgede metin arayabiliyorsun.

**Faz 6 — Paketleme ve dağıtım**
- Tauri bundler ile .msi/.exe, ikon, sürüm bilgisi; otomatik güncelleme; code signing (bkz. Bölüm 10).
- *Bitti tanımı:* Bir arkadaşın senin gönderdiğin linkten indirip kurabiliyor, SmartScreen'de takılmıyor (ya da beklenen ilk-sürüm uyarısını anlıyor).

**Faz 7 — Büyüme (bkz. Bölüm 14)**
- Gerçek içerik düzenleme motoru, bulut senkronizasyonu, işbirliği, mobil companion, gelişmiş dönüştürme, opsiyonel Pro katman.

---

## 8. Yaygın hatalar / tuzaklar

Bunlar, benzer indie masaüstü projelerinde en sık görülen, projeyi geciktiren veya baltalayan hatalar:

1. **"Her şeyi Faz 1'de yapmaya çalışmak."** Acrobat'ın 25 yıllık özellik setini ilk sürümde hedeflemek, hiçbir şeyi bitirememekle sonuçlanır. Fazları sırayla bitir.
2. **Content-editing'i erken vaat etmek.** Bölüm 6'yı oku — "tam metin düzenleme" pazarlama diline erken girerse, teslim edemediğinde güven kaybedersin.
3. **Code signing'i son ana bırakmak.** Bölüm 10'daki gerçekleri (SmartScreen artık EV'de bile anında geçiş vermiyor) bilmeden yayınlarsan, ilk kullanıcıların "virüs" uyarısıyla karşılaşıp uygulamayı hiç açmadan siler.
4. **Büyük dosyalarda performansı test etmemek.** 5 sayfalık bir PDF ile geliştirip 500 sayfalık bir PDF'te asla test etmemek — özellikle `pdf.js` render performansı ve bellek kullanımı büyük dosyalarda ayrı bir konudur. Sanal kaydırma (virtualized rendering) düşün.
5. **Bozuk/şifreli/olmayan-standart PDF'leri test etmemek.** Gerçek dünyadaki PDF'lerin önemli bir kısmı bozuk üstbilgiye, eksik font gömmeye veya beklenmeyen yapıya sahiptir. Kütüphane bir istisna fırlattığında uygulaman çökmemeli, kullanıcıya anlamlı bir hata göstermeli.
6. **Güvenilmeyen dosya girişini güvenli işlememek.** Bir kullanıcı sana/uygulamana kötü niyetli hazırlanmış bir PDF verirse ne olur? Ayrıştırma işini mümkünse izole bir süreçte/worker'da yap, bağımlılıkları güncel tut (CVE takibi).
7. **Lisans ihlalini fark etmeden yapmak.** İlerde "şu özelliği hızlıca eklemek için şu GitHub reposundan kopyala-yapıştır yaparım" dürtüsüne kapılırsan, önce lisansına bak. Bölüm 5'teki kuralı bir alışkanlık haline getir.
8. **Marka/isim çakışmasını sona bırakmak.** Aylarca geliştirip yayınlamadan hemen önce isminin tescilli olduğunu öğrenmek, yeniden markalama (rebranding) zorunluluğu doğurur. Bölüm 5'teki kontrolü Faz 0'da yap.
9. **Tek dilli düşünmek.** Türkçe ile başlasan bile, arayüz metinlerini baştan bir çeviri dosyasında (i18n) tutmak, ileride İngilizce/başka dile açılmayı ucuzlaştırır.
10. **Telemetri/analitik eklerken kullanıcıyı bilgilendirmemek.** İleride kullanım istatistiği veya çökme raporu toplamak istersen, KVKK (Türkiye) kapsamında kullanıcıyı açıkça bilgilendirmen ve mümkünse opt-in yapman gerekir — bunu en baştan "aydınlatma metni" gerektiren bir özellik olarak planla, sonradan yama olarak değil.

---

## 9. Güvenlik notları (kısa)

- Tauri'nin izin sistemini varsayılan kısıtlı bırak; her yeteneği (dosya sistemi, shell erişimi) sadece gerçekten ihtiyaç duyduğun kadar aç.
- PDF ayrıştırmayı ana arayüz sürecinden mümkün olduğunca izole et (worker/ayrı süreç) — bozuk bir dosya tüm uygulamayı çökertmesin.
- Otomatik güncelleme mekanizmasını (Faz 6) mutlaka imzalı/doğrulanabilir yap — imzasız bir güncelleme kanalı, saldırganın kullanıcılarına kötü amaçlı yazılım dağıtmasının en kolay yollarından biridir.
- Bağımlılıklarını (npm paketleri) düzenli güncel tut; `pdf.js` ve benzeri kütüphanelerde geçmişte güvenlik yamaları çıkmıştır.

---

## 10. Dağıtım ve Windows code signing gerçeği (2026 durumu)

Bu konuda güncel ve önemli bir gelişme var, muhakkak bil:

- **EV sertifikaları artık SmartScreen'i anında atlatmıyor.** Mart 2024'ten beri Microsoft, EV ve OV (standart) sertifikaları SmartScreen itibarı açısından eşitledi. Yani pahalı bir EV sertifikası (400$+/yıl) alman, ucuz bir OV sertifikasına göre anlık bir avantaj sağlamıyor — ikisi de indirilme sayısına göre zamanla "itibar" kazanıyor.
- **En pratik ve ucuz seçenek: Microsoft'un Azure Artifact Signing hizmeti (~9.99$/ay).** Geleneksel OV sertifikalardan (150-300$/yıl) çok daha uygun, bireysel geliştiriciler için ABD ve Kanada'da; kurumlar için ABD, Kanada, AB ve İngiltere'de mevcut. Türkiye'den bireysel başvuru şu an bu kapsamda görünmüyor — başvuru öncesi güncel uygunluk listesini kontrol et.
- **En sorunsuz yol: Microsoft Store üzerinden MSIX olarak yayınlamak.** Store, paketini kendi imzalıyor, SmartScreen uyarısı çıkmıyor, ek sertifika maliyeti yok. Yeni uygulamalar için önerilen birincil yol bu.
- **İmzasız veya kendinden imzalı sertifikayla genel kullanıma açmak** — güçlü bir SmartScreen engeliyle karşılaşırsın, önerilmez.
- **Gerçekçi beklenti:** Hangi yolu seçersen seç, ilk sürümlerde bazı kullanıcılar "Windows bilgisayarınızı korudu" uyarısı görecek. Bu normal; tutarlı bir yayıncı adıyla, her sürümü imzalayarak ve zaman damgası kullanarak zamanla itibar birikir.

**Öneri:** Faz 6'da önce Microsoft Store/MSIX yolunu dene (ücretsiz, en az sürtünme); paralelde GitHub Releases üzerinden Azure Artifact Signing ile imzalanmış bir .exe/.msi de sun (Store'a girmek istemeyen/kurumsal kullanıcılar için).

---

## 11. Kalite ve test stratejisi

- **Çeşitli PDF üreticileriyle test et:** Word'den kaydedilen, tarayıcıdan kaydedilen, telefon kamerasıyla taranmış, farklı Adobe sürümleriyle üretilmiş, LaTeX çıktısı gibi farklı kaynaklardan gelen PDF'ler birbirinden çok farklı iç yapıya sahip olabiliyor.
- **Kenar durumları:** şifreli PDF, formu olmayan PDF, bozuk/kesik dosya, çok büyük dosya (500+ sayfa), Türkçe karakter içeren metinler ve fontlar (özellikle font gömme/embedding testleri kritik).
- **Otomatik test:** En azından temel işlemler (aç, birleştir, böl, kaydet) için birim testleri; her yeni özellik eklerken küçük bir örnek PDF seti üzerinde regresyon testi.
- **Gerçek kullanıcı testi erken başlasın.** "Küçük bir çevreyle paylaşırım" dediğin aşamayı Faz 2 sonunda başlat, Faz 6'yı bekleme — erken geri bildirim en pahalı hataları önler.

---

## 12. Marka ve küçük hukuki notlar

- İsim seçerken: kısa, Türkçe klavyede kolay yazılan, `.com`/`.app` domain'i müsait, TÜRKPATENT'te çakışma yok, GitHub'da organizasyon adı müsait — bunların hepsini aynı gün kontrol et.
- Açık kaynak bildirimleri sayfası (About/Hakkında ekranında) kullandığın her kütüphanenin lisansını listele — bu hem MIT/Apache şartlarını yerine getirir hem de profesyonel bir izlenim bırakır.
- Uygulama ileride kişisel veri (e-posta, kullanım istatistiği vb.) toplarsa, KVKK kapsamında bir gizlilik politikası ve aydınlatma metni gerekir. Bunu en baştan basit bir taslak olarak hazırlamak, sonradan aceleye getirmekten iyidir.

---

## 13. Rakip haritası (kısa özet)

Daha önceki araştırmamdan, konumlandırma için özet:

| Rakip | Model | Senin için anlamı |
|---|---|---|
| Adobe Acrobat | Ücretli, çok kapsamlı | Hedef referans noktası, ama fiyat/karmaşıklık senin boşluğun |
| Stirling PDF | Açık kaynak (karışık lisans), self-host | Özellik listesi için referans, kod için değil |
| PDF24 Creator | Ücretsiz ama kapalı kaynak | Basitlik/hız kıyaslaması |
| PDF-XChange Editor | Ücretsiz katman + ücretli üst özellikler | "Freemium" modelinin nasıl işlediğine örnek |
| Smallpdf / iLovePDF | Tarayıcı tabanlı, dosya sunucuya yükleniyor | Senin "yerel/gizlilik dostu" avantajın burada öne çıkar |

Senin farkın: **yerel işleme (dosya hiçbir yere yüklenmiyor) + tamamen kendi markan + açık, izin sorunu olmayan bir temel.** Bunu pazarlama diline erken koy.

---

## 14. Gelecek potansiyeli / büyüme fikirleri

Bunlar şu an yapılacaklar listesi değil, ama mimarinin bunları **engellememesi** için baştan bilinmesi gereken yönler:

- **Gerçek içerik düzenleme motoru** (Bölüm 6, Faz 7) — büyüme sağlarsa en büyük yatırım kalemi.
- **PDF'le sohbet / AI destekli özetleme, arama** — Claude API'sini kendi uygulamana entegre edip "bu belgeyi özetle", "şu maddeyi bul" gibi özellikler eklemek, günümüzde farklılaşma noktası olabilir.
- **Bulut senkronizasyon / cihazlar arası erişim** — opsiyonel, gizlilik önceliğini bozmayacak şekilde (yerel-öncelikli + isteğe bağlı senkron).
- **Mobil companion (Android/iOS)** — Tauri v2 mobil hedefleri de destekliyor, ileride aynı kod tabanından mobil sürüm çıkarma ihtimalini açık tutar.
- **Tarayıcı uzantısı** — web'de karşılaşılan PDF'leri tek tıkla uygulamana açma.
- **Kurumsal/Pro katman** — imza doğrulama, toplu işleme, SSO gibi özellikler için opsiyonel ücretli katman; "ücretsiz tabanlı" hedefinle çelişmez, temel özellikler ücretsiz kalır.
- **Erişilebilirlik (PDF/UA)** — ekran okuyucu uyumlu PDF üretimi/kontrolü, kurumsal/kamu müşterileri için ileride ayırt edici olabilir.

---

## 15. Şimdi ne yapmalısın (ilk somut adım)

1. İsmini seç ve Bölüm 12'deki kontrolleri yap.
2. Claude Code'u aç, Faz 0'ı (Bölüm 7) başlat: "Tauri v2 + React + TypeScript ile [isim] adlı bir Windows masaüstü PDF uygulaması iskeleti kur."
3. Faz 0 bittiğinde buraya dön, Faz 1'e geç.

Bu doküman yaşayan bir belge olarak düşünülmeli — her fazın sonunda "beklenenle gerçekleşen neydi" notlarını buraya ekleyip güncelleyebilirsin.

---

## Kaynaklar

- pdf-lib — https://github.com/Hopding/pdf-lib
- pdf.js (Mozilla) — https://github.com/mozilla/pdf.js
- tesseract.js — https://github.com/naptha/tesseract.js
- Tauri v2 — https://tauri.app
- Stirling PDF (özellik referansı) — https://github.com/Stirling-Tools/Stirling-PDF
- Microsoft: Windows uygulamaları için code signing seçenekleri — https://learn.microsoft.com/en-us/windows/apps/package-and-deploy/code-signing-options
