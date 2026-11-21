#include "program/recovery_policy.hpp"
#include <cassert>
using ocoop::recovery::AnchorState;
using ocoop::recovery::CanAnchor;

int main() {
    AnchorState safe{true,false,false,false,true,true,true,false,false};
    assert(CanAnchor(safe));                       // P1 alive on ground; P2 can return
    auto s = safe;
    s.dead = true; assert(!CanAnchor(s));          // P1 dead
    s = safe; s.abyss = true; assert(!CanAnchor(s));    // partner in recovery
    s = safe; s.terminal = true; assert(!CanAnchor(s)); // private health already zero
    s = safe; s.grounded = false; assert(!CanAnchor(s)); // both falling, no anchor
    s = safe; s.valid = false; assert(!CanAnchor(s));
    s = safe; s.keeperValid = false; assert(!CanAnchor(s));
    s = safe; s.colliderValid = false; assert(!CanAnchor(s));

    s = safe; s.grounded = false; s.inWater = true;
    assert(!CanAnchor(s));                        // water option off
    s.waterAllowed = true; assert(CanAnchor(s));  // grounded-equivalent water option
    s.terminal = true; assert(!CanAnchor(s));     // water cannot bypass terminal

    const AnchorState bothDownA{true,false,true,true,true,true,false,false,false};
    const AnchorState bothDownB{true,true,false,true,true,true,false,false,false};
    assert(!CanAnchor(bothDownA));                // no reciprocal airborne target
    assert(!CanAnchor(bothDownB));

    return 0;
}
