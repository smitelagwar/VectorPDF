# SRC-APP-003 — Open PDF Studio v1.92.0

- **Karar:** WATCH / REJECT-CURRENT
- **Canonical:** https://github.com/OpenAEC-Foundation/open-pdf-studio
- **Release:** `v1.92.0`
- **Commit:** `8919c8efaacffe43c5e59f2270ba9f038eb1ea17`
- **İnceleme:** 2026-08-31
- **Yeniden değerlendirme:** Lisans/provenance upstream tarafından netleşir ve yeterli bakım/security geçmişi oluşursa

## DOĞRULANDI

- Exact release [root LICENSE.md](https://github.com/OpenAEC-Foundation/open-pdf-studio/blob/8919c8efaacffe43c5e59f2270ba9f038eb1ea17/LICENSE.md) LGPL-3.0 metnidir.
- Aynı release [package.json](https://github.com/OpenAEC-Foundation/open-pdf-studio/blob/8919c8efaacffe43c5e59f2270ba9f038eb1ea17/open-pdf-studio/package.json) manifesti ve Rust manifestlerinin lisans beyanı kök kapsamıyla ayrıca uzlaştırılmalıdır.
- `package.json` doğrudan `mupdf` paketi kullanır; MuPDF açık dağıtım yolu AGPL-3.0 veya ticari lisanstır. Exact transitive/use scope hukuk ve provenance incelemesi olmadan LGPL ürün beyanıyla uyumlu sayılmaz.
- Proje Ocak 2026'da oluşturulmuş; yüksek release numarası saha olgunluğu kanıtı değildir.

## PROJE İDDİASI

Tauri 2/Rust/SolidJS, multi-process PDFium, PDF.js text layer, geniş edit/form/XFA/redaction/measurement/page/compare özellikleri ve render regression testleri.

## Değerli referans

- PDFium'u ayrı sidecar process'e alma
- render regression CI
- modern desktop/web UI task breakdown
- AEC measurement/compare workflow fikirleri

## Blokaj

Kök LGPL, manifest beyanları ve doğrudan AGPL MuPDF kullanımının exact kapsamı çelişkili/belirsizdir. Proje ürün baseline'ı veya kod kaynağı değildir; mimari fikirler bağımsız uygulanabilir. Lisans sorunu çözülse bile young-project/bus-factor/corpus/security Gate 1 gerekir.
