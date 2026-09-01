# PDF Standartları ve Uygunluk İddiası Politikası

**Durum:** Active policy  
**Son doğrulama:** 2026-08-31

## Temel kural

Bir PDF'nin uygulamada açılması, görünmesi veya validator'dan geçmesi tek başına kayıpsız edit, profile uygun üretim ya da erişilebilirlik garantisi değildir. Her iddia exact standard/profile/version, feature scope ve test kanıtı taşır.

## PDF 2.0

Normatif taban [ISO 32000-2:2020](https://www.iso.org/standard/75839.html) ve yayımlanmış [errata/issue akışı](https://pdf-issues.pdfa.org/32000-2-2020/)'dır. Eski Adobe PDF Reference tarihsel/uygulama notu olabilir; normatif kaynağın yerine geçmez.

Tek “PDF 2.0 destekli” bayrağı yoktur. `PDF_SUPPORT_MATRIX` şu boyutları ayrı izler:

- parse/open
- render
- create
- edit
- save/preserve
- validate
- exclusions ve known loss

Bilinmeyen fakat geçerli nesneler mümkünse korunur. Korunamıyorsa silent drop yerine işlem engeli veya raporlu yeni kopya gerekir.

## PDF/A

İlk araştırma hedefi [ISO 19005-4:2020 / PDF/A-4](https://www.iso.org/standard/71832.html)'dür. Profile seçimi M8 ADR'sinde yapılır:

- PDF/A-4: temel
- PDF/A-4f: embedded file gereken iş akışı
- PDF/A-4e: engineering/3D özel ihtiyaç; varsayılan değil

PDF/A-4'te eski `a/u` conformance level'ları kullanılmaz. Erişilebilirlik ayrı PDF/UA/WTPDF hattıdır. Encryption PDF/A ile bağdaşmaz.

PDF/A export:

1. Kaynaktan ayrı artifact üretir.
2. Hedef profile ve yapılacak kayıpları önce gösterir.
3. Font/color/metadata/attachment/encryption/action politikası uygular.
4. Pinned [veraPDF](https://docs.verapdf.org/validation/) ile doğrular.
5. Visual + semantic fark ve dönüşüm raporu üretir.

veraPDF pass'i tek başına görüntü/meaning preservation veya hukuki arşiv süreci sertifikası değildir.

## PDF/UA ve tagged PDF

PDF/UA-2, PDF 2.0 ile eşleşir; UA-1 kullanımını otomatik olarak ortadan kaldırmaz. Ürün her zaman profile/version söyler ve iki profilin kanıt paketini karıştırmaz.

Ortak incelenen yapı: structure tree, reading order, document language, ActualText/alt text, headings/lists/tables, form labels, bookmarks, Unicode ve gerçek assistive-technology davranışı.

### PDF/UA-1 kanıt paketi

- [ISO 14289-1](https://www.iso.org/standard/64599.html)
- [PDF/UA-1 reference suite](https://pdfa.org/resource/pdfua-reference-suite/)
- [Matterhorn Protocol 1.1](https://pdfa.org/resource/the-matterhorn-protocol/) machine ve insan maddeleri
- Profile uygun validator + Narrator/NVDA/keyboard testi

### PDF/UA-2 kanıt paketi

- [ISO 14289-2:2024](https://www.iso.org/standard/82278.html) ve profile özgü güncel errata/resmî açıklamalar
- Pinned veraPDF veya başka UA-2 validator profile'ı; yalnız makineyle test edebildiği maddeler kapsamında
- ISO 14289-2'ye göre ayrı insan checklist'i ve Narrator/NVDA/keyboard testi

PDF/UA-1 reference suite ve Matterhorn 1.1 sonucu **PDF/UA-2 uygunluk kanıtı değildir**. Resmî/profile özgü UA-2 reference suite oluşursa source registry'ye ayrı version olarak eklenir.

Edit tag yapısını bozacaksa uyarı/engel; sessiz tag silme yok. Uygulama UI erişilebilirliği ile üretilen PDF'nin erişilebilirliği ayrı hatlardır; ayrıntı [../quality/ACCESSIBILITY.md](../quality/ACCESSIBILITY.md).

## PAdES ve dijital imza

Yayımlanmış başlangıç kaynağı [ETSI EN 319 142-1 V1.2.1](https://www.etsi.org/deliver/etsi_en/319100_319199/31914201/01.02.01_60/en_31914201v010201p.pdf) ve doğrulama prosedürü [ETSI TS 119 102-1 V1.4.1](https://www.etsi.org/deliver/etsi_ts/119100_119199/11910201/01.04.01_60/ts_11910201v010401p.pdf)'dir. Taslak/yeni çalışma yayımlanmadan ürün iddiası olmaz.

PAdES B/T/LT/LTA farklı capability'lerdir. Revision/incremental-save modeli kanıtlanmadan imzalama başlamaz.

Doğrulama sonucu ayrı alanlardır:

- signed byte integrity
- imzadan sonraki revision/değişiklik
- CMS cryptographic verification
- certificate chain
- trust policy/root
- revocation (OCSP/CRL)
- timestamp
- claimed qualification/profile

Offline veya trust bilgisi eksikken genel “Geçerli” etiketi yoktur. AB nitelikli/eIDAS gibi hukukî iddia ayrı trust-list, policy ve hukuk incelemesi ister.

## Şifreleme

Yeni üretimde hedef AES-256 Revision 6'dır; interoperability kanıtlanmadan AES-GCM varsayılan olmaz. Zayıf eski algoritmalar yalnız gerektiğinde okunur, yeni yazılmaz. Permission flags güçlü DRM değildir.

PDF/A+encryption, signed+encrypted, attachment ve redaction çatışmaları `PDF_SUPPORT_MATRIX` policy tablosunda tutulur.

## İddia onay kapısı

Bir ürün/website/release note cümlesi `supported` veya `conformant` demeden önce:

- exact profile/version ve exclusions
- normative source ID
- requirement ve accepted ADR
- fixture/corpus provenance
- test runner/version/result artifact
- independent validator/reader sonucu
- visual/semantic/human review gereken alanlar
- last verified date ve engine commit

bulunur. İddia bu kapsamdan daha geniş yazılamaz.

## Taslak ve gelecek standartlar

Draft/revision çalışmaları source registry'de `watch` olabilir. Kod deneysel feature flag arkasında PoC yapabilir; yayımlanmış standard veya conformance diye gösterilemez. Standart/errata her release planning başında yeniden kontrol edilir.
