#pragma once

#include <cmath>

namespace ocoop::camera {

struct Vec3 {
    float x, y, z;

    Vec3 operator-(Vec3 other) const { return {x - other.x, y - other.y, z - other.z}; }
    Vec3 operator*(float scale) const { return {x * scale, y * scale, z * scale}; }
};

inline float Dot(Vec3 a, Vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline Vec3 Cross(Vec3 a, Vec3 b) {
    return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}

inline bool Normalize(Vec3& direction) {
    const float length = std::sqrt(Dot(direction, direction));
    if (!std::isfinite(length) || length < 0.001f)
        return false;
    direction = direction * (1.0f / length);
    return true;
}

enum class Visibility { Uncertain, Inside, Outside };

struct View {
    Vec3 eye{};
    Vec3 forward{};
    Vec3 right{};
    Vec3 up{};
    float halfHeight = 0.0f;

    bool Set(Vec3 position, Vec3 target, Vec3 cameraUp, float fovyDegrees) {
        halfHeight = 0.0f;
        if (!(fovyDegrees >= 15.0f && fovyDegrees <= 100.0f))
            return false;
        auto facing = target - position;
        if (!Normalize(facing))
            return false;
        auto sideways = Cross(facing, cameraUp);
        if (!Normalize(sideways))
            return false;
        eye = position;
        forward = facing;
        right = sideways;
        up = Cross(right, forward);
        halfHeight = std::tan(fovyDegrees * 0.00872664626f);
        return true;
    }

    Visibility Classify(Vec3 position) const {
        constexpr float aspect = 16.0f / 9.0f;
        constexpr float margin = 150.0f; // Allow for the body around the actor origin.
        const auto offset = position - eye;
        const float depth = Dot(offset, forward);
        if (!(halfHeight > 0.0f && depth > margin))
            return Visibility::Uncertain;
        const float x = std::fabs(Dot(offset, right));
        const float y = std::fabs(Dot(offset, up));
        const float vertical = depth * halfHeight;
        const float horizontal = vertical * aspect;
        if (x > horizontal + margin || y > vertical + margin)
            return Visibility::Outside;
        if (x < horizontal - margin && y < vertical - margin)
            return Visibility::Inside;
        return Visibility::Uncertain;
    }
};

}  // namespace ocoop::camera
