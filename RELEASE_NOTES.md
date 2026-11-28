# OdysseyTogether v0.1.0-alpha.7

A stability-focused prerelease for **Super Mario Odyssey 1.4.1 on Ryujinx**, using the original Switch executable. This update combines the earlier alpha.6 camera and startup improvements with the Cap Kingdom Spark Pylon fix and a built-in English full-player co-op guide.

## What's new

- **Cap Kingdom Spark Pylon:** incorporates the Test 3 protection for the scripted capture and transition. A P2-led crossing successfully reached Cascade Kingdom in Ryujinx. P1-led crossings and later pylons still require gameplay regression testing. [#11](https://github.com/The-Power-0/OdysseyLocalCoop/issues/11)
- **Integrated co-op guide:** the English confirmation screen now describes two full characters and two controllers instead of Mario/Cappy roles. The text is applied at runtime to the existing layout; no separate OCoop Guide or extracted game files are needed. The standalone patch was playtested; the integrated display still needs an in-game check. [#57](https://github.com/The-Power-0/OdysseyLocalCoop/issues/57)
- **Shared camera and recovery:** retains earlier zoom-out, bounded vertical framing and the shorter off-screen bubble grace period when the other player is a safe anchor. The improved baseline was playtested and accepted. [#58](https://github.com/The-Power-0/OdysseyLocalCoop/issues/58)
- **Startup:** retains the hidden Joy-Con startup guide. A brief black loading screen may still appear before the title menu.
- Retains independent player controls, portable Ryujinx installer detection, development recovery safeguards, story diagnostics and the opt-in P0 Test Lab (off by default).

## Installation / upgrading

1. Confirm Super Mario Odyssey **1.4.1** is installed in Ryujinx and configure two controllers.
2. Right-click Odyssey in Ryujinx and choose **Open Mods Directory**.
3. Disable or remove older Odyssey co-op mods, including any loose co-op `exefs` / `romfs` from test builds and the separate `OCoop Guide` entry. Back up your saves first.
4. Extract the release ZIP into the mods directory. The result should be one `OdysseyTogether` folder containing `exefs` and `romfs`. The ZIP's SHA-256 checksum is provided separately.
5. Launch the game, enter a stage, and select **Two Players** from the pause menu.

## Validation and known limitations

- [Integration PR #59](https://github.com/The-Power-0/OdysseyLocalCoop/pull/59): build/package checks, recovery-policy, Test Lab, fixture and portable installer tests passed on the integration head.
- Gameplay evidence is specific to Ryujinx and the tested original Switch 1.4.1 build. This is not a complete campaign or physical Switch compatibility claim.
- Avoid switching between solo and co-op after defeating Topper but before reaching Cascade. A prior test build produced an inconsistent Cap Kingdom save state when switching modes there. If the pylon fails, restore a pre-Topper backup and complete the crossing in solo before enabling co-op in Cascade.
- P1-led and later Spark Pylon transitions, the integrated guide and additional camera/recovery edge cases remain to be verified.
- Metro boss, Lake painting, Lost Kingdom lava and simultaneous-death issues remain open: [#2](https://github.com/The-Power-0/OdysseyLocalCoop/issues/2), [#3](https://github.com/The-Power-0/OdysseyLocalCoop/issues/3), [#4](https://github.com/The-Power-0/OdysseyLocalCoop/issues/4), [#5](https://github.com/The-Power-0/OdysseyLocalCoop/issues/5).

## Credits

Based on [Gudui’s Odyssey Local Co-op](https://github.com/Gudui/OdysseyLocalCoop) and [Colbster937’s executable compatibility work](https://github.com/colbster937/OdysseyLocalCoopPlus). See CREDITS.md and THIRD_PARTY_NOTICES.md. Unofficial fan project; not affiliated with Nintendo.
