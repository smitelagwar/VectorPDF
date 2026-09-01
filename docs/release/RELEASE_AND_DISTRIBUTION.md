# Windows Release ve Dağıtım Planı

**Durum:** Active — ADR-0003 yönü Accepted  
**Son güncelleme:** 2026-09-01

## Kanal kararı

### Birincil: Microsoft Store + MSIX

Avantajlar:

- Kullanıcıya güvenilir edinme ve update kanalı
- Store submission'da paket imzalama/reputation avantajı
- WPF/Win32/Qt uygulamalar paketlenebilir
- Otomatik update ve daha temiz uninstall

Store kullanılabilirliği, ücret/hesap/ülke şartları release tarihinde resmî Microsoft kaynağından tekrar doğrulanır. Store dağıtımı parser sandbox'ı değildir; desktop process full-trust olabilir.

### İkincil: doğrudan imzalı indirme

Gerekirse MSIX/App Installer veya imzalı installer/portable zip sunulur. Kamuya güvenilen code-signing sertifikası veya uygun cloud signing gerekir. Self-signed sertifika genel kullanıcı çözümü değildir; EV sertifika da SmartScreen itibarını otomatik garanti etmez.

Özel updater ancak Store/App Installer ihtiyacı karşılamazsa yapılır.

## Kimlikler

M0'da tek kayıt:

- Product/display name
- Publisher legal/display identity
- Package family/identity
- Upgrade code/product code (installer türüne göre)
- Executable/service/worker names
- File associations ve protocol scheme
- Registry/config/cache/recovery paths
- Update channel ve feed roots
- Crash/app IDs

Upstream kimliklerinin hiçbiri yanlışlıkla korunmaz; telif/notice ayrı tutulur. Rename script/config tek kaynaktan idempotent olmalıdır.

## Build pipeline

1. Immutable source tag/commit checkout
2. Pinned toolchain/dependency restore; checksum/digest verify
3. License policy ve source-registry freshness
4. Clean release build; final compile aşamasında mümkünse ağ kapalı
5. Unit/integration/golden/relevant conformance/hostile smoke
6. Unsigned payload + symbols + SBOM + notices + provenance
7. İkinci bağımsız runner'da unsigned payload karşılaştırması
8. Isolated signing step
9. Signature/timestamp/SBOM/provenance verification
10. Clean VM install/update/rollback/uninstall smoke
11. Staged publish; monitoring; rollback window

Toolchain locale/timezone/path/order ve `SOURCE_DATE_EPOCH` gibi build inputs kaydedilir. Reproducibility hedefi imzadan önceki payload içindir; Authenticode timestamp doğal olarak farklı olabilir.

## Artifact seti

Her release:

- MSIX/installer/portable artifact (desteklenen kanala göre)
- SHA-256 checksum dosyası
- Authenticode signature + RFC 3161 timestamp
- SPDX veya CycloneDX SBOM
- third-party notices ve license texts
- build provenance/attestation
- ayrı symbol package ve kontrollü retention
- source tag/commit ve gerekiyorsa GPL/LGPL source/relink materyali
- release notes, known issues, capability maturity
- rollback artifact/metadata

SBOM native DLL, CRT/runtime, Qt modülü, codec, font, ICC profile, OCR engine/model ve optional sidecar'ı içerir.

## Signing key yönetimi

- Key normal developer/CI runner diskinde yoktur.
- HSM/cloud signing/izole release host tercih edilir.
- Least privilege ve onaylı release workflow.
- Audit log belge/secret içermez.
- Key rotation, compromise ve revocation runbook'u public beta öncesi test edilir.
- Dev/test key ile production key ayrıdır.

## Güvenli update

Store dışı updater gerekiyorsa yalnız HTTPS yetmez:

- pinned offline root
- signed targets/snapshot/timestamp metadata veya TUF-benzeri model
- version, expiry, hash, size ve signature doğrulaması
- rollback/freeze koruması
- threshold/rotation strategy
- least-privilege download; yükseltilmiş işlem yalnız commit aşaması
- atomic swap ve health-check rollback
- staged stable/beta kanalları; downgrade policy
- unsigned plugin/DLL side-load yok

Update metadata/key değişikliği protected zone ve ADR gerektirir.

## Windows test matrisi

`ADR-0003` ile kabul edilen politika:

- Release tarihinde Microsoft tarafından desteklenen Windows 11 sürümleri
- x64 primary; ARM64 yalnız native/emulation test kanıtından sonra
- Windows 10 desteklenmez; çalışması support claim değildir
- standard user, non-admin install/use
- high DPI/multi-monitor/high contrast/Turkish locale
- NTFS primary; network share/removable filesystem ayrı policy
- clean install, in-place update, skipped-version update, rollback, uninstall/reinstall

EOL işletim sistemi için güvenlik desteği vaat edilmez.

## Release branch ve kanallar

- `nightly`: güvenilmez; test kullanıcıları, production document önerilmez
- `preview/beta`: capability sınırları ve telemetry policy açık
- `stable`: bütün P0 gates ve destek matrisindeki claims

SemVer ürün API'si ve kullanıcı beklentisi için kullanılır; çok hızlı büyük sürüm numarası kalite kanıtı değildir.

## Release checklist

### Karar ve kaynak

- [ ] Accepted ADR'ler ve açık decision queue kontrol edildi
- [ ] Source registry freshness ve exact lock doğrulandı
- [ ] License/path/asset/model scan temiz
- [ ] Upstream advisory/CVE review tamam

### Kalite

- [ ] P0/P1 release blockers yok
- [ ] Full test + save/reopen/preservation
- [ ] Hostile/fuzz smoke
- [ ] Accessibility ve performance regression review
- [ ] Supported Windows clean VM matrisi

### Supply chain

- [ ] Clean/reproducible unsigned build karşılaştırması
- [ ] SBOM/notices/license texts
- [ ] Provenance/attestation
- [ ] Artifact checksum
- [ ] Signature/timestamp verify

### Dağıtım

- [ ] Install/update/skipped update/rollback/uninstall
- [ ] File association ve identity
- [ ] Privacy/security notices ve support channel
- [ ] Release notes destek iddiasıyla tutarlı
- [ ] Rollback artifact hazır

Release işlemi [../playbooks/RELEASE.md](../playbooks/RELEASE.md) kaydıyla yürütülür.
