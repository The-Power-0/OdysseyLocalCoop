# Development progress

**Maintained milestone ledger for [The-Power-0/OdysseyLocalCoop](https://github.com/The-Power-0/OdysseyLocalCoop).**

Back to [README](README.md) · [All issues](https://github.com/The-Power-0/OdysseyLocalCoop/issues) · [Open pull requests](https://github.com/The-Power-0/OdysseyLocalCoop/pulls) · [GitHub Actions](https://github.com/The-Power-0/OdysseyLocalCoop/actions)

> [!NOTE]
> The tables below are a **dated snapshot**, not a live automated dashboard. GitHub Issues and pull requests are authoritative for current status. Update this document when a milestone changes, rather than copying every individual issue's comments here.

**Snapshot:** November 20, 2026 (America/Indiana/Indianapolis)  
**Starting baseline:** Gudui upstream `v0.1.0-alpha.5` source, Super Mario Odyssey `1.0.0`.  
**Tracked backlog at snapshot:** **47 open issues** (`#2`–`#48`): 20 labeled `bug`, 27 labeled `enhancement`. Priorities in titles: P0 × 4, P1 × 29, P2 × 10, P3 × 3, P4 × 1.  
**Fork releases:** none. **Merged fork-specific gameplay commits on `main`:** none.

## Current milestone board

| Workstream | Issues / PRs | State | Next evidence required |
| :-- | :-- | :-- | :-- |
| Foundation and first opt-in improvement | [PR #1](https://github.com/The-Power-0/OdysseyLocalCoop/pull/1), [#46](https://github.com/The-Power-0/OdysseyLocalCoop/issues/46) | **Build verified; gameplay pending** | Two-controller Ryujinx test at offset 0 and 120, scene transitions, negative offset, recovery |
| Metro and Lake progression blockers | [#2](https://github.com/The-Power-0/OdysseyLocalCoop/issues/2), [#3](https://github.com/The-Power-0/OdysseyLocalCoop/issues/3) | **Open / not fixed** | Reliable reproductions, traces, then verified scene transitions and persisted progress |
| Severe death / recovery failures | [#4](https://github.com/The-Power-0/OdysseyLocalCoop/issues/4), [#5](https://github.com/The-Power-0/OdysseyLocalCoop/issues/5), [#6](https://github.com/The-Power-0/OdysseyLocalCoop/issues/6), [#7](https://github.com/The-Power-0/OdysseyLocalCoop/issues/7) | **Open / not fixed** | Safe recovery state decisions and tested multi-player lava/abyss cases |
| Independent health, capture and events | [#8](https://github.com/The-Power-0/OdysseyLocalCoop/issues/8), [#9](https://github.com/The-Power-0/OdysseyLocalCoop/issues/9), [#10](https://github.com/The-Power-0/OdysseyLocalCoop/issues/10), [#15](https://github.com/The-Power-0/OdysseyLocalCoop/issues/15), [#20](https://github.com/The-Power-0/OdysseyLocalCoop/issues/20), [#21](https://github.com/The-Power-0/OdysseyLocalCoop/issues/21), [#48](https://github.com/The-Power-0/OdysseyLocalCoop/issues/48) | **Backlog** | Player-aware interaction tests and validated fixes |
| Core technical architecture | [#38](https://github.com/The-Power-0/OdysseyLocalCoop/issues/38), [#39](https://github.com/The-Power-0/OdysseyLocalCoop/issues/39), [#40](https://github.com/The-Power-0/OdysseyLocalCoop/issues/40), [#41](https://github.com/The-Power-0/OdysseyLocalCoop/issues/41) | **Backlog** | Reviewable, bounded refactors with unchanged binary behavior |
| Regression, CI, release gates | [#42](https://github.com/The-Power-0/OdysseyLocalCoop/issues/42), [#43](https://github.com/The-Power-0/OdysseyLocalCoop/issues/43), [#45](https://github.com/The-Power-0/OdysseyLocalCoop/issues/45) | **Partial foundation** | Kingdom matrix, automated regression checks, explicit acceptance evidence |
| In-game polish | [#22](https://github.com/The-Power-0/OdysseyLocalCoop/issues/22), [#23](https://github.com/The-Power-0/OdysseyLocalCoop/issues/23), [#24](https://github.com/The-Power-0/OdysseyLocalCoop/issues/24), [#25](https://github.com/The-Power-0/OdysseyLocalCoop/issues/25), [#26](https://github.com/The-Power-0/OdysseyLocalCoop/issues/26), [#27](https://github.com/The-Power-0/OdysseyLocalCoop/issues/27), [#28](https://github.com/The-Power-0/OdysseyLocalCoop/issues/28), [#29](https://github.com/The-Power-0/OdysseyLocalCoop/issues/29), [#30](https://github.com/The-Power-0/OdysseyLocalCoop/issues/30), [#31](https://github.com/The-Power-0/OdysseyLocalCoop/issues/31), [#32](https://github.com/The-Power-0/OdysseyLocalCoop/issues/32), [#33](https://github.com/The-Power-0/OdysseyLocalCoop/issues/33) | **Backlog** | Individual UX changes after story stabilization |
| Odyssey 1.3.0 and hook-version safety | [#36](https://github.com/The-Power-0/OdysseyLocalCoop/issues/36), [#44](https://github.com/The-Power-0/OdysseyLocalCoop/issues/44) | **Not supported** | Fully audited version-specific offsets and game testing |
| 3–4 players, split-screen, online research | [#34](https://github.com/The-Power-0/OdysseyLocalCoop/issues/34), [#35](https://github.com/The-Power-0/OdysseyLocalCoop/issues/35), [#37](https://github.com/The-Power-0/OdysseyLocalCoop/issues/37) | **Research/backlog** | Standalone feasibility, performance and compatibility evaluations |

## Phase 1: evidence

**Draft PR:** [#1 — P2 spawn spacing and host tests](https://github.com/The-Power-0/OdysseyLocalCoop/pull/1)  
**Head commit:** [`a6ccc31`](https://github.com/The-Power-0/OdysseyLocalCoop/commit/a6ccc3161812977b4299eed17ced0598ecbfca5b)

- [x] Created independent GitHub fork and development branch.
- [x] Added an opt-in P2 spawn offset; default `0` preserves the original setting.
- [x] Added SDK-independent C++ positioning tests and test workflow.
- [x] Host unit tests passed ([PR run](https://github.com/The-Power-0/OdysseyLocalCoop/actions/runs/44857762871)).
- [x] Pinned toolchain build, package, verification and artifact upload passed ([PR run](https://github.com/The-Power-0/OdysseyLocalCoop/actions/runs/44857762895)).
- [ ] Test in Ryujinx 1.0.0 with two configured controllers, zero and nonzero offsets, stage changes, pipes, captures and recovery.
- [ ] Document exact test results on [#46](https://github.com/The-Power-0/OdysseyLocalCoop/issues/46) and review PR #1.
- [ ] Merge only after acceptance criteria and regression evidence are satisfied.

## Issue priorities and state vocabulary

**Priorities (in issue titles):** `P0` critical blocker or severe corruption risk; `P1` important reliability/quality; `P2` useful planned work; `P3` optional exploratory features; `P4` long-horizon research. GitHub's `bug` and `enhancement` labels distinguish type, **not** completion.

**Work states:**
- **Backlog** — scoped but no implementation accepted.
- **In progress** — active investigation or implementation with linked issue/PR.
- **Build verified** — source compiled, tests and packaging passed, but not necessarily played successfully.
- **Gameplay verified** — defined emulator/hardware scenarios actually executed with recorded results.
- **Merged** — reviewed changes landed on `main`; does not automatically mean released.
- **Released** — tagged, packaged, documented build with explicit compatibility/limitations.

Use these terms rather than vague percent-complete claims. A CI artifact is never a release merely because the build passed.

## Definition of done

Before closing a gameplay issue or marking a milestone complete:

1. The issue's acceptance criteria are satisfied and linked to one or more reviewed PRs/commits.
2. Source compiles and packaging/verification and applicable host unit tests pass.
3. Gameplay testing records game version, emulator/hardware, controller configuration, kingdom, reproduction and results.
4. Both solo mode and co-op mode are regression-tested for affected systems.
5. README/`KNOWN_ISSUES.md`/changelog and compatibility claims are updated if behavior changed.
6. The issue is closed only after integration, and a release is advertised only with separately verified supported scenarios.

## Milestone update template

When a meaningful milestone changes, update the snapshot date and relevant row above, then append a compact entry:

```markdown
### YYYY-MM-DD — Short milestone
- **Change:** What merged, with PR/commit links
- **Tests:** Build / emulator / hardware, version, configuration and results
- **Remaining blockers:** Links to still-open issues
- **Next gate:** The concrete evidence required before release
```

## Update history

### 2026-11-20 — Issue tracker and first development baseline

- **Change:** Created separate backlog tickets [#2–#48](https://github.com/The-Power-0/OdysseyLocalCoop/issues), documented the independent fork, opened draft [PR #1](https://github.com/The-Power-0/OdysseyLocalCoop/pull/1).
- **Tests:** Phase 1 host tests and package verification passed on GitHub Actions. No claim of fork-specific in-game testing.
- **Remaining blockers:** [Progression #2–#3](https://github.com/The-Power-0/OdysseyLocalCoop/issues/2), [recovery #4–#7](https://github.com/The-Power-0/OdysseyLocalCoop/issues/4), and [PR #1 gameplay validation #46](https://github.com/The-Power-0/OdysseyLocalCoop/issues/46).
- **Next gate:** Review and record the first controlled Ryujinx playtest and establish the kingdom-by-kingdom test matrix.

