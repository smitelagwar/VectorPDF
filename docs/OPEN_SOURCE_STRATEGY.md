# Açık Kaynak ve Upstream Stratejisi

**Durum:** Active — ADR-0001 permissive rota  
**Son güncelleme:** 2026-09-01

## Amaç

Hazır çalışan açık kaynak avantajını almak; rastgele repo kodlarını birbirine karıştırarak lisans, davranış ve bakım felaketi üretmemek.

## Dört kullanım biçimi

1. **Baseline fork:** Ürünün omurgası. Tek aday Gate 1 sonrası PDF4QT'dir. KillerPDF mevcut permissive rotada baseline değildir.
2. **Dar bileşen:** İyi tanımlı adapter/worker arkasındaki PDFium, qpdf, Tesseract gibi dependency.
3. **Davranış referansı:** SumatraPDF/Okular/Sioyek/PDFsam/Xournal++/Stirling gibi UX incelenir; kod kopyalanmaz.
4. **Araştırma/watch:** Lisans/provenance/olgunluk sorunu düzelene kadar Open PDF Studio gibi projeler izlenir.

Bir projenin fork edilebilir olması, akıllıca baseline olduğu anlamına gelmez.

## Baseline fork prosedürü

1. Exact release tag ve dereference edilmiş commit kaydedilir.
2. `upstream` remote salt okunur mantıkla korunur; `origin` kendi depomuzdur.
3. `baseline/<upstream-version>` tag'i imzalanır.
4. Değişikliksiz build/test/golden sonuçları saklanır.
5. Marka ve paket kimliği ayrı commit'te temizlenir.
6. Architecture boundary/refactor yeni feature'dan önce gelir.
7. Upstream sync periyodik dev merge değil, ayrı riskli dependency upgrade olarak ele alınır.

Her upstream sync:

- release notes + diff + license/path change taraması
- yeni dependency/asset/model scan'i
- security advisory/CVE incelemesi
- full corpus, fuzz smoke, render/semantic diff
- installer/SBOM/notices yeniden üretimi
- rollback tag'i

gerektirir.

## Patch bütçesi

Forkun uzun ömürlü olması için:

- mümkün olan genel bug/security fix upstream'e küçük PR olarak gönderilir;
- marka, ürün politikası ve özel workflow adapter'larda tutulur;
- upstream core dosyalarında büyük, dağıtık patch birikimi ölçülür;
- her release'te `upstream distance`, çatışma sayısı, özel patch sayısı ve merge süresi raporlanır;
- upstream güvenlik düzeltmesi alınamaz hale gelirse replacement/rebase ADR tetiklenir.

## Bileşen sınırları

- Her motor tek purpose ve capability interface arkasında olmalıdır.
- Bir dosyanın ownership'i transaction sırasında tek writer'dadır.
- CLI/sidecar çağrısı timeout, exit code, version probe, resource limit ve sanitized stderr sözleşmesine sahiptir.
- Binary/system dependency sessizce `PATH` üzerinden bulunmaz; paketli exact artifact veya açık kullanıcı seçimi gerekir.
- Upstream default network/telemetry/action davranışı ürün policy'sini geçemez.

## Kopyalamadan öğrenme

Reference projeler incelenirken kaydedilebilecekler:

- görev akışı ve ekran davranışı
- kullanıcı sorunu ve edge case listesi
- test senaryosu ve beklenen çıktı, lisansı izin veriyorsa fixture provenance
- public standard/API davranışı
- bağımsız tasarım kararı

Doğrudan kod, ikon, çeviri, screenshot, örnek dosya veya özgün metin alınmaz. Benzer davranış bağımsız uygulanır ve kaynak “tasarım referansı” olarak sicile yazılır.

## Proje bazlı kararlar

### PDF4QT

Gate 1'i geçerse fork. Öncelik sırası: provenance → build/test → corpus → worker spike → rebrand → refactor. Upstream'in kendi motor iddiaları `PROJE İDDİASI`dır; destek matrisine fixture olmadan taşınmaz.

### KillerPDF

GPLv3 olduğu için `ADR-0001` ile seçilen mevcut permissive rotada baseline değildir; yalnız davranış/test taksonomisi referansıdır ve kod alınmaz. Yerelde geçen 1.716 test olumlu sinyaldir fakat conformance/security kanıtı değildir. Ancak ADR-0001 ileride açıkça supersede edilirse yeniden aday olabilir.

### Open PDF Studio

Multi-process PDFium ve render-regression tasarımı referanstır. Kök LGPL/manifest MIT/doğrudan AGPL MuPDF ilişkisinin upstream tarafından açıklanması, provenance temizliği ve olgunluk süresi olmadan kod alınmaz.

### Stirling-PDF

Current main'in engine ve desktop özel lisans yolları baseline'a dahil edilmez. Araç kataloğu ve workflow UX'i referanstır. Eski MIT tag'den fork varsayılan değildir; güvenlik backport ve current-code provenance maliyeti çok yüksektir.

### SumatraPDF / Okular / Sioyek / PDFsam / PDF Arranger / Xournal++

Reader, accessibility, navigation, batch, thumbnail, ink ve recovery UX referansları. Lisans ve mimari nedeniyle kod harmanlama yapılmaz.

## Kabul/red kuralları

Bir dependency şu hallerden biri varsa bloke edilir:

- lisans yok, kapsam belirsiz veya nested lisans çelişkili;
- exact source/artifact eşleşmiyor;
- varsayılan ağ/credential/document erişimi kapatılamıyor;
- güvenlik bildirimi/upstream sahipliği yok ve kritik yüzey yüksek;
- clean build tekrarlanamıyor;
- supported platform artifact'i yeniden üretilemiyor;
- aynı işi mevcut kabul edilmiş dependency yeterince yapıyor;
- removal/replacement için interface sınırı kurulamıyor.

Popülerlik veya commit sıklığı tek başına kabul nedeni değildir.

## Kaynak güncelliği

`SOURCE_REGISTRY.yml` içindeki zaman duyarlı girdiler:

- aktif baseline/engine: en geç 30 günde veya upgrade öncesi
- standart/errata: release planning başında
- license/terms: her dependency update ve her dağıtım öncesi
- reference projeler: yalnız yeniden değerlendirilirken

kontrol edilir. Kaynak değişirse eski gerçek silinmez; `superseded` ve tarih ile iz bırakır.
