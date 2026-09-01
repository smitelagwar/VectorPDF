# Ürün ve Sistem Gereksinimleri

**Durum:** Proposed baseline  
**Son güncelleme:** 2026-09-01

Kimlikler kalıcıdır. Gereksinim silinmez; `withdrawn/superseded` yapılır. `Supported` ürün iddiası için test/evidence bağlantısı gerekir.

Durumlar: `PROPOSED`, `ACCEPTED`, `IMPLEMENTED`, `VERIFIED`, `DEFERRED`, `REJECTED`.

## P0 ürün gereksinimleri

| ID | Gereksinim | Durum | Doğrulama |
|---|---|---|---|
| REQ-PROD-001 | Uygulama kendi adı, logo, package/app ID ve update kanalıyla dağıtılabilmelidir. | PROPOSED | Installer/identity audit |
| REQ-PROD-002 | Temel viewer/edit/form işlevleri hesap ve ağ olmadan çalışmalıdır. | PROPOSED | Offline E2E, network deny |
| REQ-PROD-003 | Kullanıcıya ücretsiz dağıtım, seçilen lisans ve third-party yükümlülükleriyle uyumlu olmalıdır. | PROPOSED | ADR-0001 + license scan |
| REQ-PROD-004 | Özellikler capability maturity (`experimental/preview/supported/conformant`) ile doğru adlandırılmalıdır. | PROPOSED | UI/docs/support matrix review |

## P0 açma ve görüntüleme

| ID | Gereksinim | Durum | Doğrulama |
|---|---|---|---|
| REQ-VIEW-001 | Desteklenen PDF'ler open dialog, drag/drop ve file association ile açılmalıdır. | PROPOSED | Windows E2E |
| REQ-VIEW-002 | Zoom, fit, rotate-view, thumbnail ve outline büyük belgede UI'yi bloklamamalıdır. | PROPOSED | Perf + E2E |
| REQ-VIEW-003 | Search/select/copy Unicode ve Türkçe metinde doğru sonuç vermelidir. | PROPOSED | Corpus semantic test |
| REQ-VIEW-004 | Link hedefi açık gösterilmeli; yalnız izinli scheme kullanıcı onayıyla dışarı açılmalıdır. | PROPOSED | Security policy test |
| REQ-VIEW-005 | Unsupported veya kayıplı özellik açılışta/capability panelinde görünmelidir. | PROPOSED | Fixture/UI test |

## P0 veri bütünlüğü

| ID | Gereksinim | Durum | Doğrulama |
|---|---|---|---|
| REQ-SAVE-001 | Orijinal dosya temp→flush→reopen→validate→atomic replace tamamlanmadan değişmemelidir. | PROPOSED | Fault injection/hash |
| REQ-SAVE-002 | Save sırasında disk full, access denied, process kill veya validator fail kaynak dosyayı bozmamalıdır. | PROPOSED | Fault matrix |
| REQ-SAVE-003 | Dışarıdan değiştirilmiş kaynak sessizce overwrite edilmemelidir. | PROPOSED | Race/fingerprint test |
| REQ-SAVE-004 | Crash sonrası doğrulanmış recovery checkpoint/journal bulunmalı; kullanıcı seçim yapabilmelidir. | PROPOSED | Kill/restart E2E |
| REQ-SAVE-005 | Unknown objects ve dokunulmayan yapılar preservation policy olmadan sessizce atılmamalıdır. | PROPOSED | Round-trip matrix |

## P0 düzenleme ve sayfa işlemleri

| ID | Gereksinim | Durum | Doğrulama |
|---|---|---|---|
| REQ-PAGE-001 | Reorder/rotate/delete/duplicate/extract/insert/merge/split undo/redo ile çalışmalıdır. | PROPOSED | Command property + E2E |
| REQ-ANN-001 | Temel annotation create/edit/delete ve save/reopen desteklenmelidir. | PROPOSED | Annotation corpus |
| REQ-EDIT-001 | Overlay text/image, mevcut-content edit ve reflow UI/telemetry/testte farklı capability olmalıdır. | PROPOSED | UX/API review |
| REQ-EDIT-002 | Desteklenmeyen content edit engellenmeli veya açık kayıplı kopya seçeneği sunmalıdır. | PROPOSED | Edge corpus |

## P0 formlar

| ID | Gereksinim | Durum | Doğrulama |
|---|---|---|---|
| REQ-FORM-001 | AcroForm text/check/radio/combo/list alanları doğru value ve appearance ile doldurulmalıdır. | PROPOSED | Cross-reader round-trip |
| REQ-FORM-002 | Read-only/required/validation/tab-order ve accessible-name davranışı korunmalıdır. | PROPOSED | Form/a11y corpus |
| REQ-FORM-003 | Flatten yalnız ayrı kopya ve geri döndürülemez sonuç uyarısıyla yapılmalıdır. | PROPOSED | UI + object test |
| REQ-FORM-004 | XFA çalıştırılmamalı; algılanıp destek düzeyi açıkça gösterilmelidir. | PROPOSED | XFA fixtures |

## P0 güvenlik ve gizlilik

| ID | Gereksinim | Durum | Doğrulama |
|---|---|---|---|
| REQ-SEC-001 | Her PDF güvensiz kabul edilmeli; parse/render/OCR/crypto ana UI sürecinden ayrılmalıdır. | PROPOSED | Process/crash test |
| REQ-SEC-002 | PDF/OCR/conversion/signature-parse worker'larında network/profile/registry/credential/clipboard/child-process erişimi varsayılan deny olmalıdır; dar Trust/Key broker istisnaları ayrı capability ve policy ister. | PROPOSED | Sandbox/capability integration test |
| REQ-SEC-003 | Dosya/page/object/decompression/image/font/time/RAM kotaları olmalıdır. | PROPOSED | Hostile corpus |
| REQ-SEC-004 | JavaScript, Launch, external file/UNC, XFA, rich media ve 3D varsayılan kapalı olmalıdır. | PROPOSED | Action fixtures |
| REQ-PRIV-001 | Belge içeriği, OCR metni, path, form verisi, parola ve key log/telemetride bulunmamalıdır. | PROPOSED | Log/dump scanner |
| REQ-PRIV-002 | Temel ürün kullanımı için cloud/account gerekmemelidir. | PROPOSED | Offline E2E |

## P0 erişilebilirlik

| ID | Gereksinim | Durum | Doğrulama |
|---|---|---|---|
| REQ-A11Y-001 | Ana iş akışları keyboard-only tamamlanabilmelidir. | PROPOSED | Manual/E2E |
| REQ-A11Y-002 | Focus, high contrast, scale ve UI Automation/Narrator davranışı desteklenmelidir. | PROPOSED | Windows a11y matrix |
| REQ-A11Y-003 | Belge tag/reading-order kaybı yaratabilecek edit kullanıcıya bildirilmelidir veya engellenmelidir. | PROPOSED | Tagged corpus |

## P1 OCR

| ID | Gereksinim | Durum | Doğrulama |
|---|---|---|---|
| REQ-OCR-001 | OCR offline, ayrı worker ve provider adapter arkasında çalışmalıdır. | PROPOSED | No-network/process test |
| REQ-OCR-002 | İlk dil seti `tur+eng`; model sürümü/hash/provenance sabit olmalıdır. | PROPOSED | Artifact audit |
| REQ-OCR-003 | Sonuç text yanında bbox/polygon, confidence, language ve model sürümü taşımalıdır. | PROPOSED | Contract test |
| REQ-OCR-004 | Searchable layer kaynak tarama pikselini varsayılan değiştirmemelidir. | PROPOSED | Render diff |

## P1 redaction

| ID | Gereksinim | Durum | Doğrulama |
|---|---|---|---|
| REQ-RED-001 | Redaction alttaki text/image/vector içeriğini ve gerekli history'yi geri alınamaz biçimde kaldırmalıdır. | PROPOSED | Byte/object/extract/render attack test |
| REQ-RED-002 | Mark ve Apply iki ayrı adım; kaynak üzerine değil yeni artifact'e uygulanmalıdır. | PROPOSED | E2E |
| REQ-RED-003 | Metadata/attachment/hidden layer kapsamı dönüşüm raporunda gösterilmelidir. | PROPOSED | Semantic scan |

## P2 conformance ve imza

| ID | Gereksinim | Durum | Doğrulama |
|---|---|---|---|
| REQ-PDFA-001 | PDF/A export hedef profile sahip ayrı conversion job ve kayıp raporu üretmelidir. | PROPOSED | veraPDF + visual/semantic |
| REQ-PDFA-002 | PDF/A ile encryption çatışması engellenmelidir. | PROPOSED | Policy test |
| REQ-UA-001 | PDF/UA iddiası exact profile/version, profile-specific validator, insan kontrolü ve screen-reader kanıtı istemelidir. | PROPOSED | Conformance bundle |
| REQ-UA-002 | PDF/UA-1 kanıtı UA-1 reference suite + Matterhorn 1.1 + assistive-technology testini içermelidir. | PROPOSED | UA-1 conformance bundle |
| REQ-UA-003 | PDF/UA-2 kanıtı pinned UA-2 validator profile + ISO 14289-2'ye dayalı ayrı insan/AT kontrolü kullanmalı; UA-1 suite'i kanıt saymamalıdır. | PROPOSED | UA-2 conformance bundle |
| REQ-SIGN-001 | Görsel imza, signature field, crypto verification, trust ve PAdES profile ayrı gösterilmelidir. | PROPOSED | UX/domain test |
| REQ-SIGN-002 | Signed/certified belge varsayılan read-only; mutation etkisi komut öncesi gösterilmelidir. | PROPOSED | Revision corpus |
| REQ-SIGN-003 | Integrity, later change, crypto, chain, trust, revocation ve timestamp ayrı sonuç olmalıdır. | PROPOSED | Attack/trust corpus |
| REQ-SIGN-004 | AIA/OCSP/CRL/RFC 3161 ve private-key işlemleri genel PDF worker yetkisi değil, endpoint-kısıtlı TrustNetworkBroker ve non-exportable KeyBroker üzerinden yapılmalıdır. | PROPOSED | Offline/SSRF/key-isolation tests |
| REQ-CRYPT-001 | Yeni encryption varsayılanı kanıtlanmış AES-256 Revision 6; eski zayıf yöntemler üretilmemelidir. | PROPOSED | Object/interop test |

## NFR

| ID | Gereksinim | Durum | Doğrulama |
|---|---|---|---|
| REQ-NFR-001 | Desteklenen Windows sürümleri ve hardware tier'ları açık politika ve test matrisine sahip olmalıdır. | PROPOSED | Release matrix |
| REQ-NFR-002 | Performance bütçeleri baseline donanımında P50/P95/peak ile kaydedilmeli ve ratchet edilmelidir. | PROPOSED | Benchmark CI/manual |
| REQ-NFR-003 | Build exact dependency/toolchain ile temiz runner'da tekrarlanmalıdır. | PROPOSED | Rebuild/hash |
| REQ-NFR-004 | Her release SBOM, notices, checksum, provenance ve signed artifacts üretmelidir. | PROPOSED | Release verification |
| REQ-NFR-005 | Update signature/hash/expiry/rollback/freeze kontrolü yapmalıdır. | PROPOSED | Update attack tests |
| REQ-NFR-006 | Worker crash/hang/OOM ana UI ve kaynak belgeyi kaybettirmemelidir. | PROPOSED | Fault E2E |

## Traceability güncelleme kuralı

Bir satır `VERIFIED` olduğunda ilgili test ID/run artifact ve support-matrix hücresi eklenir. Bir capability regression alırsa gereksinim silinmez; `IMPLEMENTED`/`PROPOSED` düzeyine düşürülür ve release note yazılır.
