# P0 story-transition diagnostic plan

This document supports two **unresolved** progression bugs, not fixes:

- [#2 — Metro Kingdom boss victory white-screen softlock](https://github.com/The-Power-0/OdysseyLocalCoop/issues/2)
- [#3 — Lake Kingdom painting infinite loading loop](https://github.com/The-Power-0/OdysseyLocalCoop/issues/3)

No underlying cause has been verified. This small diagnostic patch adds bounded read-only logging inside **existing** Odyssey 1.0.0 hooks; it does **not** alter the game’s control flow, scene ownership or saves.

## Logging changes

| Marker | Meaning |
| --- | --- |
| `PATCH-0047 P0-STORY picture ... idx=... accepted=...` | A painting received a native BindStart / BindInit message; **idx=-1** means the sender was not resolved to an active registered actor. Previously only idx >= 0 binds were logged. |
| `PATCH-0011 P0-STORY native stage-change requested` | Odyssey's original stage-change poll had already marked a stage request; this isn't proof the next scene loaded. |
| `PATCH-0011 P0-STORY p2 area-enter` | Our added P2 area poll attempted a stage change. A flagged request isn't proof of successful loading. |
| `PATCH-0024 change-world detach` (existing) | Change-world HUD cleanup hook ran, not necessarily that transition completed. |
| `PATCH-0052 ...` (existing) | Pause-menu player-mode restart path; generally not related to boss completion unless explicitly triggered. |

Each new marker is limited to **16 or 32 messages per session** to avoid uncontrolled log spam. These paths are already active in upstream, with no new executable hooks or new persistent data.

## Safe reproduction

1. Make a backup of a **legally obtained** Super Mario Odyssey **1.0.0** save; use Ryujinx with separate P1 and P2 controller slots and disable unrelated game mods.
2. Verify the installed build is from the `diagnostics/p0-story-transitions` branch and not a released `main` build.
3. **Metro (#2):** record pre-boss checkpoint, whether P1 or P2 triggered the decisive attack, fade to white and whether any next-stage request appears. Restart only after capturing the hang evidence and verify if boss progress persisted.
4. **Lake (#3):** run separate P1-initiated and P2-initiated painting entries. Record `BINDSTART` / `BINDINIT` and `idx` / `accepted`, then whether the destination loaded.
5. Save a **redacted** excerpt of `[OCoop]` log lines and a timestamped test summary to the corresponding issue.

No game dump, Nintendo binaries, keys, firmware or personal paths should be attached to public issues. A missing log marker only means this hook wasn't observed; it does **not** prove a different system caused the failure.

## Next decision

Once reliable evidence isolates the failing stage, source or save transition, implement an issue-specific fix in another branch and verify both single-player and two-player story progression. Keep this diagnostic PR unmerged while evidence is incomplete.
