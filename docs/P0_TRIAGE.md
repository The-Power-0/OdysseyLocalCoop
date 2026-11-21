# P0 incident triage and gameplay validation

These are **test protocols and code-audit findings**, not verified fixes.
Baseline: Super Mario Odyssey **1.0.0** and Gudui upstream alpha.5.
Test a legally obtained game copy, backing up saves first.
Never upload dumps, firmware, game keys, proprietary assets, or unredacted logs.

## Test matrix

| Issue | Scenario | Steps and expected result |
| :-- | :-- | :-- |
| [#2 Metro victory white-screen](https://github.com/The-Power-0/OdysseyLocalCoop/issues/2) | Two controllers, story boss | Back up the save. Defeat Metro boss with P1 leading; repeat P2 leading if possible. Record fade/scene load and restart checkpoint. Successful result: no white hang; victory persists after restart. |
| [#3 Lake painting](https://github.com/The-Power-0/OdysseyLocalCoop/issues/3) | Stage painting travel | Enter Lake Kingdom painting first with P1 and then with P2 (separate controlled runs). Record sender and destination, whether stage ever loads, and return path. Successful result: both transitions finish and world state persists. |
| [#4 Lost lava zero-health](https://github.com/The-Power-0/OdysseyLocalCoop/issues/4) | P2 terminal purple lava | Reduce P2 to last heart then fall into Lost Kingdom purple lava. Confirm P2 either plays native death/recovery then returns with valid health or triggers a valid team miss. Never leave a controllable P2 at zero HP. |
| [#5 Dual fall mutual bubble](https://github.com/The-Power-0/OdysseyLocalCoop/issues/5) | Simultaneous terminal falls | Put both players in terminal/lava/abyss state in the same episode; verify they do not choose each other as destinations while neither is grounded. Confirm game returns to a safe state or a native full miss. |

Run each with no unrelated gameplay mods and record: exact game version, emulator build, repo commit, controller slots, scene/kingdom, P1/P2 life values, and timestamps. Use before/after videos if available without redistributing game assets.

## Code-audit findings

- **#2:** Existing `PATCH-0011` polls P2 stage-change areas, while `PATCH-0052` owns the pause-menu restart/sequence-kill lifecycle. The Metro boss's victory-save and sequence transition must be traced independently. We have **not** established its root cause.
- **#3:** Existing `PATCH-0047` scopes player index for `PictureStageChange::receiveMsg`. The Lake painting still requires traces distinguishing receive/accepted bind, stage-change request, world resource load, and arrival. We have **not** established its root cause.
- **#4/#5:** `PATCH-0043` and `PATCH-0044` already evaluate safe collision destinations for **distance** bubbles and **bubble-pop** timing. Before this PR, `PATCH-0003`'s normal recovery redirect and per-frame destination refresh did **not** check a live partner's grounding/dead status. This PR conservatively gates those redirects, and also blocks seeding P1 recovery from terminal/airborne P2.

### How to interpret logs

Useful existing `[OCoop]` markers: `PATCH-0003`, `PATCH-0006`, `PATCH-0010`, `PATCH-0011`, `PATCH-0015`, `PATCH-0033`, `PATCH-0035`, `PATCH-0044`, `PATCH-0047`, `PATCH-0052`. This PR adds bounded `P0-RECOVERY` safety-guard diagnostics. Their presence shows the guard evaluated, **not** that the user-visible bug is fixed.

### Limits of this mitigation

- It **does not** fix terminal P2 death animation or zero-health recovery lifecycle (#4).
- It **does not** prove both players recover when they are simultaneously ungrounded (#5).
- It **does not** guarantee Odyssey's native fallback safety point is actually safe.
- It **does not** change stage transitions or save behavior (#2, #3).
- Full Ryujinx regression runs are still required before claiming issue completion.
