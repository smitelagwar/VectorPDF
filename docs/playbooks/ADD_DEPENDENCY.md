# Playbook: Dış bağımlılık veya upstream kod ekleme

AI kullanıyorsanız `.agents/skills/pdf-dependency-audit/SKILL.md` de zorunludur.

1. İhtiyacı ve mevcut dependency ile çözülemediğini yaz.
2. Canonical repo/site ve exact tag/commit/artifact hash belirle.
3. Root, nested, vendored, asset/model/font ve binary lisanslarını exact revision'da oku.
4. Runtime/build/dev/data/model rolü ve transitif ağacı çıkar.
5. `docs/templates/SOURCE_EVALUATION.md` şablonundan `docs/sources/evaluations/<SOURCE-ID>-<slug>.md` kalıcı kaydı üret; şablonun kendisini düzenleme. License/NOTICE/source/relink/patent/trademark yükümlülüklerini oraya kaydet.
6. Security policy/advisory, son release, maintainer concentration, CI test/fuzz ve Windows build'i incele.
7. Clean PoC: feature, failure/cancel, resource use, removal/replacement sınırı.
8. `SOURCES.md` ve `SOURCE_REGISTRY.yml` kaydı; gerekiyorsa ADR.
9. İnsan/lisans onayı olmadan kod/binary/model içe alma.
10. Kabulde version lock/checksum, SBOM/notices/license texts ve relevant regression tests ekle.

Red koşulları: lisans yok/çelişkili; exact artifact bulunamıyor; default network/secret erişimi kapatılamıyor; temiz build yok; aynı işi mevcut dependency yapıyor; adapter olmadan ürün çekirdeğine yayılıyor.
