# ADR-0004: Ücretsiz çekirdek, diagnostics ve sürdürülebilirlik

- **Durum:** Accepted
- **Tarih:** 2026-09-01
- **Karar sahibi:** Product owner
- **İlgili riskler:** R-014, R-018, R-020
- **Supersedes:** Yok

## Bağlam

“Ücretsiz PDF aracı” hedefi yalnız indirme fiyatını değil; hesap zorunluluğu, reklam, belge verisinin kullanımı, bakım maliyeti ve gelecekteki gelir baskısını da açıklamalıdır. Belge içeriği hassastır ve telemetri kolayca veri sızıntısına dönüşebilir.

## Karar

- Resmî masaüstü çekirdeği ve roadmap'teki temel PDF yetenekleri ücretsiz kalır; hesap veya abonelik temel viewer/edit/form/OCR kullanımının önkoşulu olmaz.
- Resmî çekirdek root MIT altında açık kaynak yayımlanır.
- Reklam, kullanıcı belgesi/metni/form verisi satışı ve belge içeriğinden profil çıkarma yoktur.
- İlk stable'a kadar otomatik diagnostics/telemetry upload yoktur. Yalnız yerel, içeriksiz ve sanitize edilmiş loglar tutulur; kullanıcı açıkça seçerse destek için manuel export eder.
- Crash dump varsayılan gönderilmez ve PDF byte'ı, OCR metni, parola, form verisi, tam hassas path veya key içeremez.
- Sürdürülebilirlik sırası: bağış/sponsorluk, ücretli destek ve entegrasyon, OEM/kurumsal hizmet. Ücretli veya dış servisli özellik çekirdeği gizlice ağ bağımlı yapamaz; ayrı ADR, privacy/data-flow ve lisans incelemesi ister.
- Bu politikanın değişmesi yeni Accepted ADR ve product-owner onayı gerektirir; Gemini veya başka bir ajan gelir gerekçesiyle tek başına değiştiremez.

## Sonuçlar

- İlk sürümlerde telemetry backend'i ve kullanıcı hesabı geliştirilmez.
- Hata raporları kullanıcı tarafından incelenebilir, redakte edilebilir küçük diagnostic bundle üretir.
- Yerel AI/OCR temel ürünün çevrimdışı çalışma ilkesini korur; dış AI provider ancak ayrı, per-task açık rıza ile ileride değerlendirilebilir.
- Code-signing, Store hesabı ve bakım maliyeti ücretsiz ürünün gerçek proje gideri olarak risk sicilinde izlenir.

## Doğrulama

- Network-deny E2E temel iş akışlarını geçirir.
- Log/dump scanner yasak veri sınıflarını fixture'larla test eder.
- Release privacy checklist'inde ağ endpoint'i, diagnostics ve account gereksinimi denetlenir.
