# Yapay Zekâ Çalışma, Hafıza ve Kanıt Sistemi

**Durum:** Active  
**Son güncelleme:** 2026-09-01

## Amaç

Codex, Claude, Gemini veya gelecekteki başka modeller değişse bile proje bilgisinin sohbete/model hafızasına bağımlı kalmaması; her değişikliğin amacı, kaynağı, testi ve geri dönüşünün Git içinde denetlenebilir olması.

## Neden tek `MASTER.md` veya `memory.md` yok?

Dev bir dosya kısa sürede:

- aynı gerçeği birden çok yerde tekrarlar;
- güncel durumla tarihsel kararı karıştırır;
- modele gereksiz bağlam yükler;
- çelişkide hangi cümlenin geçerli olduğunu belirsizleştirir;
- her küçük değişiklikte merge çatışması yaratır.

`MASTER_PLAN.md` strateji haritasıdır; her konunun gerçeği kendi kanonik dosyasındadır. Sohbet özeti yararlı olabilir ama kaynak değildir.

## Kalıcı hafıza katmanları

| Hafıza | Sorduğu soru | Yaşam döngüsü |
|---|---|---|
| `PROJECT.md` | Ne ve kim için yapıyoruz? | Seyrek değişir; ürün kararı ister |
| `AGENTS.md` | Bu depoda nasıl çalışılır? | Kısa tutulur; süreç değişince |
| `STATUS.md` | Şimdi neredeyiz, sıradaki üç iş ne? | Her gerçek faz değişiminde |
| `MASTER_PLAN.md` / `ROADMAP.md` | Strateji ve sıralama nedir? | Gate/strateji değişince |
| Requirements/support matrix | Tam olarak ne vaat ediliyor? | Feature kanıtıyla |
| `SOURCES.md` / registry | Dış gerçek nereden geliyor? | Kaynak doğrulama/upgrade'de |
| ADR | Neden bu önemli karar verildi? | Accepted kayıt değişmez; supersede edilir |
| Risk register | Neyi kaybedebiliriz? | Risk review'da |
| Task record | Bu sınırlı işte ne yapılacak/yapıldı? | İş boyunca, sonra arşiv |
| Engineering log | Hangi önemli çalışma/kanıt üretildi? | Append-only kayıt |
| Git | Tam olarak ne değişti? | Commit geçmişi |
| `CHANGELOG.md` | Kullanıcı hangi değişikliği görecek? | Release davranışı değişince |

Aynı bilgi iki yerde tutulmaz; diğer belge kanonik kaynağa link verir.

## Model adapter'ları

- Codex, kök ve kapsama yakın `AGENTS.md` dosyalarını okur.
- Claude için `CLAUDE.md`, yalnız `@AGENTS.md` import eder.
- Gemini için `GEMINI.md`, yalnız `@./AGENTS.md` import eder.
- Başka sağlayıcı için eklenen dosya ürün gerçeğini kopyalamaz; kök sözleşmeye ve ilgili dokümana yönlendirir.
- Alt modülde özel kural gerekirse o klasöre küçük bir `AGENTS.md` konur; kök kuralları tekrarlanmaz.

Bu düzen, model adı değişse bile kanonik hafızayı sabit tutar.

Kullanıcının kopyalayacağı ilk prompt, Gemini doğrulaması ve başka modele geçiş yönergesi `docs/AI_START_HERE.md` içindedir. Adapter dosyaları bu metni veya ürün gerçeğini çoğaltmaz.

## Oturumlar arası devam protokolü

`STATUS.md` her zaman tek bir **birincil aktif task** bağlantısı taşır. Ayrıntılı kaldığım-yer kaydı task dosyasının `Devam checkpoint'i` bölümündedir. Git çalışma ağacı ve test/kanıt kayıtları bu checkpoint'i doğrular; üçü çelişirse sohbet değil dosya ve kanıt incelenir.

`devam`, `plana devam et` veya eşdeğer komutta ajan:

1. `AGENTS.md` → `STATUS.md` → birincil aktif task → task'ın zorunlu okuma listesi zincirini izler.
2. Checkpoint'i `git status`, diff, son commit ve çalışma kaydıyla uzlaştırır.
3. Mevcut fazı, son doğrulanmış adımı, sıradaki tek eylemi ve blokajı kısaca bildirir.
4. İnsan kararı yoksa ilk açık ve önkoşulu sağlanmış adımdan çalışır.
5. Her doğrulanmış anlamlı birimden sonra checkpoint'i; görev devrinde hem yeni task'ı hem `STATUS.md` bağlantısını günceller.

Bu yapı kesintisiz sohbet taklidi yapmaz. Yeniden başlatılabilir mühendislik durumu üretir. Chain-of-thought veya sohbet özeti saklanmaz; yalnız karar, değişiklik, kanıt, blokaj ve sonraki eylem saklanır.

## Proje içi gerçek önceliği

Çelişkide tahmin etme. İlgili kapsam için:

1. Accepted ve daha yeni ADR
2. `PROJECT.md` içindeki kabul edilmiş ürün ilkesi
3. Accepted gereksinim ve support-matrix kanıtı
4. `STATUS.md` içindeki güncel state
5. Resmî/normatif source-registry kaydı
6. `MASTER_PLAN.md` / `ROADMAP.md` önerisi
7. `referanslar/`, issue, forum, sohbet veya model cevabı

İki üst düzey kaynak çelişirse işi durdur, çelişkiyi `DECISION_QUEUE`/ADR'ye yaz ve gerekli insan kararını iste.

## Evidence sözleşmesi

Her önemli cümle zihinsel olarak şu sınıflardan biridir:

- `DOĞRULANDI`: Birincil kaynağın exact revision'ında kontrol edildi.
- `YERELDE KANITLANDI`: Komut/ortam/exit/result mevcut.
- `PROJE İDDİASI`: Upstream söylüyor; bizim fixture kanıtımız yok.
- `ÖNERİ`: Henüz kabul edilmemiş yön.
- `BİLİNMİYOR`: Eksik bilgi; tahmin yapılmaz.

### Kaynak önceliği

1. ISO/ETSI/Microsoft gibi normatif veya resmî kaynak
2. Maintainer canonical repo, LICENSE, release, API docs
3. Birincil araştırma/makale
4. Issue/PR — yalnız bildirimin/uygulama tartışmasının kanıtı
5. Forum/blog/arama özeti — araştırma ipucu

Sürüm, lisans, security support ve “güncel” iddiası doğrulama tarihi taşır. Arama sonucu URL'si kaynak olarak kaydedilmez; canonical sayfa açılır.

## Her AI görevinin başı

1. `AGENTS.md` ve `STATUS.md` oku.
2. `STATUS.md` içindeki birincil aktif task'ı ve checkpoint'ini oku.
3. İlgili kanonik belgeleri belirle; bütün repo belgelerini körlemesine yükleme.
4. `git status` ve ilgili diff'i kontrol et; başkasının değişikliğini sahiplenme/silme.
5. İş birden çok adıma/oturuma yayılıyorsa, protected zone'a dokunuyorsa, source audit/ADR içeriyorsa veya devralınacaksa `docs/templates/TASK.md` ile `docs/tasks/TASK-YYYY-NNN-<slug>.md` kaydı aç; küçük tek-oturum işi için mevcut issue/istekte aynı kabul kriterini açıkça tut.
6. Baseline testini çalıştırabiliyorsan çalıştır; çalıştıramıyorsan açıkça kaydet.
7. Dış gerçeğe dayanan değişiklikte source freshness kontrol et.
8. Protected zone ise gerekli human approval/ADR'yi doğrula.

## Çalışma sırasında

- Küçük değişiklik ve kısa feedback loop.
- Her varsayım açık; bilinmeyen API/sürüm/path uydurulmaz.
- Dependency ekleme ve kaynak kod alma için `pdf-dependency-audit` skill'i.
- Testi önce veya değişiklikle birlikte yaz; PDF mutation için save/reopen doğrulaması.
- Drive-by refactor ve yeni dependency yok.
- Üçüncü taraf kodun lisans başlığını/notice'ını silme.
- Kullanıcı verisini log/prompt/corpus'a koyma.
- Komut çıktısını başarı gibi yorumlamadan exit code ve ilgili test sayısını kaydet.

## Görev sonu

1. Gerçekten çalıştırılan testleri komut + exit + kısa sonuçla kaydet.
2. Çalıştırılamayan testi ve nedeni açıkça yaz.
3. Değişen dosyaları ve davranışı özetle.
4. Çok oturumlu işte task checkpoint'ini son doğrulanmış adım, sıradaki tek eylem, blokaj ve çalışma ağacı bilgisiyle güncelle.
5. Requirement/support/ADR/source/risk belgelerinden gerçekten etkilenenleri güncelle.
6. `CHANGELOG.md` yalnız kullanıcı davranışı değiştiyse.
7. Yeni risk, TODO ve rollback planını bırak.
8. `STATUS.md` yalnız mevcut faz/birincil görev/üst düzey sonraki adım değiştiyse.
9. Hassas veya büyük tool output'unu commit etme; sanitized özet + artifact hash kullan.

## Protected zones

Aşağıdaki alanlarda AI tek başına son karar veremez; Accepted ADR + insan onayı + ilgili review gerekir:

- ürün lisansı, dependency lisansı ve provenance
- parser sandbox/worker privileges ve security quotas
- serialization, incremental update, revision ve save pipeline
- kriptografik imza/trust, encryption ve key handling
- redaction/sanitize
- updater, release signing ve root keys
- telemetry/privacy ve dış AI provider'a belge gönderme
- migration/storage format
- PDF/A, PDF/UA, PAdES veya “conformant” ürün iddiası

AI şunları yapamaz:

- CI'ı yeşil yapmak için testi skip/delete etmek;
- güvenlik limitini gerekçesiz yükseltmek/kapatmak;
- golden baseline'ı kendi çıktısıyla otomatik onaylamak;
- lisans uyumsuzluğunu “ayrı process” diyerek kabul etmek;
- release key/secret istemek, görüntülemek veya değiştirmek;
- imzalı belgeyi uyarısız yazmak;
- test fixture olmadan support matrisini `supported` yapmak.

## ADR gerektiren değişiklikler

- baseline/UI/motor/runtime değişimi
- yeni persistent format veya document transaction modeli
- dependency lisans sınıfı/paketleme biçimi
- güvenlik sınırı veya network capability
- desteklenen Windows/standart profili
- public API/plugin/update architecture
- privacy/telemetry/AI data flow

Accepted ADR düzenlenmez; yeni ADR `supersedes` alanıyla değiştirir.

## Görev dosyası kuralı

Yalnız anlamlı ve birden çok adıma/oturuma yayılan işler task kaydı alır. Chat transcript kopyalanmaz.

Kalıcı task kayıtları `docs/tasks/` altında tutulur; şablonun kendisi düzenlenmez. Task kaydı:

- goal / non-goals
- requirement/ADR/source IDs
- assumptions ve açık sorular
- acceptance/evidence plan
- risk ve rollback
- changed paths
- gerçek komut/exit/result
- sonuç ve remaining work

Günlük “ne düşündüm” yerine karar ve kanıt yazılır.

## AI eval sistemi

Agent/prompt/model/tool güncellemesinde sabit görev seti kullanılır:

### Normal görevler

- küçük annotation bug fix
- form appearance regression
- atomic save fault fix
- dependency minor upgrade
- source doğrulama ve ADR yazma

### Adversarial görevler

- uydurma PDF API'si kullanma tuzağı
- lisanssız GitHub snippet'i kopyalama isteği
- testi kapatarak düzeltme
- imzalı belgeyi uyarısız değiştirme
- kullanıcı PDF'sini log/corpus'a ekleme
- sandbox/network limitini gevşetme
- fake “PDF/A compliant” etiketi ekleme

### Ölçüler

- fonksiyonel başarı ve regression
- talimat uyumu
- desteksiz iddia oranı
- kaynak doğruluğu/tazeliği
- değişiklik kapsamı hassasiyeti
- gerçekten test çalıştırma oranı
- security/license boundary ihlali
- insan review sonucu

Kaydedilen koşullar: model/tool sürümü, AGENTS commit'i, network/sandbox izinleri, task fixture sürümü ve seed. Farklı koşullar “model daha iyi” kıyası için doğrudan karşılaştırılmaz.

## Skill politikası

Skill ürün bilgisinin deposu değil, tekrar eden bir sürecin icra kılavuzudur. İyi skill:

- dar bir trigger description'a sahiptir;
- adım adım çıktı sözleşmesi verir;
- kanonik docs'u referanslar, kopyalamaz;
- gerekliyse deterministik script kullanır;
- gereksiz README/changelog üretmez.

Başlangıç skill'i: `.agents/skills/pdf-dependency-audit/SKILL.md`.

## Hallüsinasyon kontrol listesi

- Exact version/tag/commit gördüm mü?
- LICENSE'ın doğru revision ve doğru alt dizinini okudum mu?
- “Destekliyor” dediğim şey upstream iddiası mı, lokal test mi?
- Normatif standardı blogla karıştırdım mı?
- Tarihe duyarlı bilgiye tarih koydum mu?
- Bir validator sonucunu tam uygunluk diye büyüttüm mü?
- Build output/exit code gerçekten elimde mi?
- Yapmadığım testi “geçti” diye yazdım mı?
- Karşıt kanıtı/known issue'yu kaydettim mi?
- Bilinmeyeni açıkça bırakmak yerine tahmin mi ettim?

Bu sorulardan biri olumsuzsa iddia düşürülür veya görev araştırma olarak kalır.
