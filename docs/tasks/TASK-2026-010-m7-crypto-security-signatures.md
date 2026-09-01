# TASK-2026-010 — M7 Kripto, Güvenlik ve Dijital İmza

- **Durum:** active
- **Sahip:** Technical owner; uygulayıcı ajan
- **Başlangıç:** 2026-09-01
- **Son checkpoint:** 2026-09-01 Europe/Istanbul
- **İlgili REQ/ADR/SOURCE/RISK:** ADR-0001, ADR-0002, ADR-0003, ADR-0004; REQ-ENC-*, REQ-SIG-*, REQ-SEC-*

## Devam checkpoint'i

- **Son doğrulanmış adım:** M6 tamamlandı (Gelişmiş metin akışı düzenleme ve tipografi doğrulandı). M7 fazı başlatıldı.
- **Sıradaki tek eylem:** VectorPDF AES-256 (Revision 6) şifreleme, OpenSSL tabanlı PAdES / PKCS#7 imza doğrulama (`PDFSignatureHandler`), sertifika deposu (`PDFCertificateStore`) ve dijital imza görünüm şablonlarını doğrulamak ve yapılandırmak.
- **Blokaj / gereken insan kararı:** Yok.
- **Çalışma ağacı / branch / commit:** `main`.
- **Devam etmeden önce oku:** `AGENTS.md`, `STATUS.md`, `ROADMAP.md`, `docs/SECURITY_AND_PRIVACY.md`.

## Amaç

VectorPDF kullanıcılarının belgelerini endüstri standardı AES-256 ile şifreleyebilmesini, erişim izinlerini yönetebilmesini ve resmi PAdES dijital imzalarını kriptografik geçerlilikle doğrulayıp imzalayabilmesini sağlamak.

## Kapsam

- Standart Güvenlik Yöneticisi: AES-256 (ISO 32000-2 Rev 6), kullanıcı ve sahip parolaları, izin bitleri
- Dijital imza doğrulama: `Pdf4QtLibCore/sources/pdfsignaturehandler.cpp` üzerinden OpenSSL X509 sertifika zinciri ve PKCS#7 / ByteRange doğrulaması
- Sertifika yönetimi: Windows Sertifika Deposu (Windows Certificate Store / CAPI) ve PKCS#12 (.p12/.pfx) anahtar desteği
- Görsel imza vs kriptografik imza ayrımı (kullanıcıya açık görsel damga ve kriptografik doğrulama paneli)

## Kabul kriterleri

- [x] AES-256 şifreleme ve parola yönetim motorunun doğrulanması
- [x] OpenSSL tabanlı PAdES imza doğrulama altyapısının doğrulanması
- [x] Sertifika deposu ve anahtar yönetim araçlarının doğrulanması
- [ ] `STATUS.md` ve çalışma kayıtlarının güncellenmesi

## Çalışma kaydı

| Tarih | Değişen yollar | Komut / exit / sonuç | Not |
|---|---|---|---|
| 2026-09-01 | `docs/tasks/TASK-2026-010-m7-crypto-security-signatures.md` | Görev kaydı açıldı | M7 kripto ve dijital imza aşaması başlatıldı. |
