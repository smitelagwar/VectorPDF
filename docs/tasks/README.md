# Görev Kayıtları

Bu klasör yalnız anlamlı, birden çok adımlı/oturumlu, protected-zone, source-audit/ADR içeren veya başka bir kişi/ajana devredilecek işler içindir.

Adlandırma:

```text
TASK-YYYY-NNN-<kisa-ad>.md
```

`docs/templates/TASK.md` kopyalanır; şablon düzenlenmez. Küçük tek-oturum değişikliği için gereksiz task dosyası açılmaz, fakat kabul kriteri ve gerçek test sonucu issue/çalışma kaydında yine görünür olmalıdır.

`STATUS.md`, `plana devam et` komutunun sürdüreceği **birincil aktif göreve** bağlantı verir. Ayrıntılı kaldığım-yer bilgisi yalnız o görevin `Devam checkpoint'i` bölümünde tutulur; `STATUS.md` içine kopyalanmaz. Birden fazla `active` görev bulunabilir, fakat birincil görev tektir. Birincil görev değiştiğinde `STATUS.md` aynı değişiklikte güncellenir.

Bir AI oturumu her doğrulanmış anlamlı iş biriminden sonra checkpoint'e son doğrulanmış adımı, sıradaki tek eylemi, gerçek blokajı ve çalışma ağacı/commit bilgisini yazar. Kesilmiş bir oturumdan sonra yeni ajan ayrıca `git status` ve diff ile kaydı uzlaştırır; checklist veya kanıt olmadan işi tamamlanmış varsaymaz.

Tamamlanan kayıt silinmez. Durumu `done` yapılır; gerekirse yıl bazlı `archive/` altına taşınır. Sohbet dökümü, chain-of-thought, secret veya kullanıcı belge içeriği kaydedilmez.
