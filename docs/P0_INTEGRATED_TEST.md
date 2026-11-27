# All-in-one P0 development test build

**UNRELEASED / IN-GAME UNVERIFIED.** This branch combines the code from three independent draft PRs for testing without swapping mod installs.

| Source | Purpose |
| --- | --- |
| [PR #50](https://github.com/The-Power-0/OdysseyLocalCoop/pull/50) | Conservative recovery anchor safeguards + host tests |
| [PR #51](https://github.com/The-Power-0/OdysseyLocalCoop/pull/51) | Non-mutating Metro/Lake transition logging |
| [PR #54](https://github.com/The-Power-0/OdysseyLocalCoop/pull/54) | Opt-in in-game P0 Test Lab menu + local save fixtures |

This is an **integration-only branch**. Do not merge it into main; review and merge individual PRs after each issue is verified. All P0 issues #2–#5 remain open.

## Odyssey 1.4.1 development package

The `-smo-1.4.1` artifact targets the original Switch executable in the 1.4.1 update. Use it with Odyssey showing **1.4.1** in Ryujinx. The other artifact targets 1.0.0.

The updated executable shares its main build ID (`b424be150a8e7d78701cbe7a439d9ebf`, first 16 bytes) with 1.3.0 on original Switch. The port uses [Colbster937’s 1.3.0 offsets and compatibility changes](https://github.com/colbster937/OdysseyLocalCoopPlus/commit/c6bc8372e42d7d991619a1ef572b44205cb949b7). At startup it checks for that build ID in the main executable’s mapped read-only data before installing hooks. If the ID is missing, it logs the failure and skips the hooks.

Build with `./scripts/build.ps1 -UseDocker -GameVersion 1.4.1`, then run `./scripts/package.ps1` and `./scripts/verify.ps1`. Use a disposable Ryujinx save for the first boot and controller check. Switch 2 and in-game behavior still require validation.

## Install/test procedure

1. Back up your Ryujinx save data, disable conflicting gameplay mods and download the development artifact matching your Odyssey version from this branch's successful **Build release package** GitHub Actions run. Extract the package into a subfolder **inside your portable Ryujinx directory**, so the installer searches upward and finds the siblings `Ryujinx.exe` and `portable/`.
2. With Ryujinx closed, open PowerShell beside the extracted `install.ps1` and run `./install.ps1` **without arguments**. It automatically installs to `portable/mods/contents/0100000000010000`. Use `-ContentsRoot "D:\\Ryujinx\\portable\\mods\\contents"` only when the extracted package is outside that folder tree. The installer logs its chosen destination; inspect it before launching the game.
3. Set `debug.menu.enabled=1` in the installed `romfs/OCoop/settings.ini`. Configure two separate controllers and select the game's `Two Players` option.
4. Use P1 `D-pad Up` to select the P0 preset and `A` twice to mark or replay in-scene safe positions. `Minus` hides or shows the lab (normal map input is reserved while the lab is enabled).
5. Read [P0_TEST_LAB.md](P0_TEST_LAB.md), [P0_TRIAGE.md](P0_TRIAGE.md) and [P0_STORY_TRACE.md](P0_STORY_TRACE.md). For pre-boss event rollback, use the local `scripts/p0-fixtures.ps1` with the emulator **closed**, not a mod operation.
6. Capture redacted `[OCoop]` logs, exact game/emulator/commit/controller state, and post-restart story progress in the relevant GitHub issue.


For example, the following **nested** layout is supported without `-ContentsRoot`:

```text
Ryujinx/
  Ryujinx.exe
  portable/
  P0-Test/
    OdysseyLocalCoop-v0.1.0-alpha.5/
      install.ps1
      payload/
      PACKAGE_MANIFEST.json
      CHECKSUMS.txt
```

## Limits

The Test Lab prototype currently does **not** directly warp into Metro boss, Lake painting or Lost lava, alter boss quest state, or trigger the native dual lava death path. Scene-local markers require reaching each encounter once. Private local save fixtures can restore pre-captured story state after a restart, but may require closing Ryujinx. The menu's layout and position replay have passed compilation only, not game validation.

## Why these checks matter

The build, unit tests, and fixture restore tests can pass while the real boss/painting event still hangs. They validate infrastructure; only instrumented gameplay can validate an issue fix. No P0 issue should be closed based on this build alone.
