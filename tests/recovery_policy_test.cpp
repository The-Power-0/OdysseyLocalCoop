#include "program/recovery_policy.hpp"
#include "program/camera_visibility.hpp"
#include <cassert>
using ocoop::recovery::AnchorState;
using ocoop::recovery::CanAnchor;
using ocoop::recovery::SeparationGrace;

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

    // A brief jump by the anchor must not make the trailing player wait twice.
    SeparationGrace grace;
    auto airborne = safe;
    airborne.grounded = false;
    assert(!grace.Tick(true, 3));
    assert(!grace.Tick(true, 3));
    assert(grace.Tick(true, 3) && !CanAnchor(airborne));
    assert(grace.Tick(true, 3) && CanAnchor(safe));
    assert(grace.frames == 3);

    // Rejoining, a scripted demo, or a change of candidate discards the wait.
    assert(!grace.Tick(false, 3));
    assert(grace.frames == 0);
    assert(!grace.Tick(true, 3));
    assert(grace.Tick(true, 0));
    assert(!grace.Tick(false, 0));

    using ocoop::camera::View;
    using ocoop::camera::Visibility;
    View view;
    assert(view.Classify({0, 0, 1000}) == Visibility::Uncertain);
    assert(view.Set({0, 0, 0}, {0, 0, 1}, {0, 1, 0}, 60));
    assert(view.Classify({0, 0, 1000}) == Visibility::Inside);
    assert(view.Classify({1500, 0, 1000}) == Visibility::Outside);
    assert(view.Classify({-1500, 0, 1000}) == Visibility::Outside);
    assert(view.Classify({0, 900, 1000}) == Visibility::Outside);
    assert(view.Classify({1025, 0, 1000}) == Visibility::Uncertain);
    assert(view.Classify({0, 0, -1000}) == Visibility::Uncertain);

    // A rolled camera exchanges which world axis hits the shorter edge.
    assert(view.Set({0, 0, 0}, {0, 0, 1}, {1, 0, 0}, 60));
    assert(view.Classify({800, 0, 1000}) == Visibility::Outside);
    assert(view.Classify({0, 800, 1000}) == Visibility::Inside);
    assert(!view.Set({0, 0, 0}, {0, 0, 1}, {0, 0, 1}, 60));
    assert(view.Classify({0, 0, 1000}) == Visibility::Uncertain);

    return 0;
}
