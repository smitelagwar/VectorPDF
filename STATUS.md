# Güncel Durum

**Tarih:** 2026-09-01  
**Faz:** Gate 1 — PDF4QT v1.6.0.0 baseline bake-off aktif  
**Kod durumu:** Upstream kodu henüz içe aktarılmadı (Gate 1 audit izole alanda yürütülür)

## Birincil aktif görev

[TASK-2026-002 — Gate 1 PDF4QT v1.6.0.0 bake-off](docs/tasks/TASK-2026-002-gate1-pdf4qt-bakeoff.md)

Yeni bir AI oturumunda `plana devam et` denildiğinde önce bu dosya, sonra yukarıdaki görev kaydının `Devam checkpoint'i` bölümü okunur. Görev bağlantısı değişmedikçe ayrıntılı kaldığım-yer bilgisi görev dosyasındadır; bu bölüm aynı ayrıntıyı tekrar etmez.

## Tamamlananlar

- Beş referans rapor incelendi ve ortak ilkeler/çelişkiler çıkarıldı.
- Güncel upstream repo, release ve lisansları birincil kaynaklardan kontrol edildi.
- PDF4QT, KillerPDF ve Open PDF Studio kaynakları geçici klasörde incelendi.
- KillerPDF güncel `main` üzerinde yerelde 1.439 engine + 277 app testi çalıştırıldı: toplam **1.716 geçti, 0 başarısız**. Bu yalnız mevcut testlerin geçtiğini gösterir; conformance veya güvenlik sertifikası değildir.
- PDF motoru, standart, OCR, güvenlik, test corpus'u, Windows dağıtımı ve AI yönetişimi için başlangıç belgeleri kuruldu.
- Klasör `main` dallı yerel Git deposu olarak başlatıldı; henüz commit atılmadı.
- Bağımsız dokümantasyon QA'sında P0 bulgu çıkmadı; source-ID/exact-license, contribution policy, trust broker, PDF/UA profile ve task/evaluation kayıtlarındaki P1/P2 tutarsızlıklar düzeltildi.
- `ADR-0001`: permissive rota, root MIT ve inbound MIT+DCO 1.1 Accepted; M0'a kadar haricî katkı kapalı.
- `ADR-0003`: Microsoft desteğindeki Windows 11 x64 ve Store/MSIX-first Accepted; Windows 10 unsupported, ARM64 deferred.
- `ADR-0004`: ücretsiz/açık çekirdek, offline/account-free temel kullanım ve otomatik telemetry upload olmaması Accepted.

## Kabul edilen teknik rota

1. **PDF4QT v1.6.0.0**, Gate 1'in tek fork adayıdır.
2. **KillerPDF v1.8.2**, mevcut permissive rotada yalnız davranış/test referansıdır; kodu alınmaz.
3. PDF4QT kalite kapısını geçmezse yedek yol: permissive özel kabuk + PDFium/qpdf/Tesseract adapter'ları.

Fork kararı, `ADR-0002` Gate 1 kanıtıyla Accepted olana kadar verilmiş sayılmaz.

## Belirlenen marka ve sonraki adımlar

- **Nihai marka:** Product owner tarafından **VectorPDF** olarak belirlendi; M0 rebrand aşamasında GitHub/domain/trademark ön taraması yapılacak. Bu karar Gate 1 teknik kabulünü bloklamaz.

## Sonraki işler

1. PDF4QT exact source/dependency/license/provenance envanterini tamamla (`SRC-APP-001-PDF4QT.md`).
2. Temizse Windows 11 x64 toolchain (CMake/Qt6/MSVC) ile derleme ve gerçek `ctest` aşamasına geç.
3. 25 dosyalık smoke korpusu, atomik kayıt ve izole worker spike'larını yürüt.
4. Gate 1 başarı kriterleri sağlandığında ön onay doğrultusunda doğrudan M0 (VectorPDF rebrand & repo import) aşamasına geç.

## Aktif blokajlar

- `ADR-0002` Gate 1 kanıtıyla Accepted olmadan upstream kodu ürün deposuna alınamaz.
- Gate 1 sonuçlanmadan teknoloji/yapı komutları `AGENTS.md` içine sabitlenemez.
- VectorPDF adı belirlendi; installer identity, publisher, package family ve update channel M0 rebrand aşamasında sabitlenecektir.
