# Odyssey Local Co-op

**An independent, stability-first development fork of [Gudui's Odyssey Local Co-op](https://github.com/Gudui/OdysseyLocalCoop).** Our goal is a reliable, polished, full-player couch co-op experience for Super Mario Odyssey—starting with an end-to-end two-player story playthrough.

[![Two fully controllable Mario players in the Sand Kingdom](media/odyssey-local-coop.png)](https://vimeo.com/1211084414/41139bee79?share=copy&fl=sv&fe=ci)

*Original upstream gameplay showcase. The image is not a demonstration of additional features developed by this fork.*

[Development status](#development-status) · [Progress tracker](PROGRESS.md) · [Issues](https://github.com/The-Power-0/OdysseyLocalCoop/issues) · [Roadmap](#roadmap) · [Installation](#installation) · [Building](#building-from-source)

> [!IMPORTANT]
> **This is an experimental community fork, not a new released mod.** The `main` branch currently matches upstream `v0.1.0-alpha.5` source. Work in branches and pull requests is **not** included in `main` or an official fork release. The inherited playable target remains **Super Mario Odyssey 1.0.0**, with **two full players on one shared screen**. Four-player mode, Odyssey 1.3.0, split-screen, and online play are **not supported features of this fork**.

## Development status

| Area | Current state | Evidence / next step |
| :-- | :-- | :-- |
| Upstream baseline | **Inherited, early alpha** | [Original project](https://github.com/Gudui/OdysseyLocalCoop) · [upstream known issues](KNOWN_ISSUES.md) |
| Fork `main` | **Unmodified upstream snapshot** | No fork-specific gameplay changes merged |
| First fork change | **Draft / build verified, gameplay untested** | [PR #1: opt-in P2 spawn spacing](https://github.com/The-Power-0/OdysseyLocalCoop/pull/1) · [issue #46](https://github.com/The-Power-0/OdysseyLocalCoop/issues/46) |
| Automated validation for PR #1 | **Passed** | [Host tests](https://github.com/The-Power-0/OdysseyLocalCoop/actions/runs/44857762871) · [build, package and verification](https://github.com/The-Power-0/OdysseyLocalCoop/actions/runs/44857762895) |
| Story-wide co-op testing | **Not established for this fork** | [Kingdom test matrix #42](https://github.com/The-Power-0/OdysseyLocalCoop/issues/42) |
| Fork releases | **None published** | Do not mistake temporary CI artifacts for tested releases |

The original project's alpha is playable in tested scenarios, but has documented progression blockers and recovery bugs. A successful build or unit test **does not** establish that a gameplay fix works in Ryujinx or on physical Switch.

**Live tracking:** [Open issues](https://github.com/The-Power-0/OdysseyLocalCoop/issues?q=is%3Aissue%20is%3Aopen) · [Pull requests](https://github.com/The-Power-0/OdysseyLocalCoop/pulls) · [Actions](https://github.com/The-Power-0/OdysseyLocalCoop/actions) · [Progress ledger](PROGRESS.md)

## What already works in the upstream baseline

This fork inherits these features from Gudui's mod; they are **not new improvements made by this fork**:

- Two independent, fully controllable Mario actors using two separately configured controllers.
- Pause-menu `Two Players` / `One Player` actions that rebuild the stage to enter or leave full-player co-op.
- Configurable P2 costume and cap, using game-provided model names.
- A shared camera with midpoint tracking, separation-based zoom, and right-stick input from either player.
- Recovery bubbles for falls and excessive player separation in tested scenarios.
- P2 support for tested stage doors, 2D sections, and other player-aware interactions.
- Simultaneous captures and cap return ownership in tested paths.
- Separate P2 health tracking and a scene-owned life display.
- An optional bundled coin / Moon competition scoreboard (disabled by default because of known edge cases).

This is **local shared-screen co-op**, not the game's native Mario/Cappy separate-play mode.

## Roadmap

We will prioritize a dependable two-player campaign before optional multiplayer expansion. The table describes **plans and gates**, not promises of shipped features.

| Phase | Focus | Exit gate | Tracking |
| :-- | :-- | :-- | :-- |
| 1 — Foundation | Reproducible builds, tests, code safety, small opt-in improvements | Build verified **and** in-game regression checks | [PR #1](https://github.com/The-Power-0/OdysseyLocalCoop/pull/1), [#42](https://github.com/The-Power-0/OdysseyLocalCoop/issues/42), [#43](https://github.com/The-Power-0/OdysseyLocalCoop/issues/43) |
| 2 — Story stability | Metro/Lake softlocks, lava recovery, unsafe respawns | No known main-story progression blockers | [#2](https://github.com/The-Power-0/OdysseyLocalCoop/issues/2), [#3](https://github.com/The-Power-0/OdysseyLocalCoop/issues/3), [#4–#7](https://github.com/The-Power-0/OdysseyLocalCoop/issues/4) |
| 3 — Gameplay completeness | Health, captures, bosses, vehicles, cinematics, ownership | Both players can complete verified story interactions reliably | [#8–#21](https://github.com/The-Power-0/OdysseyLocalCoop/issues/8), [#47](https://github.com/The-Power-0/OdysseyLocalCoop/issues/47), [#48](https://github.com/The-Power-0/OdysseyLocalCoop/issues/48) |
| 4 — Polish & compatibility | Camera, HUD, menus, controller experience, 1.3.0 evaluation | Separately tested feature builds and clear version support | [#22–#36](https://github.com/The-Power-0/OdysseyLocalCoop/issues/22), [#44](https://github.com/The-Power-0/OdysseyLocalCoop/issues/44) |
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
| Super Mario Odyssey **1.0.0** | Upstream alpha target and only current baseline |
| Ryujinx | Primary upstream-tested emulator target; fork changes need separate testing |
| Atmosphere / physical Switch | **Experimental upstream** (limited physical testing); no new fork hardware claims |
| Odyssey **1.3.0** | **Not supported**; [planned investigation #36](https://github.com/The-Power-0/OdysseyLocalCoop/issues/36) |
| 3–4 players | **Not supported**; [future investigation #34](https://github.com/The-Power-0/OdysseyLocalCoop/issues/34) |

Back up your saves before experimenting. The upstream [known issues](KNOWN_ISSUES.md) include Metro Kingdom's post-boss white screen and Lake Kingdom's painting transition loop, as well as severe Lost Kingdom lava recovery cases. Avoid assuming that anything in a draft branch fixes those defects.

## Installation

**There is currently no published release from this fork.** For the existing upstream alpha, obtain the `OdysseyLocalCoop-v0.1.0-alpha.5.zip` and corresponding SHA-256 file from the [original author's releases](https://github.com/Gudui/OdysseyLocalCoop/releases/tag/v0.1.0-alpha.5). These instructions apply to that **upstream** package, not the experimental PR #1 artifact.

1. Use a legally obtained copy of **Super Mario Odyssey 1.0.0**, already configured in Ryujinx, with two separately configured controllers.
2. Download the upstream ZIP and its `.sha256` file, then check the ZIP hash:
   ```powershell
   Get-FileHash .\OdysseyLocalCoop-v0.1.0-alpha.5.zip -Algorithm SHA256
   ```
3. Extract the package. Inspect `install.ps1` before running it, then from that directory:
   ```powershell
   Unblock-File .\install.ps1
   .\install.ps1
   ```
4. Launch the game, enter a stage, select `Two Players` from the pause menu, and follow the controller prompt. Use `One Player` there to return to solo mode.

The installer verifies packaged files and backs up files it replaces at the installation paths. To install manually, preserve the title-ID/`exefs`/`romfs` folder structure under Ryujinx's per-title mod directory. Do not mix executable and HUD files from different builds.

<details>
<summary><strong>Configuration and troubleshooting</strong></summary>

### Settings

Edit the installed `contents/0100000000010000/romfs/OCoop/settings.ini` with Ryujinx closed. Settings reload when a new `StageScene` initializes.

| Key / prefix | Purpose |
| :-- | :-- |
| `camera.zoom.*` | Base/max zoom, separation thresholds and smoothing |
| `player.p2.body` / `player.p2.cap` | P2's game-provided costume and cap model names |
| `respawn.delay_seconds` | Recovery delay |
| `bubble.*` | Distance, hold time, cooldown and water-recovery behavior |
| `competition.coin.*` / `competition.moon.*` | Optional scoreboards; disabled by default |

**Experimental branch only:** PR #1 introduces `player.p2.spawn_offset` (default `0`, meaning no change to upstream behavior). It is **not** in `main` or the upstream release.

### Troubleshooting

- **P2 not appearing:** verify game version 1.0.0, controller assignment, and the pause-menu `Two Players` controller prompt.
- **Controllers controlling the wrong actor:** switch to `One Player`, let the stage rebuild, and try `Two Players` again. Restart the game if needed.
- **Stage crashes or hangs:** reinstall one matching package, disable unrelated mods, and temporarily disable competition modes. Check [known issues](KNOWN_ISSUES.md) before reporting.
- **Reporting:** include game/mod/emulator versions, kingdom, expected versus actual behavior, reproduction steps and redacted `[OCoop]` log lines. Do not share game files, keys, personal paths or sensitive save data.

</details>

## Building from source

Clone **this fork** with the pinned ExLaunch submodule:

```powershell
git clone --recurse-submodules https://github.com/The-Power-0/OdysseyLocalCoop.git
Set-Location OdysseyLocalCoop
```

Build locally using devkitPro, or use the pinned devkitPro container:

```powershell
.\scripts\build.ps1 -DevkitProRoot PATH_TO_DEVKITPRO
# OR:
.\scripts\build.ps1 -UseDocker

.\scripts\package.ps1
.\scripts\verify.ps1
```

Outputs are generated under `artifacts/`. The build requires the repository's pinned ExLaunch commit; the original [build workflow](.github/workflows/build.yml) compiles, packages, verifies and uploads a **development artifact**, not a gameplay certification. Use the appropriate development branch checkout when reviewing an unmerged PR.

<details>
<summary><strong>How the original mod works (technical overview)</strong></summary>

The mod builds an [ExLaunch](https://github.com/shadowninja108/exlaunch) `subsdk9` module for Odyssey 1.0.0. During a stage initialization window, it copies the native P1 player-construction data while that data is valid, constructs a second player actor with a separate controller and costume, and registers that actor with the stage's native player holder.

The shared camera layers midpoint targeting, distance-based zoom and right-stick arbitration over Odyssey's camera systems instead of replacing them. Bubble recovery reuses native recovery animations, monitors player separation, chooses a partner destination and retargets recovery while the partner moves. Other scoped hooks adapt doors, 2D valves, cap ownership, captures and scene-owned P2 health layouts.

The main remaining challenge is that multiple native game systems assume a single player at index zero. Changes must correct those assumptions **only in their relevant context** to avoid regressing unrelated gameplay. See the original [CONTRIBUTING.md](CONTRIBUTING.md) for requirements around verified hook offsets and private reverse-engineering material.

</details>

## Contributing and credits

Start with [open issues](https://github.com/The-Power-0/OdysseyLocalCoop/issues), read [CONTRIBUTING.md](CONTRIBUTING.md) and [PROGRESS.md](PROGRESS.md), and keep changes scoped to one behavior. Please provide sanitized reproduction steps and clearly label emulator tests, hardware tests, and unverified hypotheses.

**Original author and project:** [Gudui/OdysseyLocalCoop](https://github.com/Gudui/OdysseyLocalCoop). This independent fork retains the upstream project history, [credits](CREDITS.md), [third-party notices](THIRD_PARTY_NOTICES.md), and [GPL-2.0-only license](LICENSE). The fork's additional work does not imply upstream endorsement.

Super Mario Odyssey and its assets are copyright Nintendo. This unofficial fan project is not affiliated with or endorsed by Nintendo. Do not submit copyrighted game executables, ROM dumps, keys, firmware or private game assets.

