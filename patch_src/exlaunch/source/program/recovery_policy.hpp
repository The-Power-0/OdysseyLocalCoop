#pragma once

// Pure recovery policy for game-specific callers and host-side tests.
// Never use a teammate as a recovery destination if the teammate is in
// terminal damage/death, Abyss, or lacks a safe collision authority.
namespace ocoop::recovery {

struct AnchorState {
    bool valid;
    bool dead;
    bool abyss;
    bool terminal;
    bool keeperValid;
    bool colliderValid;
    bool grounded;
    bool inWater;
    bool waterAllowed;
};

inline bool CanAnchor(const AnchorState& state) {
    return state.valid && !state.dead && !state.abyss && !state.terminal &&
           state.keeperValid && state.colliderValid &&
           (state.grounded || (state.waterAllowed && state.inWater));
}

// Accrue sustained separation independently of the destination gate. A
// temporarily airborne teammate may delay recovery but cannot restart the wait.
struct SeparationGrace {
    unsigned frames = 0;

    bool Tick(bool eligible, unsigned requiredFrames) {
        if (!eligible) {
            frames = 0;
            return false;
        }
        if (frames < requiredFrames)
            ++frames;
        return frames >= requiredFrames;
    }
};

}  // namespace ocoop::recovery
