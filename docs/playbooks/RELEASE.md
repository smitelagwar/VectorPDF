# Playbook: Release

1. Release scope'u support matrix ve changelog ile dondur.
2. Accepted ADR, risk, decision queue, known blockers ve upstream advisory review.
3. Exact source/dependency/toolchain lock; clean build.
4. Full test, conformance-relevant, hostile/fuzz smoke, performance/a11y regression.
5. İki bağımsız runner'da unsigned payload karşılaştırması veya fark raporu.
6. SBOM, third-party notices/license texts, checksum, provenance ve symbols üret.
7. İzole adımda EXE/DLL/MSIX/updater imzala ve RFC 3161 timestamp uygula.
8. İmza/hash/provenance'ı tüketici gibi tekrar doğrula.
9. Clean VM install/update/skipped update/rollback/uninstall; file associations.
10. Stable'a staged rollout; health signal ve geri alma kararı.
11. Release artifact/commit/test IDs ve known issues engineering log'a ekle.
12. Security/source freshness ve sonraki upstream review tarihlerini güncelle.

Bir adım başarısızsa release kapsamını küçült; test/güvenlik/lisans kapısını atlama.
