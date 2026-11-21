# Odyssey Local Co-op — development plan (unofficial fork)

Original: https://github.com/Gudui/OdysseyLocalCoop
Supported baseline: Super Mario Odyssey 1.0.0, from upstream main.

This fork begins with gameplay-preserving, opt-in experiments. Do not
advertise a feature as compatible with hardware or emulator until tested.

## Roadmap

1. Baseline: reproducible builds, host-side math tests, and Ryujinx regression cases.
2. Progression: investigate Metro Kingdom boss and Lake Kingdom painting loading loops.
3. Recovery: eliminate simultaneous-bubble loops and unsafe terrain respawns.
4. Health: repair player-two heart pickups, Life-Up Hearts, and Moon healing.
5. Ownership: make captures, doors, pipes, bosses, cinematics and collectibles player-aware.
6. Polish: camera behavior, player UI, settings and controller handling.
7. Optional expansion: review 1.3.0 and 3–4-player adaptations independently after stability.

## Phase 1: experimental P2 spawn spacing

- Configuration: player.p2.spawn_offset in romfs/OCoop/settings.ini
- Default 0 retains upstream behavior; try 120 for a sideways offset.
- Valid range -1000 to +1000, relative to Mario's local horizontal axis.
- The translation/quaternion offsets are taken from the related Plus fork's
  implementation and have not been independently game-tested in this fork.
- Host-side tests exercise the pure positioning math, not game integration.

## Release criteria

- Unit test workflow succeeds.
- Original build, package and verification scripts succeed with pinned dependencies.
- Ryujinx testing covers 1P ↔ 2P, stages, pipes, doors, camera, captures, recovery.
- Repeat on hardware before claiming physical Switch compatibility.
- Preserve all original copyright notices and GPL-2.0-only attribution.
- Do not commit copyrighted game files, keys, private builds or game dumps.

Known upstream problems: see KNOWN_ISSUES.md.
