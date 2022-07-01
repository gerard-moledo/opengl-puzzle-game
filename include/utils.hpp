#ifndef UTILS_H
#define UTILS_H

#include "glm/gtx/polar_coordinates.hpp"

constexpr float SPEED = 35;

enum class Mode { play, edit };
enum class State { idle, premove, moving, postmove };
enum class BlockType { block, wall, goal };

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

struct LevelInfo
{
    Vector2i size;
    Vector2i playerStart;
    std::vector<std::pair<BlockType, Vector2i>> blockData;
};

inline glm::vec3 PolarToRect(glm::vec3 polar)
{
    return glm::euclidean(glm::vec2(polar.y, polar.x)) * polar.z;
}

inline Vector2i LevelToWorld(Vector2i levelCoord, Vector2i worldSize)
{
    return Vector2i { levelCoord.x - worldSize.x / 2, (levelCoord.y - worldSize.y / 2) * -1 };
}

inline Vector2i WorldToLevel(Vector2i worldCoord, Vector2i worldSize)
{
    return Vector2i { worldCoord.x + worldSize.x / 2, worldSize.y / 2 - worldCoord.y };
}

#endif