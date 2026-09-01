# Güvenlik ve Mahremiyet Tasarımı

**Durum:** Threat model başlangıç sürümü  
**Son güncelleme:** 2026-09-01

## Korunan varlıklar

- Kullanıcının orijinal PDF'si ve dosya sistemi
- PDF içeriği, çıkarılmış metin, attachment ve metadata
- Parolalar, sertifika private key'leri, token/credential'lar
- Uygulama update/signing anahtarları
- Clipboard, yazıcı, kamera/tarayıcı ve ağ erişimi
- Kullanıcının bir imza/redaction/conformance sonucuna duyduğu güven

## Tehdit modeli

Saldırgan:

- özel hazırlanmış bozuk PDF, font, image, object stream, XFA/JavaScript/action veya attachment sağlar;
- parser/render/OCR'da memory corruption, hang, zip/decompression bomb veya path traversal tetikler;
- imza görünümünü veya post-sign revision'ı manipüle eder;
- redacted içeriği object/history/metadata'dan geri çıkarmaya çalışır;
- update kanalına artifact sokar;
- log/crash dump/telemetri yoluyla belge içeriği toplar;
- dependency typosquat veya upstream compromise kullanır.

Yerel disk, e-posta eki, tarayıcı download'u veya kurumsal share fark etmez: her PDF güvensizdir.

## Trust boundaries

| Sınır | İçeride neye güvenilir? | İzin verilmeyen |
|---|---|---|
| UI process | Kullanıcı niyeti ve policy orchestration | PDF parse/render/OCR/crypto, key erişimi |
| PDF worker | Yalnız verilen read handle ve temp output | Ağ, profile, registry, clipboard, child process, key store |
| OCR worker | Rendered page image + model | Kaynak dosya sistemi/ağ |
| Signature parse/crypto worker | Belirli digest/revision + public certificate data | Ağ, key store, private key export/log |
| Trust network broker | Yalnız policy-onaylı AIA/OCSP/CRL/RFC 3161 isteği | Genel web erişimi, PDF byte/text, arbitrary URL/redirect/private IP |
| Key broker | Windows certificate store/CNG/smart-card üzerinde kontrollü sign operation | Private key export, belge parse/render, genel registry/profile erişimi |
| Update service | Signed metadata + pinned root | Uygulamadan key alma, imzasız payload |
| Optional conversion worker | Tek input/output ve limitli runtime | Genel shell, kullanıcı profile, ağ |

## Dangerous PDF features

Varsayılan deny:

- JavaScript ve automatic actions
- Launch actions ve shell execution
- `file:` / UNC / external file references
- embedded executable ve attachment auto-open
- XFA execution/dynamic XFA
- rich media, video/audio auto-play ve 3D
- remote content/network fetch

HTTPS link bile otomatik açılmaz; tam host/path kullanıcıya gösterilir ve OS browser'a açık onayla verilir. URI scheme allowlist kullanılır.

Attachment çıkarma kullanıcı seçimiyle ayrı klasöre yapılır; path canonicalize edilir, traversal engellenir, executable uyarısı gösterilir ve Windows güvenlik/AV/MOTW davranışı PoC ile uygulanır.

## Worker hardening

- Process başına veya kontrollü havuz; farklı güven seviyeleri karıştırılmaz.
- Job Object: kill-on-close, child-process deny, memory/CPU/time limiti.
- Restricted token; uygun olduğunda AppContainer/LPAC/Windows App Isolation.
- PDF/OCR/conversion/signature-parse worker'larında network capability yok. Trust network broker ayrı kimlik ve yalnız scheme/host/IP/redirect/response-size/time allowlist'iyle çalışır; key broker ayrı OS capability'sidir.
- DEP, ASLR, CFG, CET ve derleyici hardening; uyumluluk testli.
- Read-only input; broker kontrollü temp output.
- Watchdog, cancel ve worker recycle.
- Crash/hang/OOM iş başarısızlığıdır, UI recovery sunar.

### Kota sınıfları

Sayılar baseline benchmark'ından sonra ADR ile sabitlenir; en az:

- file size, page count, object/xref count
- object nesting/recursion
- total decompressed bytes ve compression ratio
- image dimensions/pixel count
- font/glyph work
- render DPI/bitmap bytes
- OCR page/time/RAM
- attachment count/size
- total job wall time

limitleri vardır. Kullanıcı limiti artırabiliyorsa risk açıkça gösterilir; sınırsız seçeneği varsayılan olmaz.

## Güvenli save ve path işlemleri

- Path'ler canonicalize edilir; symlink/reparse-point, ADS ve device path senaryoları test edilir.
- Kaynak fingerprint commit öncesi yeniden kontrol edilir.
- Temp aynı volume'de ve dar ACL ile oluşturulur; ad tahmin edilemez.
- Output worker'dan geldikten sonra ayrı process/engine ile tekrar açılıp doğrulanır.
- Replace atomik ve rollback edilebilir olmalıdır; fault-injection testleri elektrik kesintisi/process kill/disk full/access denied içerir.
- Network share ve removable drive davranışı yerel NTFS ile aynı varsayılmaz; ayrı support policy gerekir.

## İmza güvenliği

- Signed bytes, current document bytes ve visible appearance ayrı gösterilir.
- UI sonuçları: integrity, later revision, crypto, certificate chain, trust policy, revocation, timestamp, qualification.
- Offline doğrulamada revocation bilinmiyorsa “geçerli” denmez.
- Certificate expiry, revoked/untrusted root, multiple signatures ve certified permissions test edilir.
- Shadow/ISA/SWA sınıfı saldırı corpus'u zorunludur.
- Düzenleme öncesi imza etkisi; signed/certified belgede varsayılan read-only.
- Private key export edilmez; log/dump'a girmez; Windows CNG/store veya güvenli token/HSM kullanılır.
- AIA/OCSP/CRL ve RFC 3161 erişimi offline-first üründe isteğe bağlıdır. Açıldığında kullanıcı/kurum politikası, endpoint allowlist'i, DNS/IP/redirect SSRF savunması, küçük response limiti ve ayrı cache/retention uygulanır; PDF içeriği veya kullanıcı credential'ı gönderilmez.

## Redaction ve sanitize

- Redaction mark ile apply iki ayrı adımdır.
- Apply yalnız görünüşü kapatmaz; text/image/vector, ilgili metadata/attachment ve eski revision/history incelenir.
- Full rewrite gerektirebilir ve imzaları bozar; kaynak üzerine yazılmaz.
- Sonuç byte search, object traversal, text extraction ve render ile test edilir.
- “Sanitize” kayıplı yeni artifact ve değişiklik raporudur; mutlak güven garantisi diye pazarlanmaz.

## Encryption

- Eski algoritmalar uyumluluk için okunabilir; yeni dosyada varsayılan AES-256 Revision 6 hedefidir.
- RC4/40-bit/eski 128-bit ve Revision 5 yeni üretimde yoktur.
- Permission flags güçlü DRM değildir; UI dürüstçe açıklar.
- Owner password boş bırakılmaz.
- AES-GCM, ekosistem uyumluluğu corpus ile kanıtlanmadan varsayılan olmaz.
- Parolalar kısa ömürlü mutable buffer'da tutulur, iş sonunda zeroize edilir; string/log/exception/telemetriye düşmez.
- PDF/A + encryption ve signed encrypted document çatışma matrisi vardır.

## Mahremiyet

Varsayılan:

- belge, sayfa görüntüsü, OCR metni, filename/path, annotation veya form verisi cihaz dışına çıkmaz;
- ürün temel özellikleri hesap gerektirmez;
- analytics yoktur veya yalnız explicit opt-in anonim olaydır;
- crash report belge byte/text/path/parola/key içermez ve gönderim öncesi gösterilebilir;
- recent-file listesi yereldir, kapatılabilir ve temizlenebilir;
- hassas thumbnail/cache şifreleme/ACL ve retention politikasıyla yönetilir;
- AI özelliği ileride gelirse per-task açık rıza, hedef sağlayıcı, gönderilen veri önizlemesi ve offline fallback gerekir.

Kullanıcı belgesi test corpus'una veya model eğitimine kendiliğinden eklenmez.

## Log sınıfları

İzinli: version, operation ID, capability/result code, duration bucket, page count bucket, worker exit class, sanitized stack/module.  
Yasak: full path/filename, document bytes/text/image, form value, password, key/certificate secret, raw URI query, attachment content.

Debug modu bile hassas veriyi varsayılan loglamaz. Gerekli forensic artifact kullanıcıya açık, süreli ve şifreli opt-in paketidir.

## Tedarik zinciri ve update

- Dependency/action/toolchain exact sürüm/commit veya digest pinli.
- SBOM, notices, provenance ve SHA-256 her release'te.
- Signing key normal CI runner'da bulunmaz.
- HTTPS tek başına yeterli değildir; artifact ve metadata signature/hash/size/expiry doğrulanır.
- Rollback/freeze ve key rotation senaryoları test edilir.
- Plugin/updater unsigned code yüklemez.

## Güvenlik release bloklayıcıları

- UI process'te unbounded untrusted parse/render
- Orijinal üzerine doğrulamasız write
- Açık PDF JavaScript/Launch/network
- Log/crash dump'ta belge veya secret
- İmza sonucu yanlış “valid” gösterimi
- Redaction içeriğinin geri alınabilmesi
- İmzasız/unpinned update/dependency
- Kritik/yüksek CVE için mitigation veya kabul edilmiş risk olmaması

## Olay yönetimi

Public release öncesi özel security-reporting kanalı, disclosure policy, CVE koordinasyonu ve response owner atanır. Kötü amaçlı örnekler normal issue'ya yüklenmez; karantina/şifreli aktarım kullanılır. Her P0/P1 güvenlik olayı için etki, root cause, düzeltme, regression fixture ve takip eylemi olan postmortem gerekir.
