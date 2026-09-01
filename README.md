# VectorPDF

Bu depo, Windows için ücretsiz, çevrimdışı çalışmayı önceleyen ve zamanla Acrobat sınıfı iş akışlarına yaklaşması hedeflenen bir PDF uygulamasının **ürün, mühendislik ve yapay zekâ çalışma sistemi**dir.

Henüz ürün kodu içe aktarılmadı. Bu bilinçli bir güvenlik kapısıdır: marka, dağıtım lisansı ve teknik taban kararı verilmeden bir projeyi fork etmek sonraki bütün kararları gereksiz yere kilitleyebilir.

## Şu anki hüküm

- Kabul edilen permissive/MIT rotanın tek birincil fork adayı **PDF4QT v1.6.0.0**'dır. Windows üzerinde çalışan viewer/editor/page manager/diff/CLI bileşenleri olan en yakın hazır tabandır. Kabulü otomatik değildir; `Gate 1` testlerinden geçmelidir.
- **KillerPDF v1.8.2**, GPLv3 olduğu için mevcut ürün rotasında baseline değildir; yalnız davranış/test referansı olabilir. Yeni ve açık bir lisans ADR'si olmadan kodu alınmaz.
- Sıfırdan kabuk yazmak varsayılan rota değildir. PDF4QT elenirse permissive yedek rota **PDFium + qpdf + Tesseract/Leptonica** bileşimidir.
- Open PDF Studio ve Stirling-PDF güncel halleri lisans/provenance sorunları nedeniyle ürün tabanı değildir. MuPDF, AGPL uyumu veya ticari lisans olmadan kullanılamaz.

Kararın ayrıntısı [MASTER_PLAN.md](MASTER_PLAN.md), uygulanma sırası [ROADMAP.md](ROADMAP.md), bugünkü durum [STATUS.md](STATUS.md) içindedir.

## Yapay zekâ ile başlat veya devam et

İlk kez Gemini/Codex/Claude ya da başka bir yerel ajanla başlayacaksan [AI ile Başlatma ve Devam Ettirme](docs/AI_START_HERE.md) dosyasındaki ilk prompt'u kullan. Sonraki yeni sohbetlerde çalışma alanına erişimi olan ajan için `Plana devam et.` yeterlidir: kök `AGENTS.md`, `STATUS.md` içindeki birincil aktif göreve ve onun checkpoint'ine yönlendirir.

Şu anki birincil kayıt her zaman [STATUS.md](STATUS.md) içinden bulunur. Sohbet geçmişi proje hafızası değildir; task checkpoint'i + Git diff/commit + gerçek test kayıtlarıdır.

## Okuma sırası

1. [PROJECT.md](PROJECT.md) — ürünün değişmez niyeti ve sınırları
2. [MASTER_PLAN.md](MASTER_PLAN.md) — nihai strateji ve karar ağacı
3. [ROADMAP.md](ROADMAP.md) — aşamalar, çıktılar ve geçiş kapıları
4. [STATUS.md](STATUS.md) — yalnızca güncel faz, açık kararlar ve sonraki işler
5. [SOURCES.md](SOURCES.md) — doğrulanmış GitHub/standart kaynakları ve kanıt düzeyi
6. [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md) — hedef teknik sınırlar
7. [docs/AI_WORKFLOW.md](docs/AI_WORKFLOW.md) — yapay zekâ hafızası ve değişiklik protokolü
8. [docs/AI_START_HERE.md](docs/AI_START_HERE.md) — ilk prompt ve yeni sohbetten devam etme
9. [docs/GATE1_BAKEOFF.md](docs/GATE1_BAKEOFF.md) — ilk repo kararını çalıştırma kılavuzu

## Kanonik belge haritası

| Konu | Tek gerçek kaynağı |
|---|---|
| Vizyon, kullanıcı, kapsam dışı | `PROJECT.md` |
| Güncel çalışma durumu | `STATUS.md` |
| Uzun vadeli plan | `MASTER_PLAN.md` |
| Aşama ve gate'ler | `ROADMAP.md` |
| Mimari sınırlar | `docs/ARCHITECTURE.md` |
| Ürün gereksinimleri | `docs/requirements/REQUIREMENTS.md` |
| PDF özellik iddiaları | `docs/standards/PDF_SUPPORT_MATRIX.md` + `docs/standards/CONFORMANCE_POLICY.md` |
| Kaynak ve repo gerçekleri | `SOURCES.md` + `docs/sources/SOURCE_REGISTRY.yml` + `docs/sources/evaluations/` |
| Mimari kararların gerekçesi | `docs/adr/` |
| Riskler | `docs/risks/RISK_REGISTER.md` |
| Kullanıcıya görünen sürüm değişiklikleri | `CHANGELOG.md` |
| İç mühendislik izi | `docs/ENGINEERING_LOG.md` |
| Birincil aktif iş ve kaldığım yer | `STATUS.md` → bağlı `docs/tasks/TASK-...md` checkpoint'i |

## Kanıt etiketleri

- **DOĞRULANDI:** Birincil kaynakta, belirtilen tarih/sürümde kontrol edildi.
- **YERELDE KANITLANDI:** Komut, ortam ve sonuç kaydedildi.
- **PROJE İDDİASI:** Upstream'in söylediği fakat bizim henüz bağımsız doğrulamadığımız özellik.
- **ÖNERİ:** Henüz kabul edilmiş karar değil.
- **BİLİNMİYOR:** Tahmin edilmez; araştırma veya PoC gerekir.

## Değişmez ilkeler

- Kullanıcı belgesinin orijinali üzerine doğrudan yazılmaz.
- Her PDF güvensiz girdidir; parse/render/OCR/kriptografi uzun vadede düşük yetkili worker sınırında çalışır.
- “Metin ekleme” ile “mevcut metni yeniden akıtma”, görsel imza ile kriptografik imza, siyah kutu ile gerçek redaction aynı özellik değildir.
- Bir API'nin bulunması destek kanıtı değildir. Destek, fixture + test + kaydet/aç/karşılaştır kanıtıyla ilan edilir.
- Dış kod, model, font veya binary exact sürüm/commit, lisans kapsamı, notice ve SBOM kaydı olmadan içe alınmaz.

Referans olarak verilen beş eski rapor `referanslar/` altında korunur; onlar karar kaynağı değil, araştırma girdisidir.
