# AI ile Başlatma ve Devam Ettirme

Bu dosya kullanıcıya dönük başlatıcıdır; ürün gerçeğinin yeni bir kopyası değildir. Kanonik durum `STATUS.md`, ayrıntılı kaldığım-yer kaydı onun bağladığı aktif task, çalışma kuralları `AGENTS.md` ve gerçek değişiklik geçmişi Git'tir.

## Önkoşul

AI aracı yerel çalışma alanındaki dosyaları okuyup değiştirebilmeli ve çalışma dizini depo kökü olmalıdır:

```text
C:\Users\hsyn\Desktop\pdf_prototip
```

Salt web sohbeti yerel dosyalara erişemiyorsa yalnız `plana devam et` cümlesiyle kaldığı yeri bulamaz. Böyle bir ortamda bu depo bağlanmalı veya en az `AGENTS.md`, `STATUS.md` ve `STATUS.md` içindeki aktif task dosyası sohbete verilmelidir.

## Gemini 3.7 Flash (High) başlangıç kontrolü

Gemini CLI/yerel ajan depo kökünde açılır. Model elle seçilebiliyorsa `gemini-3.7-flash`, düşünme seviyesi sunuluyorsa `High` seçilir. İlk mesajdan önce veya hemen sonra:

1. `/memory show` ile `GEMINI.md` üzerinden kök `AGENTS.md` içeriğinin yüklendiğini doğrula.
2. Görünmüyorsa `/memory reload` çalıştır ve tekrar kontrol et.
3. İlk durum keşfi için plan/read-only veya her işlemde onay modu kullan; YOLO/koşulsuz otomatik onay kullanma.

`High`, daha fazla akıl yürütme bütçesidir; lisans kararı, kaynak kanıtı, test sonucu veya insan onayı değildir.

**DOĞRULANDI — 2026-09-01:** Google'ın resmî model sayfası stable model kodunu `gemini-3.7-flash`, desteklenen thinking seviyelerini `low`, `medium`, `high` olarak listeler. Gemini CLI'nin resmî bağlam belgesi kök `GEMINI.md`, `@file.md` importu, `/memory show` ve `/memory reload` davranışlarını; ayarlar belgesi plan/read-only ve onay modlarını açıklar.

- [Gemini 3.7 Flash — resmî model sayfası](https://ai.google.dev/gemini-api/docs/models/gemini-3.7-flash)
- [Gemini thinking levels — resmî kılavuz](https://ai.google.dev/gemini-api/docs/thinking)
- [Gemini CLI `GEMINI.md` context — resmî repo belgesi](https://github.com/google-gemini/gemini-cli/blob/main/docs/cli/gemini-md.md)
- [Gemini CLI settings/approval modes — resmî repo belgesi](https://github.com/google-gemini/gemini-cli/blob/main/docs/cli/settings.md)

## İlk prompt — kopyala ve yapıştır

```text
C:\Users\hsyn\Desktop\pdf_prototip deposunda çalışıyorsun. Bu proje uzun süreli ve birden fazla yapay zekâ tarafından devralınabilir; sohbet hafızası kanıt veya proje hafızası değildir.

Önce kök GEMINI.md ve AGENTS.md talimatlarını, ardından STATUS.md dosyasını tamamen oku. STATUS.md içindeki “Birincil aktif görev” bağlantısını aç; o task dosyasının “Devam checkpoint'i”, kabul kriterleri ve doğrulama planını tamamen oku. Task içinde “Devam etmeden önce oku” olarak listelenen belgeleri ve yalnız bu işe gerekli ilgili ADR/source/playbook dosyalarını oku. Sonra git status, ilgili diff ve varsa son commitleri kontrol et.

AGENTS.md içindeki “Plana devam et” protokolüne uy. İşleme başlamadan önce bana yalnız şu dört bilgiyi kısa biçimde bildir:
1) mevcut faz,
2) son doğrulanmış adım,
3) sıradaki tek eylem,
4) varsa gereken insan kararı veya gerçek blokaj.

Checkpoint ile çalışma ağacı çelişirse tahmin etme; önce çelişkiyi açıkla ve güvenli biçimde uzlaştır. İşaretlenmemiş işi yapılmış sayma. Başka bir AI cevabını veya README iddiasını kanıt kabul etme. Güncel sürüm, lisans, güvenlik, destek ve standart iddialarını canonical birincil kaynaktan tarih ve exact revision ile doğrula.

Gerçek insan kararı gerektiren blokaj yoksa aktif task'taki ilk tamamlanmamış ve önkoşulları sağlanmış adımdan çalışmaya başla. Küçük, doğrulanabilir birimler halinde ilerle. Her anlamlı ve doğrulanmış birimden sonra task dosyasındaki “Devam checkpoint'i” bölümünü güncelle; gerçek komut/exit/sonucu çalışma kaydına yaz. Görev bitince onu done yap, ROADMAP.md sırasına göre sonraki task'ı aç ve STATUS.md içindeki birincil aktif görev bağlantısını aynı değişiklikte güncelle.

Mevcut fazı yalnız STATUS.md ve onun bağladığı aktif task'tan belirle. Accepted ADR'ler bağlayıcıdır; yeni ve açık insan kararı olmadan değiştirme veya yeniden tartışmaya açma. Özellikle ADR-0001 permissive/MIT rotasını, ADR-0003 Windows/dağıtım sınırını ve ADR-0004 ücretsiz çekirdek/gizlilik politikasını koru. ADR-0002 Gate 1 kanıtıyla kabul edilmeden upstream ürün kodunu bu deponun ağacına veya geçmişine alma. Dış kaynak değerlendirmesi gerekirse .agents/skills/pdf-dependency-audit/SKILL.md sürecini eksiksiz uygula. Test çalıştırmadıysan “geçti” deme; bilinmeyeni BİLİNMİYOR bırak.

Şimdi dosyalardan durumu keşfet ve yukarıdaki protokole göre devam et. Benden planı yeniden anlatmamı isteme.
```

## Sonraki sohbetlerde

Yerel ajan kök talimatlarını otomatik yüklüyorsa yeni sohbette şu tek cümle yeterlidir:

```text
Plana devam et.
```

Bağlam yüklenip yüklenmediğinden emin değilsen daha dayanıklı kısa komut:

```text
Plana devam et. Önce AGENTS.md → STATUS.md → STATUS.md'nin gösterdiği birincil aktif task → task içindeki “Devam etmeden önce oku” zincirini izle; git durumuyla checkpoint'i doğrula ve ilk açık, önkoşulu sağlanmış adımdan ilerle.
```

## Model/araç değiştirme

- **Codex:** Kök `AGENTS.md` çalışma sözleşmesidir.
- **Gemini CLI:** Kök `GEMINI.md`, `AGENTS.md` dosyasını import eder; `/memory show` ile doğrulanır.
- **Claude Code:** Kök `CLAUDE.md`, `AGENTS.md` dosyasına yönlendirir/import eder.
- **GitHub Copilot:** `.github/copilot-instructions.md`, kök sözleşmeye yönlendirir.
- **Başka bir yerel ajan:** Otomatik talimat dosyası tanımıyorsa ilk prompt'u kullan; kalıcı gerçekleri vendor dosyasına kopyalama.

Model adı veya sohbet oturumu task checkpoint'inin yerine geçmez. Değerlendirme/reprodüksiyon gerekiyorsa kullanılan model, düşünme seviyesi, araç sürümü, izin modu ve `AGENTS.md` commit'i task/eval kaydına yazılır.

## Oturum kesilirse

Yeni ajan şu sırayla toparlar:

1. Task checkpoint'i ve checklist
2. `git status`, diff ve son commit
3. Task çalışma kaydındaki gerçek komut/exit sonuçları
4. Üretilmiş artifact/hash ve source evaluation

Bir dosya değişmiş ama doğrulama kaydı yoksa işlem tamamlanmış sayılmaz. Ajan değişikliği inceleyip testi tamamlar veya güvenli biçimde açık iş olarak checkpoint'e yazar.
