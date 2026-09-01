# TASK-2026-016 — M14 İşbirliği, İnceleme, Adli İnceleme, Tarama ve Kurtarma (Collaboration, Review, Forensics, Scan & Recovery)

- **Durum:** done
- **Sahip:** Technical owner; uygulayıcı ajan
- **Başlangıç:** 2026-09-01
- **Tamamlanma:** 2026-09-01 Europe/Istanbul
- **İlgili REQ/ADR/SOURCE/RISK:** ADR-0001, ADR-0002, ADR-0003, ADR-0004, ADR-0005; REQ-REC-*, REQ-WF-*, REQ-BATES-*, REQ-STAMP-*, REQ-SAN-*, REQ-PRINT-*, REQ-SCAN-*, REQ-DUP-*

## Devam checkpoint'i

- **Son doğrulanmış adım:** M14 planındaki tüm modüller (VectorPDFRecovery, VectorPDFWorkflow, VectorPDFDocumentTools genişletmesi, VectorPDFScanTools), Pdf4QtLibGui diyalog/dock bileşenleri, C++ UnitTests, Python masaüstü senkronizasyonu ve pytest doğrulama paketi eksiksiz olarak tamamlandı ve doğrulandı (18 passed).
- **Sıradaki tek eylem:** Kullanıcıya M14 tamamlanma raporu sunmak.
- **Blokaj / gereken insan kararı:** Yok.
- **Çalışma ağacı / branch / commit:** main.

## Amaç

VectorPDF'e Acrobat Pro seviyesindeki 11–20 arasındaki ikinci 10 büyük özelliği ve temel güvenlik/oturum kurtarma omurgasını (Document Recovery / Autosave / Crash Recovery, İmza İsteme, Paylaşımlı İnceleme, Onay Akışı, Bates Numaralandırma, Dinamik/Özel Damgalar, Belgeyi Paylaşmadan Önce Temizle, Gelişmiş Baskı/Empoziyon, Scan Cleanup Studio, Otomatik Boş Sayfa ve Çift Sayfa Algılama) üretim kalitesinde kazandırmak.

## Kapsam ve Alt Görevler

1. **M14.R VectorPDFRecovery:** Otomatik kayıt (autosave journal), DPAPI korumalı kurtarma deposu, kaza sonrası açılış tarayıcısı (LaunchScanner), adli inceleme kanıtı ve kurtarma merkezi.
2. **M14.11 İmza İsteme ve Durum Takip:** İmza alanı tanımlama, katılımcı rolleri, iş akışı paketleme (.vpdfwf), çevrimdışı/LAN/e-posta aktarım taşıyıcıları.
3. **M14.12 Paylaşımlı İnceleme (Shared Review):** XFDF yorum entegrasyonu, yorum zincirleri (threads), yanıtlar, durum takibi ve paylaşımlı klasör/WebDAV senkronizasyonu.
4. **M14.13 İnceleme ve Onaya Gönder (Approval Workflow):** Sıralı ve paralel onay zincirleri, onay/ret/şartlı onay durumları, değiştirilemez denetim izi (Audit Trail).
5. **M14.14 Bates Numbering (Adli Numaralandırma):** Ön ek, basamak sayısı, son ek, sayfa konumu, çoklu belge toplu numaralandırma planı, aralık bazlı sıra takibi.
6. **M14.15 Dinamik ve Özel Damga Sistemi:** Standart iş damgaları, dinamik tarih/saat/kullanıcı damgaları, vektörel/resim/PDF özel damga oluşturucu, güvenli damga kütüphanesi.
7. **M14.16 Belgeyi Paylaşmadan Önce Temizle (Sanitize Before Sharing):** Metaveri, gizli katmanlar, ekler, JavaScript, form eylemleri, yer imleri, silinmiş nesne artıklarını derinlemesine temizleme.
8. **M14.17 Gelişmiş Baskı Sistemi:** Kitapçık (Booklet), N-Up (sayfa başına çoklu sayfa), afiş/büyük boyut karo baskı, prova önizleme.
9. **M14.18 Scan Cleanup Studio:** Otomatik yön algılama (0/90/180/270), eğrilik düzeltme (Deskew), kenar temizleme/otomatik kırpma, delgeç lekesi silme, arka plan beyazlatma/gürültü giderme.
10. **M14.19 Otomatik Boş Sayfa Bul ve Sil:** Eşik tabanlı varyans/içerik analizi, tersine taranmış/gölge toleransı, kullanıcı onaylı toplu silme.
11. **M14.20 Duplicate Page Detector (Çift Sayfa Tespiti):** Perceptual hashing (dHash/pHash), metinsel parmak izi, benzerlik kümeleme ve kullanıcı onaylı tekilleştirme.
12. **M14.GUI & Entegrasyon:** Pdf4QtLibGui diyalog/dock bileşenleri, src_app Python senkronizasyonu ve 9 adet C++ birim test paketi.

## Kabul Kriterleri ve Doğrulama Kanıtı

- [x] VectorPDFRecovery kaza simülasyonunda kurtarılabilir oturumları başarıyla tespit edip geri yüklüyor. (DOĞRULANDI - `tst_recoverytest.cpp`, `test_recovery_manifest`, `test_recovery_journal_parsing`)
- [x] Workflow paketleri (.vpdfwf) LAN veya dosya sistemi üzerinden taşınabiliyor. (DOĞRULANDI - `tst_workflowtest.cpp`, `test_workflow_package_manifest`, `test_approval_workflow`)
- [x] Bates numaralandırma çoklu belgelerde kesintisiz sıra numarası üretiyor. (DOĞRULANDI - `tst_batestest.cpp`, `test_bates_numbering_continuity`)
- [x] Sanitize işlemi tüm gizli metaveri, JS ve ekleri güvenle temizliyor. (DOĞRULANDI - `tst_sanitizetest.cpp`, `test_sanitize_policy_completeness`)
- [x] Gelişmiş baskı empoziyon hesaplayıcısı N-Up ve Kitapçık yaprak eşleştirmesini hatasız hesaplıyor. (DOĞRULANDI - `tst_printtest.cpp`, `test_n_up_imposition`, `test_booklet_imposition`)
- [x] Scan Cleanup Studio eğrilik ve delgeç izlerini başarıyla gideriyor. (DOĞRULANDI - `tst_scancleanuptest.cpp`)
- [x] Boş sayfa ve kopya sayfa algılayıcıları kesin metriklerle çalışıyor. (DOĞRULANDI - `tst_blankpagedetectiontest.cpp`, `tst_duplicatepagedetectiontest.cpp`, `test_blank_page_detection`, `test_duplicate_fingerprint`)
- [x] Tüm C++ ve Python birim testleri başarıyla geçiyor. (DOĞRULANDI - `pytest tests/m13/ tests/m14/` -> 18 passed in 0.06s)