# Experimental P0 Test Lab — in-game developer menu

**Status:** First prototype, **not** a complete P0 scenario launcher. Not merged into the public baseline.

This tool is for people developing and testing [#2](https://github.com/The-Power-0/OdysseyLocalCoop/issues/2), [#3](https://github.com/The-Power-0/OdysseyLocalCoop/issues/3), [#4](https://github.com/The-Power-0/OdysseyLocalCoop/issues/4) and [#5](https://github.com/The-Power-0/OdysseyLocalCoop/issues/5) on a **backed-up** Super Mario Odyssey 1.0.0 save.

## Enable

Edit the installed `romfs/OCoop/settings.ini`:

```ini
debug.menu.enabled=1
```

Start full two-player co-op and enter a stage. The Test Lab uses a separate **OCoopScoreBoard layout instance** to display the selected preset and status. This is a temporary reuse of existing custom mod UI; text placement and appearance require Ryujinx testing. It is disabled by default and is **not** meant to be present in release builds.

**Controls (Player 1):** `D-pad Up` cycles the four presets; `A` arms the current action, then `A` again runs it. `Minus (-)` hides/shows the debug overlay. While the lab is enabled, `Minus` is reserved for it instead of opening the map. Native gameplay actions for other buttons are not suppressed; avoid using the overlay during a cutscene or capture.

## Presets

| Menu item | What the first confirmation does | Subsequent confirmations in the *same stage* |
| --- | --- | --- |
| **#2 Metro boss** | Saves both players' safe current positions near the boss | Moves both players to the recorded positions |
| **#3 Lake painting** | Saves safe positions near the painting | Moves both players back to the positions |
| **#4 Lost lava** | Saves both positions and sets P2's **private** health to 1 if not terminal | Restores positions and re-arms P2's 1 HP |
| **#5 Double fall** | Saves both players' positions and sets P2 to 1 HP | Restores both players' positions and P2's 1 HP; enter the hazard together **normally** |

The saved marker expires when the stage is rebuilt or changes. A captured, terminal, airborne, or ungrounded player cannot be marked/repositioned by this version's safety guard. No test button directly marks a player dead or modifies story flags, since that would bypass the exact native code paths we need to debug. The pair must **actually** fall into the hazard to reproduce #4/#5.

**The current prototype cannot yet jump to Metro/Lake/Lost Kingdom or force an uncleared boss story scenario.** That needs audited 1.0.0 ChangeStageInfo/scene APIs and a disposable-save sandbox; do not misrepresent a teleport into a completed kingdom as a valid boss-victory reproduction. It can, however, replay player positions and health repeatedly within a loaded scene without traversing the area again.

## Local repeatable save fixtures (companion utility)

For Metro boss/painting events whose story state is consumed by completion, use the opt-in **local save fixture manager**. This is a separate PowerShell convenience utility because the mod must not silently rewrite campaign progress. It stores your **own** save snapshots outside the repository, and only works with Ryujinx **closed**.

```powershell
# Replace the path with the exact Ryujinx 'Open User Save Directory' path.
$SaveDir = "PATH_TO_YOUR_ODYSSEY_SAVE"
./scripts/p0-fixtures.ps1 -Action Capture -Scenario MetroBoss -SaveDir $SaveDir

# Later, after the boss victory/hang, with the emulator closed:
./scripts/p0-fixtures.ps1 -Action Restore -Scenario MetroBoss -SaveDir $SaveDir -ConfirmRestore
```

Other fixture names: `LakePainting` and `LostLava`. `-Action List` lists locally captured fixtures. A restore verifies SHA-256 hashes and preserves a timestamped backup of the previous active save. You must reach or otherwise obtain each starting checkpoint **once** to capture it; there are no supplied copyrighted game saves. Save file handling is local only. This is **not an in-game one-click scene reset** and cannot substitute for verifying stage progression transitions.

## Required next stages

1. Replace the temporary reused scoreboard UI with a dedicated, readable menu layout and explicit Exit/Reset actions.
2. Add a verified internal stage/scenario warper with a **confirm screen** and no automatic write to the user's normal save.
3. Add spawn-point selection for Metro's boss and Lake painting; verify correct scenario flags, current quest progress and checkpoint handling.
4. Add safe scenario snapshots with one-click restore or emulator snapshot integration to reproduce irreversible boss victory behavior.
5. Validate with two real controllers in Ryujinx, then on hardware only if intended.

## Safety / known limits

- Always back up the save; no claim of save isolation or scenario reset yet.
- The in-game position restore uses Odyssey's native `al::setTrans`, guarded to living grounded uncaptured players. It does not reset momentum, capture, world event state, or quest flags, and remains unverified in-game.
- The P2 health setter uses OCoop's private P2 health sidecar (does **not** decrement P1's shared save health). It is intentionally blocked while P2 is terminal.
- The temporary HUD reuses `LayoutData/OCoopScoreBoard.szs`, which may have narrow text panes; **runtime visual review is needed**.
- Do not upload Nintendo assets, game dumps, keys, firmware or private save data to GitHub.
