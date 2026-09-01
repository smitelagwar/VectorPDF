# SRC-APP-002 — KillerPDF v1.8.2

- **Karar:** REFERENCE / REJECT-CURRENT — ADR-0001 permissive rota
- **Canonical:** https://github.com/SteveTheKiller/KillerPDF
- **Release:** `v1.8.2`
- **Commit:** `fca9a6f4bd77d1fdde692bd9e402c07e0ec490de`
- **Lisans:** GPL-3.0-only
- **İnceleme:** 2026-08-31
- **Sonraki inceleme:** Yalnız ADR-0001 yeni ADR ile supersede edilirse

## DOĞRULANDI

- Exact revision [LICENSE](https://github.com/SteveTheKiller/KillerPDF/blob/fca9a6f4bd77d1fdde692bd9e402c07e0ec490de/LICENSE) GPLv3 metnidir; engine project `GPL-3.0-only` package expression kullanır.
- Uygulama .NET 10/WPF; engine ayrı .NET 10 project alanıdır.
- Repo Nisan 2026'da oluşturulmuş, dolayısıyla uzun dönem maintenance/security geçmişi henüz yoktur.
- UI tarafında App/MainWindow XAML/code-behind dosyaları büyük ve refactor riski taşır.

## PROJE İDDİASI

Upstream viewer, annotation, OCR, forms, page ops, text edit, signatures, CLI ve geniş PDF 2.0/PDF/A/PDF/UA engine kapsamı; ayrıca yaklaşık 2.900 dosyalık veraPDF corpus zero-regression gate'i beyan eder. Bunlar exact tag/full corpus bağımsız çalıştırılmadan ürün kanıtı değildir.

## YERELDE KANITLANDI

2026-08-31, current-main `256bef5dbe254b89caddb632d694f3271b83ffc8`, .NET SDK `10.0.400`:

```powershell
dotnet test .\engine\KillerPdf.Engine.Tests\KillerPdf.Engine.Tests.csproj --configuration Release --nologo
# exit 0; 1.439 passed

dotnet test .\KillerPDF.Tests\KillerPDF.Tests.csproj --configuration Release --nologo
# exit 0; 277 passed
```

Toplam 1.716 test, 0 failed/skipped. Bu current-main kanıtıdır; v1.8.2 tag, full corpus, security, performance veya conformance kanıtı değildir.

## Lisans ve ürün sonucu

- Dağıtılan türev ve corresponding source GPLv3 koşullarıyla yayımlanır; kapalı OEM/enterprise türev varsayılmaz.
- Kendi marka kullanılabilir; upstream telif/lisans notice korunur, upstream logo/trademark/asset hakkı ayrıca incelenir.
- NuGet/native PDFium/Tesseract/model/font/installer transitifleri exact tag'de taranmalıdır.

## Teknik risk ve kapı

Genç yaş, self-built engine kapsamı, UI coupling, worker isolation ve hostile corpus Gate 1'de sınanır. GPL kararının kabulü bu kapıları düşürmez. Exact tag build/tests, 25-fixture common corpus, preservation ve worker feasibility geçmeden fork yoktur.

## Alternatif

Permissive yön ADR-0001 ile seçilmiştir. PDF4QT Gate 1'e girer; KillerPDF kodu reference dışında kullanılmaz.
