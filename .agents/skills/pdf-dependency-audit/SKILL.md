---
name: pdf-dependency-audit
description: Audit any proposed GitHub fork, PDF engine, library, CLI, DLL, runtime, model, font, asset, test corpus, or copied source before it is added, updated, vendored, bundled, or distributed. Use for dependency upgrades, upstream syncs, license questions, and adopting external PDF code.
---

# PDF Dependency Audit

Do not import, copy, vendor, install into the product, or distribute the candidate until this audit reaches an approved outcome.

## Read first

- `docs/LICENSING.md`
- `docs/OPEN_SOURCE_STRATEGY.md`
- `docs/playbooks/ADD_DEPENDENCY.md`
- `SOURCES.md`
- Relevant accepted ADRs

## Audit workflow

1. State the exact need, scope, and why an accepted dependency cannot satisfy it.
2. Resolve the canonical upstream and exact tag/commit. For a binary/model/font/data artifact, record its SHA-256 and source-to-artifact relationship.
3. Inspect LICENSE/NOTICE/COPYING at that exact revision. Inspect nested directories, vendored code, package manifests, submodules, generated files, assets, models, fonts, test data, and prebuilt binaries. Never infer the whole tree's license from a badge or root file.
4. Classify every item as runtime, build, development, data, model, font, asset, or reference. Resolve direct and transitive dependencies.
5. Record SPDX choice and obligations: attribution/NOTICE, source offer, relinking, covered-file source, patent, trademark, network copyleft, redistribution, export/crypto, and license exceptions. Treat uncertainty as blocked; do not provide a legal conclusion.
6. Inspect maintenance and security: canonical advisories, supported releases, maintainer concentration, CI that actually runs tests, fuzz/corpus history, Windows build/package path, reproducibility, and default network/privilege behavior.
7. Run a bounded clean PoC when relevant. Record environment, exact commands, exit codes, test counts, artifact hashes, failures, performance, and removal/replacement cost. An upstream claim is not local evidence.
8. Compare at least one alternative, including keeping the current solution.
9. Copy the structure of `docs/templates/SOURCE_EVALUATION.md` into a permanent `docs/sources/evaluations/<SOURCE-ID>-<slug>.md` record; never edit the template as the audit result. Update `SOURCES.md` and `docs/sources/SOURCE_REGISTRY.yml`, including `evaluation_path`. Open an ADR for baseline, architecture, protected-zone, or hard-to-reverse choices.
10. Return one outcome: `ADOPT-CANDIDATE`, `COMPONENT-CANDIDATE`, `REFERENCE`, `WATCH`, or `REJECT-CURRENT`, with conditions, owner, review date, and re-evaluation trigger.

## Import gate

Only after human/required approval:

- pin version/commit/digest and verify checksums;
- add lockfile/SBOM/notices/license texts/source or relink material;
- isolate behind the narrowest adapter/worker;
- add failure, security, preservation, and upgrade regression tests;
- record the accepted ADR/source ID and rollback path.

## Immediate blockers

- No license or conflicting scope
- Exact revision/artifact cannot be identified
- Required distribution right is absent
- GPL/AGPL/source-available code conflicts with the accepted product route
- Default network/credential/document access cannot be constrained
- Clean build or artifact provenance cannot be reproduced
- A duplicate engine adds mutation ambiguity without a unique need

## Output format

Lead with the outcome and blocking facts. Separate `DOĞRULANDI`, `YERELDE KANITLANDI`, `PROJE İDDİASI`, `ÖNERİ`, and `BİLİNMİYOR`. Include direct primary URLs and verification dates. Never state that a license is legally safe; state technical findings and required legal review.
