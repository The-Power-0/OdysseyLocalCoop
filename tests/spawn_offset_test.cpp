#include "program/spawn_offset.hpp"

#include <cassert>
#include <cmath>
#include <limits>

static bool Near(float a, float b) { return std::fabs(a - b) < 0.001f; }

int main() {
    const float identity[] = {0, 0, 0, 1};
    float p1[] = {10, 20, 30};
    assert(ocoop::spawn::ShiftAlongLocalX(p1, identity, 120));
    assert(Near(p1[0], 130) && Near(p1[1], 20) && Near(p1[2], 30));

    const float s = std::sqrt(0.5f);
    const float rotatedY[] = {0, s, 0, s};
    float p2[] = {1, 2, 3};
    assert(ocoop::spawn::ShiftAlongLocalX(p2, rotatedY, 120));
    assert(Near(p2[0], 1) && Near(p2[1], 2) && Near(p2[2], -117));

    float p3[] = {1, 2, 3};
    assert(ocoop::spawn::ShiftAlongLocalX(p3, identity, -120));
    assert(Near(p3[0], -119));

    float p4[] = {1, 2, 3};
    assert(ocoop::spawn::ShiftAlongLocalX(p4, identity, 0));
    assert(Near(p4[0], 1) && Near(p4[1], 2) && Near(p4[2], 3));

    const float bad[] = {0, 0, 0, 0};
    float p5[] = {1, 2, 3};
    assert(!ocoop::spawn::ShiftAlongLocalX(p5, bad, 120));
    assert(Near(p5[0], 1) && Near(p5[1], 2) && Near(p5[2], 3));
    assert(!ocoop::spawn::ShiftAlongLocalX(p5, identity, std::numeric_limits<float>::quiet_NaN()));
    assert(!ocoop::spawn::ShiftAlongLocalX(p5, identity, std::numeric_limits<float>::infinity()));
    assert(!ocoop::spawn::ShiftAlongLocalX(nullptr, identity, 120));
    assert(!ocoop::spawn::ShiftAlongLocalX(p5, nullptr, 120));
    return 0;
}
