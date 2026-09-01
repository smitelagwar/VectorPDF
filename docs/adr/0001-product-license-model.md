# ADR-0001: Ürün lisansı ve gelecekteki esneklik

- **Durum:** Accepted
- **Tarih:** 2026-09-01
- **Karar sahibi:** Product owner
- **İlgili riskler:** R-001, R-009, R-020
- **Supersedes:** Yok

## Bağlam

Ürün kullanıcıya ücretsiz sunulacak, kendi adıyla dağıtılacak ve ileride Acrobat benzeri geniş bir özellik kümesine ulaşacak. “Ücretsiz” sözcüğü, türev kodun açık veya kapalı olacağını belirlemez. Baseline seçimi GPLv3 ile tamamen açık bir ürün veya permissive tabanla daha esnek gelecek arasında geri dönüşü pahalı bir karardır.

## Kısıtlar

- Başlangıç maliyeti mümkün olduğunca düşük olmalı.
- Hazır çalışan sistem tercih ediliyor.
- Ürün adı ve marka bize ait olmalı; upstream telif/lisans/trademark hakları korunmalı.
- Güvenlik, bakım ve code-signing ücretsiz değildir.
- Gelecekte ücretli destek/kurumsal modül/OEM ihtimali bugün kesin bilinmiyor.

## Seçenek A — permissive esnek taban

PDF4QT MIT veya PDFium/qpdf/Tesseract gibi permissive bileşenlerle ilerle.

**Artıları**

- Ürün kendi kodunu açık tutabilir; ayrıca gelecekte kapalı modül/OEM seçeneği korunur.
- White-label ve dağıtım politikası daha yönetilebilir.
- GPL/AGPL birleşik türev belirsizliği azalır.

**Eksileri**

- PDF4QT'nin Qt LGPL ve native dependency yükümlülükleri yine vardır.
- PDF4QT elenirse daha çok orkestrasyon/UI işi gerekir.
- Permissive lisans güvenlik/kalite garantisi değildir.

## Seçenek B — GPLv3 topluluk ürünü

KillerPDF gibi GPLv3 baseline kullan; dağıtılan türevin karşılık gelen kaynağını GPLv3 ile yayımla.

**Artıları**

- Hazır .NET/WPF ürün/engine üzerinde hızlı başlangıç olabilir.
- Topluluk katkısı ve kaynak şeffaflığı ürün kimliğinin parçası olur.

**Eksileri**

- Kapalı türev/OEM modül esnekliği önemli ölçüde azalır.
- Bütün dağıtım ve dependency uyumluluğu GPLv3 açısından yönetilmelidir.
- Çok genç baseline riski lisans kararıyla çözülmez.

## Karar

**Seçenek A — permissive esneklik kabul edildi.** Product owner 2026-09-01 tarihinde başlangıç rotasının belirlenmesini proje planlayıcısına bıraktı; mevcut kanıt ve hedefler doğrultusunda aşağıdaki paket bağlayıcı başlangıç kararıdır:

- Bu deponun kendi kod ve belgeleri root `LICENSE` ile **MIT** olarak yayımlanır.
- Resmî masaüstü çekirdeğinin kaynak kodu bu depoda MIT olarak açık tutulur ve son kullanıcı uygulaması ücretsiz dağıtılır.
- Üçüncü taraf kod/asset/model kendi lisansını korur; root MIT onları yeniden lisanslamaz.
- Haricî katkılar açıldığında inbound lisans outbound ile aynıdır: **MIT + Developer Certificate of Origin 1.1 sign-off**. Başlangıçta CLA yoktur.
- DCO/otomatik provenance kontrolü ve review sahipliği M0'da hazır olana kadar haricî patch/asset/corpus kabulü kapalıdır; yalnız issue/tartışma alınabilir.
- Gelecekte ücretli destek, entegrasyon, OEM veya ayrı kurumsal hizmet mümkündür. Ayrı kapalı modül ancak yeni ADR, dependency sınırı ve katkı hakları incelemesiyle oluşturulabilir; mevcut MIT katkılarının notice'ları silinmez.
- Birincil Gate 1 adayı PDF4QT v1.6.0.0'dır. GPL/AGPL baseline veya kod mevcut ürün rotasına alınmaz.

## Sonuçlar

- AGPL motorlar (MuPDF dahil) ticari lisans veya ayrı GPL/AGPL ürün kararı olmadan kullanılamaz.
- GPL projeler davranış/UX referansı olabilir; mevcut ürün rotasında kod kopyalanmaz veya ürünle birleştirilmez.
- Qt LGPL yükümlülükleri ve PDF4QT historical MIT relicense provenance Gate 1'de incelenir.
- Root MIT, upstream/nested license metinlerinin ve copyright notice'larının yerine geçmez.
- DCO bir telif devri veya CLA değildir; katkıcının gönderme hakkına dair kaydı güçlendirir.

## Doğrulama

- Product owner'ın yetki devri ve seçilen rota kayıt altına alınır.
- Kök `LICENSE`, `DCO` ve inbound policy birbiriyle tutarlıdır.
- `docs/LICENSING.md`, `STATUS.md` ve `ADR-0002` güncellenir.
- Seçilen baseline license scan ve dağıtım PoC'u Gate 1'den geçer.

## Yeniden değerlendirme tetikleyicileri

- Ticari/OEM/enterprise gereksinimi netleşir.
- Baseline lisansı veya dependency şartı değişir.
- Ürün bir ağ servisi veya plugin marketplace olur.
- Hukuk incelemesi önerinin uygulanamaz olduğunu gösterir.

## Kaynaklar

- [GNU GPL FAQ](https://www.gnu.org/licenses/gpl-faq.en.html)
- [Qt LGPL obligations](https://www.qt.io/licensing/open-source-lgpl-obligations)
- [PDF4QT v1.6.0.0 exact LICENSE](https://github.com/JakubMelka/PDF4QT/blob/23f3829ae0420da801824a0d53d82c53baa2bee9/LICENSE)
- [KillerPDF v1.8.2 exact LICENSE](https://github.com/SteveTheKiller/KillerPDF/blob/fca9a6f4bd77d1fdde692bd9e402c07e0ec490de/LICENSE)
- [Developer Certificate of Origin 1.1](https://developercertificate.org/)

## Depo katkı sonucu

Baseline lisansı ile bizim yazdığımız kodun outbound lisansı aynı karar değildir. Bu depo için root MIT ve inbound MIT+DCO 1.1 seçilmiştir. M0'a kadar haricî katkı kapalı kalır; açıldıktan sonra her commit `Signed-off-by` taşımalı ve DCO kontrolü geçmelidir.

MIT izni geniştir fakat telif/provenance kaydını ortadan kaldırmaz. Başkalarının kod/belge/asset katkısını gönderme hakkına sahip olduğu varsayılmaz. Referans: [GitHub — Licensing a repository](https://docs.github.com/en/repositories/managing-your-repositorys-settings-and-features/customizing-your-repository/licensing-a-repository).
