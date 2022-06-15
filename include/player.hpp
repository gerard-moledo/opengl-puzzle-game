#ifndef PLAYER_H
#define PLAYER_H

#include "glm/glm.hpp"

#include "models.hpp"

struct Player
{
    Player(Renderer& renderer);

    CubeModel cube;

    void Draw(glm::mat4 renderTransform);
};

#endif