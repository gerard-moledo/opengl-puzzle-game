#ifndef UTILS_H
#define UTILS_H

#include "glm/gtx/polar_coordinates.hpp"

constexpr float SPEED = 35;

enum class State { idle, premove, moving, postmove };

struct Vector2i
{
    int x;
    int y;

    Vector2i operator+(const Vector2i& other)
    {
        return Vector2i { x + other.x, y + other.y };
    }
    Vector2i operator-(const Vector2i& other)
    {
        return Vector2i { x - other.x, y - other.y };
    }
    Vector2i& operator=(const Vector2i& other)
    {
        x = other.x;
        y = other.y;
        return *this;
    }
    bool operator==(const Vector2i& other)
    {
        return x == other.x && y == other.y;
    }
    bool operator!=(const Vector2i& other)
    {
        return x != other.x || y != other.y;
    }
    
};

inline glm::vec3 PolarToRect(glm::vec3 polar)
{
    return glm::euclidean(glm::vec2(polar.y, polar.x)) * polar.z;
}

#endif