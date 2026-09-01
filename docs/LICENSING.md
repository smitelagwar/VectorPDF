# Lisans ve Marka Politikası

**Durum:** Active — ADR-0001 Accepted  
**Son doğrulama:** 2026-09-01  
**Not:** Bu belge teknik lisans risk yönetimidir; hukuk görüşü değildir. Dağıtımdan önce yetkin hukuk incelemesi gerekir.

## Temel ayrım

- **Ücretsiz:** Kullanıcıdan fiyat alınmaması.
- **Açık kaynak:** Kaynağın belirli bir OSI lisansı altında verilmesi.
- **Freeware:** Ücretsiz fakat kapalı olabilir.
- **Copyleft:** Türevin veya ilgili kapsamın dağıtımında kaynak/lisans yükümlülüğü doğurabilir.
- **Trademark:** Kod lisansı, upstream ad/logo kullanım hakkını otomatik vermez.

Kendi ürün adımızı koymak mümkündür ancak attribution, copyright, notice, kaynak sunma, relink ve trademark kuralları ayrıca uygulanır.

## Kabul edilen ürün ve depo lisansı

`ADR-0001` ile **permissive baseline + root MIT** kabul edilmiştir. Resmî masaüstü çekirdeği bu depoda MIT olarak açık tutulur ve son kullanıcıya ücretsiz dağıtılır. Bu karar gelecekte ücretli destek/entegrasyon/OEM olasılığını korur; mevcut MIT kodun copyright/notice yükümlülüklerini kaldırmaz.

Haricî katkılar açıldığında inbound lisans MIT ve provenance beyanı DCO 1.1'dir; başlangıçta CLA yoktur. M0'daki DCO/review hattına kadar katkı kanalı kapalıdır. GPLv3/KillerPDF rotası mevcut ürün için reddedilmiştir; ancak gelecekte ADR-0001'i supersede eden açık insan kararıyla yeniden ele alınabilir.

## Lisans sınıfları ve proje politikası

| Sınıf | Örnek | Varsayılan politika |
|---|---|---|
| MIT/BSD | PDF4QT, bazı yardımcılar | Kabul adayı; copyright/permission/notice paketlenir |
| Apache-2.0 | qpdf, PDF.js, Tesseract | Kabul adayı; lisans/NOTICE, değişiklik ve patent koşulları incelenir |
| MPL-2.0 | PoDoFo'nun seçilebilir yolu, pikepdf | Dar bileşen adayı; covered file değişikliklerinin kaynak yükümlülüğü ve binary dağıtımı incelenir |
| LGPL | Qt, bazı kütüphaneler | Koşullu; dinamik link/relink, lisans metni, source/offer, modül bazlı lisans ve anti-tivoization etkisi uygulanır |
| GPL | KillerPDF, Okular/PDFsam vb. | Yalnız ürün GPL uyumluysa veya bağımsız kullanım için hukukça onaylıysa |
| AGPL | MuPDF, bazı server projeleri | Permissive rota için reject; ticari lisans veya bütün ilgili ürünün AGPL uyumu gerekir |
| Source-available/proprietary | Stirling özel dizinleri, Apryse şartları | Açık yetki/sözleşme yoksa reject |
| Bilinmeyen/no license | Rastgele repo/gist/model/font | Kod/asset/binary kopyalama yasak |

“Ayrı process” lisans yükümlülüğünü otomatik olarak ortadan kaldıran sihirli bir sınır değildir. IPC, birlikte dağıtım, tight coupling ve ürün sunumu hukuk incelemesi gerektirir.

## PDF4QT özel kapıları

1. `v1.6.0.0` exact tag/commit lisansı ve bütün dosya başlıkları taranır.
2. Proje 27 Nisan 2025'te MIT'ye relicensed olduğunu beyan eder; bu tarihten önce üçüncü taraf katkıların hak/provenance durumu Git history ile çıkarılır.
3. Qt'nin kullanılan her modülü Community LGPL altında gerçekten uygun mu kontrol edilir; GPL-only Qt modülü yanlışlıkla eklenmez.
4. Qt dinamik linklenir; kullanıcı relink/debug hakkı engellenmez; ilgili lisans ve kaynak erişimi/teklifi sağlanır. Resmî [Qt LGPL yükümlülükleri](https://www.qt.io/licensing/open-source-lgpl-obligations) release checklist'e bağlanır.
5. OpenSSL, FreeType, OpenJPEG, libjpeg, zlib, LittleCMS, Blend2D ve transitif paketlerin exact lisans/notice'ları toplanır.
6. Upstream isim/logo/icon/domain/update URL'si kullanılmaz; yalnız zorunlu attribution korunur.
7. Binary, installer ve About ekranında doğru notice erişimi doğrulanır.

## GPL rota özel kapıları

- Dağıtılan türevin complete corresponding source'u, build/install scripts ve gereken interface definition'ları aynı sürüm için sağlanır.
- Kullanıcıların lisans metnine ve kaynak teklifine erişimi kolaydır.
- Tüm ek dependency lisanslarının GPLv3 ile uyumu kanıtlanır.
- Kendi marka ve varlıklarımız kullanılır; upstream attribution silinmez.
- App Store/installer/update koşullarının kullanıcıya GPL ile verilen hakları sınırlamadığı hukukça kontrol edilir.
- Gelecekte dual-license istenirse yalnız tüm gerekli copyright sahiplerinden ayrıca hak alınabileceği varsayılır; bu garanti değildir.

## Kod dışı varlıklar da bağımlılıktır

Şunlar aynı intake sürecine tabidir:

- OCR model dosyaları
- fontlar ve font subset'leri
- ikon/logo/illustration
- test PDF'leri ve corpus'lar
- ICC color profile'ları
- codec/DLL/runtime
- çeviri metinleri
- veri setleri ve AI modelleri

Repo lisansı bu varlıkları kapsamıyor olabilir. Her artifact için kaynak, exact version/hash, lisans ve redistribution hakkı gerekir.

## Dependency kabul kaydı

Her girişte:

- kaynak ID ve canonical URL
- exact tag/commit/artifact SHA-256
- doğrudan/transitif ve runtime/build/dev/data/model rolü
- SPDX lisansı ve exact evidence URL/path
- license exception/dual-license seçimi
- notice/source/relink/patent/trademark yükümlülükleri
- güvenlik/advisory kanalı ve bakım sahibi
- kabul ADR/inceleyen/tarih/sonraki inceleme
- alternatif ve kaldırma maliyeti

bulunur. `.agents/skills/pdf-dependency-audit/SKILL.md` uygulanmadan dependency eklenmez.

## Yasak pratikler

- GitHub'dan dosya/algoritma kopyalayıp kaynağı yorumda bırakmakla yetinmek
- Root LICENSE'a bakıp nested/vendor/asset lisanslarını atlamak
- `npm`, NuGet, vcpkg, Cargo veya binary paketinin transitif lisansını yok saymak
- “Sadece birkaç satır” veya “ücretsiz proje” diye copyleft/proprietary kod almak
- Lisans uyumsuz test PDF'sini halka açık repoya commit etmek
- Upstream logoyu rename ederek kullanmak
- Eski permissive tag fork edip güvenlik düzeltmelerini provenance olmadan current özel lisanslı dosyalardan cherry-pick etmek

## Release hukuk çıktıları

- Ürün lisansı
- `third_party/THIRD_PARTY_NOTICES.md`
- Bütün gereken license text'leri
- SBOM (native DLL/runtime/model/font dahil)
- Source offer/relink materials gerekiyorsa bunlar
- Trademark/attribution ve About ekranı kontrolü
- Artifact–source commit eşleşmesi

Gate 0 tamamlanmıştır; buna rağmen Gate 1 lisans/provenance scan'i temizlenmeden fork merge edilmez ve ürün dağıtımı yapılmaz.
