# PDF Test Corpus Alanı

Bu dizin PDF dosyalarının kendisini değil, güvenli ve lisanslı edinme manifestini tutar.

## Kurallar

- Büyük, özel, lisansı belirsiz veya kötü amaçlı PDF normal Git'e commit edilmez.
- `manifest.yml` canonical URL, exact version/commit, SHA-256, license evidence ve expected behavior olmadan kayıt kabul etmez.
- İndirilen dosya yalnız hash doğrulandıktan sonra cache'e girer.
- `malicious: true` örnekler ayrı `quarantine/` altında; Explorer'da çift tıklamayı teşvik etmeyen isim/ACL ve yalnız sandbox runner ile kullanılır.
- Kullanıcı belgeleri açık rıza, içerik temizliği ve redistribution hakkı olmadan corpus olmaz.
- Oracle “başka viewer açtı” değildir; standard, fixture description veya review edilmiş expected result gerekir.
- Her crash fixture minimize edilir ve ilgili REQ/test/issue ile bağlanır.

Yerel `cache/`, `quarantine/`, render ve extracted output Git'e alınmaz.
