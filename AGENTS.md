# AGENTS.md — Proje çalışma sözleşmesi

Bu dosya insan ve yapay zekâ katkılarının kısa, kanonik çalışma sözleşmesidir. Ürün gerçeğini buraya yığma; ilgili uzman belgeye bağlantı ver.

## Her işe başlarken

1. `STATUS.md` dosyasını oku.
2. `STATUS.md` birincil aktif görev gösteriyorsa o görev dosyasını ve içindeki `Devam checkpoint'i` bölümünü oku.
3. Değişiklik ürün kapsamını etkiliyorsa `PROJECT.md` ve `MASTER_PLAN.md` dosyalarını oku.
4. İlgili ADR, gereksinim, kaynak kaydı ve playbook'u oku.
5. `git status`, ilgili diff ve mevcutsa son commit'leri kontrol et; kullanıcıya ait ilgisiz değişiklikleri koru.
6. Kabul kriteri ve doğrulama yöntemi belli değilse kod yazmadan önce görev kaydında tanımla.

## “Plana devam et” protokolü

Kullanıcı `devam`, `plana devam et` veya `kaldığın yerden devam et` dediğinde sohbet geçmişine güvenme ve kullanıcıdan planı yeniden anlatmasını isteme.

1. `STATUS.md` → birincil aktif görev → görevde listelenen zorunlu belgeler sırasını izle.
2. Görev checkpoint'ini çalışma ağacı ve gerçek kanıtlarla karşılaştır. İşaretlenmemiş bir adımı yapılmış varsayma; yalnız sohbet iddiasını tamamlanmış sayma.
3. İşlemden önce kullanıcıya tek kısa özet ver: mevcut faz, son doğrulanmış adım, sıradaki tek eylem ve varsa blokaj.
4. İnsan kararı gerektiren gerçek bir blokaj yoksa ilk tamamlanmamış ve önkoşulları sağlanmış adımdan çalışmaya devam et.
5. Her doğrulanmış anlamlı birimden sonra aktif görevdeki checkpoint'i güncelle. Görev bitince `done` yap, `ROADMAP.md` sırasına göre sonraki görev kaydını aç ve `STATUS.md` içindeki birincil aktif görev bağlantısını değiştir.
6. Yarım veya başarısız işi tamamlanmış gösterme. Çalışma ağacı ile checkpoint çelişirse önce çelişkiyi kaydet ve güvenli biçimde uzlaştır.

Checkpoint karar/sonuç/kanıt içerir; sohbet dökümü, gizli chain-of-thought, secret veya kullanıcı PDF içeriği içermez.

## Gerçek ve kanıt

- İddiaları `DOĞRULANDI`, `YERELDE KANITLANDI`, `PROJE İDDİASI`, `ÖNERİ` veya `BİLİNMİYOR` olarak ayır.
- Sürüm, lisans, güvenlik, destek ve standart iddiası için birincil URL + kontrol tarihi gerekir.
- Arama özeti, yıldız sayısı, README rozeti veya başka bir yapay zekâ cevabı tek başına kanıt değildir.
- “Testler geçti” yalnızca gerçekten çalıştırılan komut, exit code ve özet kaydedildiyse yazılabilir.
- API varlığı özellik uyumluluğu; validator geçişi tam uygunluk; render başarısı kayıpsız round-trip anlamına gelmez.

## Değişiklik protokolü

- Küçük ve tek amaçlı değişiklik yap; görev dışı refactor ekleme.
- Mimari, lisans, belge serileştirme/revision, sandbox, kripto, updater, gizlilik veya conformance değişikliği için önce ADR gerekir.
- Dış bağımlılık/fork/kod/model/font/binary eklerken `.agents/skills/pdf-dependency-audit/SKILL.md` sürecini uygula.
- Ürün ağacına/history'sine upstream kod alınmadan veya dependency yapılmadan önce `ADR-0001` ve `ADR-0002` kabul edilmiş olmalıdır. Gate 1 için ürün deposu dışında/geçici audit worktree'sinde exact source clone edip build/test etmek import sayılmaz; oradan ürün kodu kopyalanmaz.
- Testi kapatmak, güvenlik limitini gevşetmek veya golden baseline'ı incelemeden yenilemek çözüm değildir.
- Kullanıcı belgesi, parola, özel anahtar, çıkarılmış metin veya PDF baytlarını log/telemetri/görev kaydına yazma.

## PDF'ye özel kırmızı çizgiler

- Orijinal dosyaya doğrudan yazma: temp dosya → flush → yeniden aç/validate → gerekirse render/semantic kontrol → atomik replace; başarısızlıkta orijinal kalır.
- İmzalı belge varsayılan salt okunur açılır; değişikliğin hangi revision/imzayı etkileyeceği açıkça gösterilir.
- Görsel imza kriptografik imza değildir. Siyah dikdörtgen redaction değildir.
- PDF JavaScript, Launch action, dış dosya/UNC, XFA, rich media ve 3D varsayılan kapalıdır.
- Parser/render/OCR çökmesi ana UI sürecini düşürmemelidir.
- Aynı işlemde iki farklı motor aynı belgeyi mutasyona uğratmaz. Motor devrinde save → close → reopen → validate uygulanır.
- Bilinmeyen nesneler, tag tree, form appearance, annotation, attachment, metadata ve imza revision'ları korunamıyorsa sessizce kaydetme.

## Bitirirken

1. İlgili testleri ve mümkünse save/reopen doğrulamasını çalıştır.
2. Gerçek komut/sonuç ve değişen yolları görev veya mühendislik kaydına yaz.
3. Çok oturumlu işte aktif görevin `Devam checkpoint'i` bölümünü son doğrulanmış durum ve sıradaki tek eylemle güncelle.
4. Davranış değiştiyse gereksinim/destek matrisi ve yalnız kullanıcıya görünen değişiklikte `CHANGELOG.md` güncellenir.
5. Karar değiştiyse eski ADR'yi düzenlemek yerine yeni ADR ile supersede et.
6. `STATUS.md` dosyasını yalnız mevcut faz, birincil aktif görev veya üst düzey sonraki eylem gerçekten değiştiyse güncelle.

## Komutlar

Teknik taban seçilmediği için kanonik build/test komutları henüz yoktur; komut uydurma. Gate 1 sonunda doğrulanmış komutlar bu bölüme ve `CONTRIBUTING.md` dosyasına yazılacaktır.

## Kapsamlı belgeler

- AI ve hafıza: `docs/AI_WORKFLOW.md`
- Mimari: `docs/ARCHITECTURE.md`
- Kalite: `docs/quality/QUALITY_AND_TESTING.md`
- Uygunluk iddiaları: `docs/standards/CONFORMANCE_POLICY.md`
- Güvenlik: `docs/SECURITY_AND_PRIVACY.md`
- Kaynaklar: `SOURCES.md`
- Lisans: `docs/LICENSING.md`
