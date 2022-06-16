#ifndef PLAYER_H
#define PLAYER_H

#include "glm/glm.hpp"

#include "models.hpp"
#include "utils.hpp"

enum class State { idle, moving };

struct Player
{
    Player(Renderer& renderer);

    State state = State::idle;
    Vector2i currentPos;
    Vector2i targetPos;

    CubeModel cube;

    void Update();
    void Draw(glm::mat4 renderTransform);

private:
    void UpdateModel();
};

#endif