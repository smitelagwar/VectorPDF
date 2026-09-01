# Kalıcı Kaynak Değerlendirmeleri

Her dış fork, dependency, binary, model, font, asset veya corpus adayı için şablonun kendisi değil, burada ayrı ve kalıcı bir kayıt oluşturulur:

```text
docs/sources/evaluations/<SOURCE-ID>-<kisa-ad>.md
```

- `docs/templates/SOURCE_EVALUATION.md` yalnız şablondur; audit sırasında düzenlenmez.
- Dosya source ID ile başlar ve `SOURCE_REGISTRY.yml` içinden `evaluation_path` ile bağlanır.
- Accepted değerlendirme geçmişi sessizce yeniden yazılmaz; yeni revision bölümü veya superseding kayıt eklenir.
- Reference-only kaynak için dosya zorunlu değildir. Kod/asset/model/binary importu, upstream sync veya dependency update öncesi zorunludur.

## Mevcut değerlendirmeler

- [SRC-APP-001 — PDF4QT](SRC-APP-001-PDF4QT.md)
- [SRC-APP-002 — KillerPDF](SRC-APP-002-KillerPDF.md)
- [SRC-APP-003 — Open PDF Studio](SRC-APP-003-Open-PDF-Studio.md)
