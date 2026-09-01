# TASK-2026-015 — M13 Günlük Araçlar ve Erişilebilirlik (Daily Tools & Accessibility)

- **Durum:** done
- **Sahip:** Technical owner; uygulayıcı ajan
- **Başlangıç:** 2026-09-01
- **Son checkpoint:** 2026-09-01 Europe/Istanbul
- **İlgili REQ/ADR/SOURCE/RISK:** ADR-0001, ADR-0002, ADR-0003, ADR-0004, ADR-0005; REQ-SIGN-*, REQ-DEC-*, REQ-FIND-*, REQ-RED-*, REQ-FLAT-*, REQ-A11Y-*

## Devam checkpoint'i

- **Son doğrulanmış adım:** M13 Günlük Araçlar ve Erişilebilirlik (10/10 araç) C++ modülleri (VectorPDFDocumentTools ve VectorPDFAccessibility), GUI diyalogları (Pdf4QtLibGui), Python masaüstü senkronizasyonu (src_app/vectorpdf_main.py), C++ birim testleri (UnitTests/) ve test paketi (tests/m13/test_m13_suite.py - 8/8 PASSED) başarıyla tamamlandı.
- **Sıradaki tek eylem:** Görev tamamlandı; STATUS.md güncellendi.
- **Blokaj / gereken insan kararı:** Yok.
- **Çalışma ağacı / branch / commit:** main.
- **Devam etmeden önce oku:** AGENTS.md, STATUS.md, ROADMAP.md, docs/ARCHITECTURE.md, docs/SECURITY_AND_PRIVACY.md.

## Amaç

VectorPDF'e normal kullanıcıların günlük olarak en çok ihtiyaç duyduğu 10 yüksek öncelikli özelliği (Fill & Sign, Header/Footer, Watermark, Background, Advanced Find & Replace, Smart Find & Redact, Flatten PDF, Accessibility Checker, Tags & Reading Order Editor, Auto-Tagging) üretim kalitesinde, %100 çevrimdışı, güvenli ve atomik kayıt garantili olarak kazandırmak.

## Kapsam ve Alt Görevler

1. **M13.0 Ortak Foundation:** VectorPDFDocumentTools & VectorPDFAccessibility modülleri, koordinat dönüşümleri, transaction ve doğrulama alt yapısı.
2. **M13.1 Fill & Sign:** Çizim (vektörel QPainterPath), Metin (Türkçe uyumlu font presetleri), Görsel (PNG/JPG/WebP/BMP şeffaflık ayıklı) imza ve paraf sistemi, DPAPI korumalı güvenli imza deposu.
3. **M13.2 Header / Footer / Sayfa Numarası:** 6 konum slotu, dinamik token motoru ({page}, {pages}, {date}, vb.), optimize kaynak paylaşımı, update/remove desteği.
4. **M13.3 Watermark (Filigran):** Metin, Görsel ve PDF Sayfa filigranı, açı/şeffaflık/karolama, içerik önü/arkası yerleşim, yönetilebilir kimlikler.
5. **M13.4 PDF Background (Arka Plan):** Düz renk, Görsel (Fit/Fill/Stretch/Tile), PDF sayfası, Z-order içerik arkası güvencesi, değiştirme ve kaldırma.
6. **M13.5 Advanced Find & Replace:** Regex, tam kelime, büyük/küçük harf, Türkçe İ/ı eşleşmeleri, taşma analizi (Fits, May Overflow), OCR katmanı uyarısı, tek transaction Undo.
7. **M13.6 Smart Find & Redact:** T.C. Kimlik (algoritmik sağlama), IBAN (ISO 13616 MOD-97), Kredi Kartı (Luhn), E-posta, Telefon, IP, URL, Tarih, Plaka algılayıcıları, kullanıcı onay paneli ve kalıcı redaction.
8. **M13.7 Flatten PDF:** Form alanları, açıklamalar, damgalar, çizimler için kalıcı içerik akışı dönüştürme; köprüleri ve dijital imza alanlarını koruma politikası.
9. **M13.8 Accessibility Checker:** Başlık, dil, tag ağacı, okuma sırası, tablo başlıkları, görsel alternatif metinleri kural motoru ve veraPDF PDF/UA doğrulama entegrasyonu.
10. **M13.9 PDF Tags + Reading Order Editor:** /StructTreeRoot, /StructElem, /ParentTree, /MCID tam okuma/yazma, numaralı görsel okuma sırası katmanı, klavye ile düzenleme.
11. **M13.10 Automatic Accessible PDF / Auto-Tag:** Yerel sezgisel etiketleyici (başlık seviyeleri H1-H6, paragraflar, listeler, tablolar, görseller, tekrar eden başlık/altlık tespiti), opsiyonel Docling worker arayüzü ve yerel AI alternatif metin önerisi.
12. **M13.11 Entegrasyon, UI ve Doğrulama:** Pdf4QtLibGui dialog ve dock panelleri, src_app Python arayüzü, kapsamlı C++ ve Python birim testleri.

## Kabul Kriterleri

- [x] VectorPDFDocumentTools modülü derleniyor ve tüm araç transaction'ları atomik çalışıyor.
- [x] VectorPDFAccessibility modülü derleniyor ve tag/reading order/checker kuralları doğrulanıyor.
- [x] Fill & Sign görsel imzaları kriptografik imzalardan ayrıştırılıyor, DPAPI koruması sağlanıyor.
- [x] Header/Footer ve Watermark 200+ sayfalık belgelerde kaynak tekrarı yapmıyor (resource reuse).
- [x] Smart Redact T.C., IBAN, Kart algılayıcıları kesin matematiksel sağlamalarla çalışıyor ve kullanıcı onayı olmadan veri silinmiyor.
- [x] Flatten PDF işleminde köprüler ve dijital imzalar sessizce bozulmuyor.
- [x] veraPDF olmadan PDF/UA uyumluluk iddiası verilmiyor (Honest capability state).
- [x] Tüm C++ ve Python birim testleri başarıyla geçiyor (`python -m pytest tests/m13/test_m13_suite.py` -> 8 passed in 0.06s).
