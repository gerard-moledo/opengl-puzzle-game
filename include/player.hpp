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

    float tMove = 0.0f;
    
    CubeModel cube;

    void Update(float dt);
    void Draw(glm::mat4 renderTransform);

private:
    void UpdateModel();
};

#endif