# Katkı Rehberi

Bu depo [MIT](LICENSE) lisanslıdır. Haricî katkı açıldığında katkılar aynı MIT lisansı altında ve [Developer Certificate of Origin 1.1](DCO) sign-off'u ile kabul edilir; başlangıçta CLA kullanılmaz.

**M0'da review sahipliği, DCO kontrolü ve contribution provenance hattı kurulana kadar haricî patch, belge, çeviri, asset, corpus veya kod katkısı kapalıdır.** Haricî öneriler kod/asset eklemeden issue/tartışma olarak paylaşılabilir. Katkı kanalının kapalı olması lisans kararsızlığı değil, intake güvenlik kapısıdır.

Katkı kanalı açıldığında her commit şu sign-off'u taşır:

```text
Signed-off-by: Ad Soyad <e-posta>
```

`git commit -s` bunu ekleyebilir. Sign-off, katkıcının katkıyı gönderme hakkını [DCO 1.1](https://developercertificate.org/) koşullarıyla beyan eder; telif devri değildir. AI yardımlı katkıda da katkıcı üretilen kodun kaynağını/lisansını incelemekten ve gönderme hakkından sorumludur.

## Değişiklik öncesi

- Kök `AGENTS.md` ve `STATUS.md` dosyalarını okuyun.
- Bir görev şablonu açın: `docs/templates/TASK.md`.
- Gereksinim kimliği, kapsam dışı maddeler ve ölçülebilir kabul kriterlerini yazın.
- Dış bağımlılık ekliyorsanız `pdf-dependency-audit` becerisini ve `docs/playbooks/ADD_DEPENDENCY.md` dosyasını uygulayın.

## Pull request kalite sözleşmesi

- Problem ve kullanıcı etkisi açık olmalı.
- İlgili REQ, ADR ve SOURCE kimlikleri bağlanmalı.
- Güvenlik, gizlilik, erişilebilirlik, performans ve lisans etkisi “yok” deniyorsa gerekçe verilmeli.
- Gerçekten çalıştırılan komutlar, exit code ve kısa sonuç eklenmeli.
- PDF değişikliği için save → reopen → validate; görsel değişiklik için kontrollü render diff; korunması gereken yapı için semantic diff kanıtı bulunmalı.
- Golden baseline, güvenlik kotası, lisans kararı veya conformance iddiası tek onayla sessizce değiştirilemez.
- Kullanıcı belgesi, parola, özel anahtar veya gizli içerik fixture/log olarak commit edilmez.

## Commit ve geçmiş

- Küçük, tek amaçlı commit'ler tercih edilir.
- `CHANGELOG.md` yalnız kullanıcı davranışı değiştiğinde güncellenir.
- Önemli kararın geçmişi commit mesajına bırakılmaz; ADR yazılır.
- Kabul edilmiş ADR geriye dönük yeniden yazılmaz; yeni ADR eskisini supersede eder.
- Katkı kanalı açıldığında bütün commit'ler DCO sign-off taşımalıdır; bot geçişi insan review'ının yerine geçmez.

## VectorPDF Derleme ve Test Komutları (Windows 11 x64)

Gereksinimler: MSVC 2022 v143 (C++20), CMake 3.25+, Qt 6.9+ (Core, Gui, Widgets, Svg, Xml, PrintSupport, TextToSpeech, Concurrent) ve vcpkg.

```powershell
# 1. vcpkg bağımlılıklarını yükleyin:
vcpkg install tbb openssl lcms zlib openjpeg freetype libjpeg-turbo libpng blend2d --triplet x64-windows

# 2. CMake ile yapılandırın:
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake" -DPDF4QT_QT_ROOT="$env:Qt6_DIR" -DPDF4QT_BUILD_TESTS=ON

# 3. Projeyi derleyin:
cmake --build build --config Release -j8

# 4. Testleri çalıştırın:
ctest --test-dir build -C Release --output-on-failure
```

