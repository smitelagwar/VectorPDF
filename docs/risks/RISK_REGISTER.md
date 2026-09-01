# Risk Sicili

**Son güncelleme:** 2026-09-01  
**Ölçek:** Olasılık (O) ve Etki (E) 1–5; skor `O×E`. 15–25 kritik, 8–14 yüksek, 4–7 orta, 1–3 düşük. Skor kararın yerine geçmez.

| ID | Risk | O | E | Skor | Azaltma / gate | Sahip | Tetikleyici |
|---|---|---:|---:|---:|---|---|---|
| R-001 | Yanlış lisans/provenance ile kod/asset dağıtımı | 3 | 5 | 15 | Gate 0/1, exact path scan, SBOM/notices, hukuk review | Product+Legal TBD | Yeni fork/dependency/release |
| R-002 | PDF4QT tek geliştirici/bus-factor ve upstream durması | 4 | 4 | 16 | Fork ownership, test/fuzz, patch bütçesi, replacement adapter | Tech owner TBD | 90 gün security response yok, merge cost artar |
| R-003 | Native parser exploit'i UI/user profile'a ulaşır | 4 | 5 | 20 | Worker, AppContainer/restricted token, quotas, mitigations, fuzz | Security owner TBD | Parser/decoder CVE/crash |
| R-004 | Save sırasında orijinal bozulur veya sessiz yapı kaybı | 4 | 5 | 20 | Atomic save, fault injection, preservation matrix, recovery | Document owner TBD | Mutation/save değişikliği |
| R-005 | İmza doğrulaması yanlış güven hissi verir | 3 | 5 | 15 | Ayrık sonuç modeli, ETSI/attack corpus, read-only signed docs | Crypto owner TBD | M9 başlaması |
| R-006 | Redaction altında içerik kalır | 3 | 5 | 15 | Full rewrite, byte/object/extract/render attacks, ayrı kopya | Security owner TBD | M6 başlaması |
| R-007 | Acrobat-benzeri text edit kapsamı patlar | 5 | 4 | 20 | Seviye A/B/C, overlay ayrımı, font/layout corpus, preview | Product owner | M7 scope artışı |
| R-008 | Birden çok motor belgeyi farklı biçimde bozabilir | 3 | 5 | 15 | Tek writer/transaction, file-boundary handoff, validation | Architect TBD | Yeni engine adapter |
| R-009 | Qt/native dependency paketleme ve LGPL yükümlülüğü eksik | 3 | 4 | 12 | Module audit, dynamic link/relink/source/notice checklist | Release+Legal TBD | Qt update/release |
| R-010 | Çok genç adayların release numarası kalite sanılır | 4 | 3 | 12 | Yaş/yıldız değil corpus/security/build gate | Product owner | Baseline review |
| R-011 | OCR Türkçe doğruluğu veya koordinat katmanı zayıf | 3 | 3 | 9 | Ground truth CER/WER/bbox, confidence UI, provider adapter | OCR owner TBD | M5 |
| R-012 | PDF/A/UA validator pass'i fazla pazarlanır | 3 | 4 | 12 | Support matrix, human/AT/visual semantic evidence | QA owner TBD | Conformance release |
| R-013 | Update/signing zinciri ele geçirilir | 2 | 5 | 10 | Isolated key, signed metadata, provenance, rollback/freeze tests | Release security TBD | Public auto-update |
| R-014 | Kullanıcı belgesi log/crash/AI yoluyla dışarı sızar | 3 | 5 | 15 | Data minimization, log scanner, opt-in, no cloud default | Privacy owner TBD | Telemetry/AI/crash changes |
| R-015 | Upstream sync özel patch'lerle imkânsız hale gelir | 4 | 3 | 12 | Thin adapters, upstream PR, distance metrics, sync ADR | Maintainer TBD | Her upstream release |
| R-016 | Windows Store/signing maliyeti veya erişimi engeller | 3 | 3 | 9 | Store-first + direct signed alternative, release-time recheck | Product owner | İlk public release |
| R-017 | Büyük/hostile PDF hang/OOM yaratır | 4 | 4 | 16 | Quotas, progressive render, watchdog, hostile corpus | Security+Perf TBD | M1/M2 |
| R-018 | AI ajanı kanıtsız API/lisans/standart iddiası ekler | 4 | 4 | 16 | AGENTS, source registry, skill, evals, protected zones | Maintainer | Agent/tool update |
| R-019 | Test corpus lisansı veya kötü amaçlı örnek geliştiriciyi riske atar | 3 | 4 | 12 | Manifest-only, hash, quarantine, no double-click dirs | QA+Security TBD | Corpus intake |
| R-020 | Ücretsiz ürünün bakım/signing/support maliyeti sürdürülemez | 4 | 4 | 16 | Scope discipline, sponsor/support model, cost dashboard | Product owner | Public adoption/release |

## En kritik kararlar

1. `R-003/R-004`: güvenli worker + save/recovery M1'den sonraya ertelenemez.
2. `R-001/R-009`: lisans temizliği kod importundan önce yapılır.
3. `R-007`: reflow edit 1.0 şartı değildir.
4. `R-018`: model değişimi docs/skill/evals gate'ini atlayamaz.

## Risk kabul formatı

Kritik/yüksek risk sessizce “kabul” edilmez. Kayıt:

- neden şimdi düzeltilemiyor
- kullanıcıya etkisi ve maruziyet
- geçici kontrol
- owner ve son tarih/gate
- residual score
- rollback/kill switch
- yeniden değerlendirme tetikleyicisi

içerir. Güvenlik, lisans, veri kaybı ve yanlış trust göstergesi riskleri sırf takvim için kabul edilemez; kapsam küçültülür.
