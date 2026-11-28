# Experimental P0 Test Lab — in-game developer menu

**Status:** Test 1C and 1D passed the user's reported menu, marker, health, capture and scene-reset checks on commit `28127dc`; Test 10 did not specify whether P1 jumped. The experimental native reload branch extends that verified baseline; its runtime behavior is not yet verified.

This tool is for people developing and testing [#2](https://github.com/The-Power-0/OdysseyLocalCoop/issues/2), [#3](https://github.com/The-Power-0/OdysseyLocalCoop/issues/3), [#4](https://github.com/The-Power-0/OdysseyLocalCoop/issues/4) and [#5](https://github.com/The-Power-0/OdysseyLocalCoop/issues/5) on a **disposable, backed-up** Super Mario Odyssey 1.4.1 save in Ryujinx.

## Enable

Edit the installed `romfs/OCoop/settings.ini`:

```ini
debug.menu.enabled=1
```

Start full two-player co-op and enter a stage. The Test Lab uses a separate **OCoopScoreBoard layout instance** to display the selected preset and status. This is a temporary reuse of existing custom mod UI; text placement and appearance require Ryujinx testing. It is disabled by default and is **not** meant to be present in release builds.

**Controls (Player 1):** `D-pad Up` cycles the original eight actions, then Pylon mark/replay, Pylon clear, and RELOAD; `A` arms the current action, then `A` again runs it. `Minus (-)` hides/shows the debug overlay. While the lab is enabled, `Minus` is reserved for it instead of opening the map. Native gameplay actions for other buttons are not suppressed. A clear-marker action requires two confirmations and only erases that scenario's in-memory marker. Confirmations are disarmed while the scene layout is hidden or a demo is active. Mark/replay refuses captured, terminal, airborne and water states.

## Presets

| Menu item | What the first confirmation does | Subsequent confirmations in the *same stage* |
| --- | --- | --- |
| **#2 Metro boss** | Saves both players' safe current positions near the boss | Moves both players to the recorded positions |
| **#3 Lake painting** | Saves safe positions near the painting | Moves both players back to the positions |
| **#4 Lost lava** | Saves both positions and sets P2's **private** health to 1 if not terminal | Restores positions and re-arms P2's 1 HP |
| **#5 Double fall** | Saves both players' positions and sets P2 to 1 HP | Restores both players' positions and P2's 1 HP; enter the hazard together **normally** |

| **#11 Cap Pylon** | Records both positions near the wire | Replays positions without modifying health or capture |

The saved marker expires when the stage is rebuilt or changes. A captured, terminal, airborne, or ungrounded player cannot be marked/repositioned by this version's safety guard. No test button directly marks a player dead or modifies story flags, since that would bypass the exact native code paths we need to debug. The pair must **actually** fall into the hazard to reproduce #4/#5.

**The current prototype cannot yet jump to Metro/Lake/Lost Kingdom or force an uncleared boss story scenario.** That needs audited 1.4.1 ChangeStageInfo/scene APIs and a disposable-save sandbox; do not misrepresent a teleport into a completed kingdom as a valid boss-victory reproduction. It can, however, replay player positions and health repeatedly within a loaded scene without traversing the area again.

## Native stage reload — Test 2

On a **separate disposable Ryujinx profile**, enable both settings:

```ini
debug.menu.enabled=1
debug.native_reload.enabled=1
```

Restart Ryujinx and enable two-player mode. Cycle to `RELOAD / A RELOAD`. Both players must be alive, grounded, uncaptured and outside a cutscene. Press A, release, then A again. The request is queued for the existing HakoniwaSequence play-stage hook, rather than killing a scene from a player control callback. The sequence checks the current actor/scene, native save readiness, transition flag, restart guard and 2P mode, then uses the same audited 1.4.1 restart path as PATCH-0052. Native game autosaves may occur; do not use this opt-in with a normal campaign save.

Expected log sequence:

- `P0-LAB native reload queued`
- `P0-LAB native reload prepared target=...`
- `PATCH-0052 sequence kill ...`
- `P0-LAB reload: new scene HUD created`
- `P0-LAB reload: new scene two-player control hook ready`

Check that both controllers work, the HUD returns, old markers are cleared, and the same stage is loaded. Repeat twice; test disabled flag, airborne/captured rejection, cancel and hidden-menu paths. `BLOCKED` means a safety guard rejected the request; inspect the logged guard matrix. Do not repeatedly retry a failed or stuck load; exit Ryujinx and preserve the log.

### Encounter coverage

| Scenario | Native reload test | Encounter reset |
| --- | --- | --- |
| Cap Pylon #11 | Reload loaded Cap stage; use Pylon marker | Restore pre-crossing fixture if story changed |
| Metro boss #2 | Reload loaded Metro stage; use Metro marker | Restore pre-victory fixture |
| Lake painting #3 | Reload loaded Lake stage; use Lake marker | Restore pre-entry fixture when necessary |
| Lost lava #4 | Reload loaded Lost stage; arm P2 one heart | Restore fixture if native state changed |
| Double fall #5 | Reload loaded hazard stage; arm P2 one heart | Trigger both falls naturally |

This native reload is **not yet an arbitrary kingdom warp, encounter selector, or quest reset**. It deliberately reuses the existing restart path instead of constructing a speculative ChangeStageInfo for 1.4.1. The fixture manager remains necessary for irreversible events and reaching exact story states. No normal-save isolation can be enforced by the guest module; separate test profile is mandatory.

## Local repeatable save fixtures (companion utility)

For Metro boss/painting events whose story state is consumed by completion, use the opt-in **local save fixture manager**. This is a separate PowerShell convenience utility because the mod must not silently rewrite campaign progress. It stores your **own** save snapshots outside the repository, and only works with Ryujinx **closed**.

```powershell
# Replace the path with the exact Ryujinx 'Open User Save Directory' path.
$SaveDir = "PATH_TO_YOUR_ODYSSEY_SAVE"
./scripts/p0-fixtures.ps1 -Action Capture -Scenario MetroBoss -SaveDir $SaveDir

# Later, after the boss victory/hang, with the emulator closed:
./scripts/p0-fixtures.ps1 -Action Restore -Scenario MetroBoss -SaveDir $SaveDir -ConfirmRestore
```

Other fixture names: `LakePainting`, `LostLava`, `DoubleFall`, and `CapPylon`. `-Action List` lists locally captured fixtures. A restore verifies SHA-256 hashes and preserves a timestamped backup of the previous active save. You must reach or otherwise obtain each starting checkpoint **once** to capture it; there are no supplied copyrighted game saves. Save file handling is local only. This is **not an in-game one-click scene reset** and cannot substitute for verifying stage progression transitions.

## Recommended disposable test session

Create a separate portable Ryujinx installation or profile and initialize its own Odyssey save directory. Verify the normal and test save directories using each instance's **Open User Save Directory** option. Close both instances. Capture the desired checkpoint once from your own save. The normal save is only read when capturing; the following command restores the verified fixture to the **separate** disposable directory, keeping a backup of its prior contents.

```powershell
./scripts/p0-fixtures.ps1 -Action PrepareTest -Scenario MetroBoss -SaveDir "NORMAL_SAVE_PATH" -TestSaveDir "DISPOSABLE_SAVE_PATH" -ConfirmRestore
```

Repeat the same command with both emulators closed after a softlock or completed one-time event. The command rejects equal or nested source/target directories, directory links, overlapping fixture storage, tampered fixtures, and unconfirmed restores. It cannot determine whether the user chose a genuinely separate emulator profile, so verify the paths manually. Do not run the normal profile while testing. The legacy Restore action deliberately targets the supplied active directory and is not the recommended workflow.

This is a **local checkpoint launcher by save restoration**, not a native stage/scenario warp. It still requires valid captured checkpoints. Markers do not persist across scenes or process restarts. Native death and victory must be triggered through gameplay.

## Required next stages

1. Replace the temporary reused scoreboard UI with a dedicated, readable menu layout and explicit Exit/Reset actions.
2. Extend verified native reload into a verified internal stage/scenario warper with a **confirm screen** and no automatic write to the user's normal save.
3. Add spawn-point selection for Metro's boss and Lake painting; verify correct scenario flags, current quest progress and checkpoint handling.
4. Add safe scenario snapshots with one-click restore or emulator snapshot integration to reproduce irreversible boss victory behavior.
5. Validate with two real controllers in Ryujinx, then on hardware only if intended.

## Safety / known limits

- Always back up the save; no claim of save isolation or scenario reset yet.
- The in-game position restore uses Odyssey's native `al::setTrans`, guarded to living grounded uncaptured players. The original marker is not independently collision-checked again at replay time; use static, unchanged terrain only. It does not reset momentum, capture, world event state, or quest flags, and passed the user's basic mark/replay tests on commit `28127dc`.
- The P2 health setter uses OCoop's private P2 health sidecar (does **not** decrement P1's shared save health). It is intentionally blocked while P2 is terminal.
- The temporary HUD reuses `LayoutData/OCoopScoreBoard.szs`, which may have narrow text panes; **runtime visual review is needed**.
- Do not upload Nintendo assets, game dumps, keys, firmware or private save data to GitHub.
