# OdysseyTogether

OdysseyTogether is a shared-screen, two-player mod for [Super Mario Odyssey](https://supermario.nintendo.com/). It builds on [Gudui’s Odyssey Local Co-op](https://github.com/Gudui/OdysseyLocalCoop).

## Download & install

**Current development version: v0.1.0-alpha.7 — Super Mario Odyssey 1.4.1 on Ryujinx.** Download the newest available prerelease from [GitHub Releases](https://github.com/The-Power-0/OdysseyLocalCoop/releases).

1. Configure two controllers in Ryujinx and confirm Odyssey is on **1.4.1**.
2. Right-click the game and choose **Open Mods Directory**.
3. Back up saves and disable/remove previous co-op mods, including the separate `OCoop Guide` entry when upgrading to alpha.7.
4. Extract the release ZIP so the directory contains `OdysseyTogether/exefs` and `OdysseyTogether/romfs`.
5. Enter a stage and select **Two Players** from the pause menu.

## What's new in alpha.7

- Cap Kingdom Spark Pylon recovery protection: a P2-led crossing to Cascade succeeded in Ryujinx testing.
- Integrated English **Local Co-op** confirmation: two full characters, two controllers, no separate guide patch required.
- Improved shared camera framing and faster off-screen recovery, carried forward from alpha.6.
- Hidden Joy-Con startup guide, independent controls and optional development Test Lab.

**Alpha status:** P1-led and later pylon crossings, integrated guide display, and broader story progression still need gameplay verification. Back up saves and avoid switching player modes between defeating Topper and reaching Cascade. The Metro, Lake and Lost Kingdom blockers remain open.

[Release notes](RELEASE_NOTES.md) · [Issue tracker](https://github.com/The-Power-0/OdysseyLocalCoop/issues) · [Test Lab](docs/P0_TEST_LAB.md)

## Roadmap

We will prioritize a dependable two-player campaign before optional multiplayer expansion. The table describes **plans and gates**, not promises of shipped features.

| Phase | Focus | Exit gate | Tracking |
| :-- | :-- | :-- | :-- |
| 1 — Foundation | Reproducible builds, tests, code safety, small opt-in improvements | Build verified **and** in-game regression checks | [PR #1](https://github.com/The-Power-0/OdysseyLocalCoop/pull/1), [#42](https://github.com/The-Power-0/OdysseyLocalCoop/issues/42), [#43](https://github.com/The-Power-0/OdysseyLocalCoop/issues/43) |
| 2 — Story stability | Metro/Lake softlocks, lava recovery, unsafe respawns | No known main-story progression blockers | [#2](https://github.com/The-Power-0/OdysseyLocalCoop/issues/2), [#3](https://github.com/The-Power-0/OdysseyLocalCoop/issues/3), [#4](https://github.com/The-Power-0/OdysseyLocalCoop/issues/4), [#5](https://github.com/The-Power-0/OdysseyLocalCoop/issues/5), [#6](https://github.com/The-Power-0/OdysseyLocalCoop/issues/6), [#7](https://github.com/The-Power-0/OdysseyLocalCoop/issues/7) |
| 3 — Gameplay completeness | Health, captures, bosses, vehicles, cinematics, ownership | Both players can complete verified story interactions reliably | [#8](https://github.com/The-Power-0/OdysseyLocalCoop/issues/8), [#9](https://github.com/The-Power-0/OdysseyLocalCoop/issues/9), [#10](https://github.com/The-Power-0/OdysseyLocalCoop/issues/10), [#15](https://github.com/The-Power-0/OdysseyLocalCoop/issues/15), [#20](https://github.com/The-Power-0/OdysseyLocalCoop/issues/20), [#21](https://github.com/The-Power-0/OdysseyLocalCoop/issues/21) and [all issues](https://github.com/The-Power-0/OdysseyLocalCoop/issues), [#47](https://github.com/The-Power-0/OdysseyLocalCoop/issues/47), [#48](https://github.com/The-Power-0/OdysseyLocalCoop/issues/48) |
| 4 — Polish & compatibility | Camera, HUD, menus, controller experience, 1.3.0 evaluation | Separately tested feature builds and clear version support | [#22](https://github.com/The-Power-0/OdysseyLocalCoop/issues/22), [#24](https://github.com/The-Power-0/OdysseyLocalCoop/issues/24), [#33](https://github.com/The-Power-0/OdysseyLocalCoop/issues/33), [#36](https://github.com/The-Power-0/OdysseyLocalCoop/issues/36), [#44](https://github.com/The-Power-0/OdysseyLocalCoop/issues/44) |
| 5 — Exploratory | Four-player, dynamic split-screen, online-co-op feasibility | Only graduate after prototypes pass real gameplay/performance testing | [#34](https://github.com/The-Power-0/OdysseyLocalCoop/issues/34), [#35](https://github.com/The-Power-0/OdysseyLocalCoop/issues/35), [#37](https://github.com/The-Power-0/OdysseyLocalCoop/issues/37) |

See **[PROGRESS.md](PROGRESS.md)** for the dated workstream ledger, status definitions, and validation requirements. All individual proposals, blockers, and acceptance criteria are in [GitHub Issues](https://github.com/The-Power-0/OdysseyLocalCoop/issues).

### Tracking rules

1. **Issues define scope and acceptance criteria.** Issue titles use `[P0]`–`[P4]` priority prefixes; `bug` and `enhancement` labels describe work type. `P0` means critical, and `P4` denotes research.
2. **Pull requests contain code and review evidence.** A PR can have passing CI while remaining a draft until gameplay testing is documented.
3. **Build verified ≠ gameplay verified ≠ released.** Mark each separately; no feature is described as supported merely because its source compiles.
4. **Close issues only when acceptance criteria are met.** Link the merged PR, testing platform/version, and known limitations. Update [PROGRESS.md](PROGRESS.md) at milestones rather than duplicating every issue here.

## Compatibility & safety

| Target | Status |
| :-- | :-- |
| Super Mario Odyssey **1.4.1** | Current OdysseyTogether prerelease target; tested on Ryujinx |
| Super Mario Odyssey **1.0.0** | Inherited upstream baseline; not the current release target |
| Ryujinx | Primary tested platform; full campaign verification ongoing |
| Atmosphere / physical Switch | **Experimental upstream** (limited physical testing); no new fork hardware claims |
| Odyssey **1.3.0** | Not a supported release target; [planned investigation #36](https://github.com/The-Power-0/OdysseyLocalCoop/issues/36) |
| 3–4 players | **Not supported**; [future investigation #34](https://github.com/The-Power-0/OdysseyLocalCoop/issues/34) |

Back up your saves before experimenting. The upstream [known issues](KNOWN_ISSUES.md) include Metro Kingdom's post-boss white screen and Lake Kingdom's painting transition loop, as well as severe Lost Kingdom lava recovery cases. Avoid assuming that anything in a draft branch fixes those defects.

## Building from source

Clone **this fork** with the pinned ExLaunch submodule:

```powershell
git clone --recurse-submodules https://github.com/The-Power-0/OdysseyLocalCoop.git
Set-Location OdysseyLocalCoop
```

Build locally using devkitPro, or use the pinned devkitPro container:

```powershell
.\scripts\build.ps1 -DevkitProRoot PATH_TO_DEVKITPRO -GameVersion 1.4.1
# OR:
.\scripts\build.ps1 -UseDocker -GameVersion 1.4.1

.\scripts\package.ps1
.\scripts\verify.ps1
```

Outputs are generated under `artifacts/`. The build requires the repository's pinned ExLaunch commit; the original [build workflow](.github/workflows/build.yml) compiles, packages, verifies and uploads a **development artifact**, not a gameplay certification. Use the appropriate development branch checkout when reviewing an unmerged PR.

<details>
<summary><strong>How the original mod works (technical overview)</strong></summary>

The mod builds an [ExLaunch](https://github.com/shadowninja108/exlaunch) `subsdk9` module; the current release target is Odyssey 1.4.1. During a stage initialization window, it copies the native P1 player-construction data while that data is valid, constructs a second player actor with a separate controller and costume, and registers that actor with the stage's native player holder.

The shared camera layers midpoint targeting, distance-based zoom and right-stick arbitration over Odyssey's camera systems instead of replacing them. Bubble recovery reuses native recovery animations, monitors player separation, chooses a partner destination and retargets recovery while the partner moves. Other scoped hooks adapt doors, 2D valves, cap ownership, captures and scene-owned P2 health layouts.

The main remaining challenge is that multiple native game systems assume a single player at index zero. Changes must correct those assumptions **only in their relevant context** to avoid regressing unrelated gameplay. See the original [CONTRIBUTING.md](CONTRIBUTING.md) for requirements around verified hook offsets and private reverse-engineering material.

</details>

## Contributing and credits

Start with [open issues](https://github.com/The-Power-0/OdysseyLocalCoop/issues), read [CONTRIBUTING.md](CONTRIBUTING.md) and [PROGRESS.md](PROGRESS.md), and keep changes scoped to one behavior. Please provide sanitized reproduction steps and clearly label emulator tests, hardware tests, and unverified hypotheses.

**Original author and project:** [Gudui/OdysseyLocalCoop](https://github.com/Gudui/OdysseyLocalCoop). This independent fork retains the upstream project history, [credits](CREDITS.md), [third-party notices](THIRD_PARTY_NOTICES.md), and [GPL-2.0-only license](LICENSE). The fork's additional work does not imply upstream endorsement.

Super Mario Odyssey and its assets are copyright Nintendo. This unofficial fan project is not affiliated with or endorsed by Nintendo. Do not submit copyrighted game executables, ROM dumps, keys, firmware or private game assets.

