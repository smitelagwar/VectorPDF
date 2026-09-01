# Architecture Decision Records

ADR, uzun ömürlü ve maliyetli kararların “neden”ini korur. Görev notu veya toplantı özeti değildir.

## Durumlar

- `Proposed`: Değerlendirme/insan kararı bekler.
- `Accepted`: Uygulamada bağlayıcıdır.
- `Rejected`: Değerlendirildi, seçilmedi.
- `Superseded by ADR-NNNN`: Tarihsel kayıt korunur; yeni karar geçerlidir.
- `Deprecated`: Artık uygulanmıyor, yerine karar gelmesi gerekebilir.

Accepted ADR'nin karar/gerekçesi geriye dönük düzeltilmez. Yeni bilgi kararı değiştiriyorsa yeni ADR yazılır.

## ADR gerektiren konular

- Ürün/fork lisansı ve baseline
- UI/runtime/PDF engine değişimi
- Document transaction, serialization, revision ve save modeli
- Sandbox, network, privacy, telemetry
- Kriptografi/imza/redaction
- Updater/signing/plugin mimarisi
- Desteklenen standard profili ve conformance iddiası
- Kalıcı veri formatı veya migration

Şablon: [../templates/ADR.md](../templates/ADR.md).

## İndeks

| ADR | Başlık | Durum |
|---|---|---|
| [0001](0001-product-license-model.md) | Ürün lisansı ve iş modeli | Accepted |
| [0002](0002-upstream-baseline.md) | Upstream baseline kabul süreci | Proposed — Gate 1 kanıtı gerekli |
| [0003](0003-supported-windows-and-distribution.md) | Desteklenen Windows ve ilk dağıtım kanalı | Accepted |
| [0004](0004-free-core-diagnostics-and-sustainability.md) | Ücretsiz çekirdek, diagnostics ve sürdürülebilirlik | Accepted |
