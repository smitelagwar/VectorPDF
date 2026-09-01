# Mühendislik Günlüğü

Bu append-only günlük önemli araştırma, PoC, test ve artifact kanıtını özetler. Kullanıcı-facing değişiklik günlüğü değildir; Git diff ve ADR'nin yerine geçmez. Hassas çıktı/belge içeriği kaydedilmez.

## 2026-08-31 / 2026-09-01 — Nihai plan ve upstream doğrulaması

**Amaç:** Beş referans rapordan bağımsız, güncel birincil kaynak ve yerel kanıtla baseline/roadmap/AI çalışma sistemi kurmak.

**İncelenenler**

- `referanslar/` altındaki 5 rapor
- PDF4QT, KillerPDF, Open PDF Studio güncel kaynakları geçici çalışma alanında
- Uygulama/motor/OCR/standart/security/corpus/release/AI resmî kaynakları; ayrıntı `SOURCES.md`

**Yerel kaynak snapshot'ları**

- KillerPDF current-main: `256bef5dbe254b89caddb632d694f3271b83ffc8`
- PDF4QT current-main: `0be824befae6b90789ec700f62a2e66a025e10a4`
- Snapshot'lar `%LOCALAPPDATA%\Temp` altında geçiciydi; ürün koduna kopyalanmadı.

**Çalıştırılan testler**

```powershell
dotnet test .\engine\KillerPdf.Engine.Tests\KillerPdf.Engine.Tests.csproj --configuration Release --nologo
# exit 0: 1.439 passed, 0 failed, 0 skipped

dotnet test .\KillerPDF.Tests\KillerPDF.Tests.csproj --configuration Release --nologo
# exit 0: 277 passed, 0 failed, 0 skipped
```

Ortam: Windows, .NET SDK `10.0.400`. Toplam 1.716 test geçti. Bu çalışma full corpus, security, performance veya conformance kanıtı değildir.

**Çalıştırılamayan**

```powershell
cmake --version
# exit 1: cmake komutu bu makinede bulunamadı
```

Bu nedenle PDF4QT yerelde build/test edilmedi; Gate 1'in ilk zorunlu işi clean CMake/Qt toolchain kurmak ve `ctest` çalıştırmaktır.

**Karar özeti**

- PDF4QT v1.6.0.0 permissive ürün için birincil Gate 1 adayı.
- KillerPDF v1.8.2 yalnız GPLv3 rota seçilirse aday.
- PDFium + qpdf + Tesseract, PDF4QT elenirse yedek motor rotası.
- Open PDF Studio/Stirling current code ürün tabanı değil; reference/watch.
- Kod importu ADR-0001/0002 kabulüne kadar bloke.

**Oluşturulan proje hafızası**

Kök sözleşme/plan/status/sources; uzman mimari, lisans, güvenlik, kalite, requirements, support matrix, ADR, risk, release, playbook ve AI skill dosyaları.

**Doğrulama ve depo kurulumu — 2026-09-01**

- `pdf-dependency-audit` skill'i skill-creator `quick_validate.py` ile doğrulandı: `Skill is valid!`.
- Kaynak sicili ve corpus manifesti PyYAML ile parse edildi; final kontrolde 48 insan indeksi kimliği, 12 karar-kritik registry kaydı ve 3 bağlı kalıcı değerlendirme tutarlı bulundu.
- Final 50 Markdown dosyasındaki yerel linkler otomatik tarandı; kırık link bulunmadı.
- 51 requirement ID benzersiz bulundu.
- `git init -b main` exit 0; yerel depo başlatıldı, bilinçli olarak commit atılmadı.

**Bağımsız QA — 2026-09-01**

P0 bulgu yoktu. Düzeltilen başlıca bulgular: source ID çakışması ve exact-revision license URL'leri; haricî katkı lisansının Gate 0'a çekilmesi; signature trust/key için genel worker ağı yerine kısıtlı broker; kalıcı source-evaluation yolu; PDF/UA-1 ve UA-2 kanıt paketlerinin ayrılması; Gate 1 feasibility ile M1 production ayrımı; task kayıt kapsamı ve changelog semantiği.

**Exact upstream link/tag kontrolü**

- PDF4QT `v1.6.0.0`, KillerPDF `v1.8.2`, Open PDF Studio `v1.92.0`, Stirling `v2.14.3`, qpdf `v12.4.1` ve PDF.js `v6.3.289` tag/commit'leri `git ls-remote` ile doğrulandı.
- Sekiz karar-kritik exact-revision LICENSE yolu `curl -L` ile HTTP 200 döndürdü.
- QA tekrar kontrolünde bulunan iki 404 (Open PDF Studio nested `package.json`, pdfme `LICENSE.md`) düzeltildi ve HTTP 200 doğrulandı. Kalan P0/P1 bulgu yok.

## 2026-09-01 — Vendor bağımsız plan devamlılığı

**Amaç:** Yeni Gemini/Codex/Claude veya başka bir yerel ajan sohbetinin konuşma geçmişine güvenmeden doğru fazı ve sıradaki işi bulabilmesi.

**Uygulanan protokol**

- `STATUS.md` tek birincil aktif task bağlantısını taşır.
- Aktif task'ın `Devam checkpoint'i` son doğrulanmış adımı, sıradaki tek eylemi, insan blokajını ve çalışma ağacı bilgisini taşır.
- `AGENTS.md`, `devam`/`plana devam et` komutunun okuma, uzlaştırma, çalışma ve handoff sırasını tanımlar.
- `docs/AI_START_HERE.md`, ilk Gemini promptu ile yeni sohbet ve model değiştirme yönergelerini içerir.
- `GEMINI.md`, `CLAUDE.md` ve Copilot adapter'ları ürün gerçeğini kopyalamadan kök sözleşmeye yönlendirir.

**Doğrulama**

```text
PowerShell Markdown/işaretçi taraması
exit 0
Markdown files checked: 52
Broken local links: 0
STATUS active task pointer: OK
Active task checkpoint: OK
GEMINI.md -> AGENTS.md import: OK
```

Depo hâlâ ilk commit'e sahip değildir; plan dosyaları untracked durumdadır. Bu durum aktif task checkpoint'inde görünür bırakıldı ve ilk insan kararından sonra incelenmiş başlangıç commit'i Gate 0 kabul kriterine eklendi.
