# ADR-0003: Desteklenen Windows ve ilk dağıtım kanalı

- **Durum:** Accepted
- **Tarih:** 2026-09-01
- **Karar sahibi:** Product owner
- **İlgili riskler:** R-003, R-013, R-016, R-017
- **Supersedes:** Yok

## Bağlam

Her Windows sürümünü desteklemek build/test maliyetini ve düşmanca PDF'lerde güvenlik riskini büyütür. Windows 10 genel desteği 14 Ekim 2025'te sona ermiştir. Windows 11 sürümleri Microsoft Modern Lifecycle kapsamında farklı tarihlerde destekten çıkar; sabit bir build numarasını süresiz desteklemek doğru değildir.

## Karar

- Ürünün ilk resmî hedefi **Microsoft tarafından release tarihinde hâlâ desteklenen Windows 11 sürümleri, x64**'tür.
- Release testi en az destek penceresindeki en eski hedef x64 Windows 11 sürümü ile en yeni genel kullanıma açık x64 sürümü kapsar. Exact VM/build numaraları release kaydına yazılır.
- Windows 10 desteklenmez, CI/release matrisi ve güvenlik taahhüdü verilmez. Uygulamanın tesadüfen çalışması destek iddiası değildir.
- ARM64 `DEFERRED` durumundadır; native/emulation build, installer, performans ve worker-isolation kanıtı olmadan destekleniyor denmez.
- İlk dağıtım kanalı **Microsoft Store üzerinden MSIX**'tir. Store hesabı/ülke/ücret şartları release hazırlığında tekrar doğrulanır.
- Doğrudan indirme ancak imzalama, timestamp, update/rollback ve support maliyeti için ayrı kabul kanıtı ve gerekiyorsa yeni ADR ile açılır. İlk sürümde özel updater yazılmaz.
- Store/MSIX paketleme parser sandbox'ı sayılmaz; worker isolation ayrı güvenlik gereksinimidir.

## Sonuçlar

- Gate 1 clean build/test Windows 11 x64 üzerinde yürür.
- Minimum işletim sistemi ve package identity M0'da nihai ürün adıyla sabitlenir.
- Windows 10 talebi ileride ayrı, ölçülmüş compatibility pack veya unsupported community build olarak değerlendirilebilir; resmî stable kapsamına sessizce eklenmez.
- Windows/architecture desteğini değiştirmek yeni ADR gerektirir.

## Doğrulama

- Release başlangıcında Microsoft supported-versions sayfası kontrol edilir.
- VM matrisi exact edition/version/build ve patch tarihiyle kaydedilir.
- Install/open/update/uninstall, standard-user, high-DPI, Turkish locale ve accessibility smoke sonuçları tutulur.

## Kaynaklar

- [Microsoft — Supported versions of Windows client](https://learn.microsoft.com/en-us/windows/release-health/supported-versions-windows-client)
- [Microsoft — Windows 11 Home and Pro lifecycle](https://learn.microsoft.com/en-us/lifecycle/products/windows-11-home-and-pro)
- [Microsoft — Windows 10 support ended 14 October 2025](https://support.microsoft.com/en-US/Windows/Deployment/Updates-Lifecycle/windows-10-support-has-ended-on-october-14-2025)
- [Microsoft — Windows app distribution paths](https://learn.microsoft.com/en-us/windows/apps/package-and-deploy/choose-distribution-path)
