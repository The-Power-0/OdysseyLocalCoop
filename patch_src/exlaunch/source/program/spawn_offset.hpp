#pragma once

// SDK-independent spawn offset calculation for host-side unit tests.
// Game-specific PlayerInitInfo layout is handled by the caller.
namespace ocoop::spawn {

inline bool ShiftAlongLocalX(float* translation, const float* quaternion, float offset) {
    if (!translation || !quaternion || !(offset >= -1000.0f && offset <= 1000.0f))
        return false;
    if (offset == 0.0f)
        return true;

    const float x = quaternion[0];
    const float y = quaternion[1];
    const float z = quaternion[2];
    const float w = quaternion[3];
    const float normSquared = x*x + y*y + z*z + w*w;
    // Fail closed on unexpected or non-finite transforms.
    if (!(normSquared > 0.5f && normSquared < 1.5f))
        return false;

    const float rightX = 1.0f - 2.0f * (y*y + z*z);
    const float rightY = 2.0f * (x*y + w*z);
    const float rightZ = 2.0f * (x*z - w*y);
    const float rightSquared = rightX*rightX + rightY*rightY + rightZ*rightZ;
    if (!(rightSquared > 0.5f && rightSquared < 1.5f))
        return false;

    translation[0] += rightX * offset;
    translation[1] += rightY * offset;
    translation[2] += rightZ * offset;
    return true;
}

} // namespace ocoop::spawn
