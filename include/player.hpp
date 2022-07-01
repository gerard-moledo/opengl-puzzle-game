#ifndef PLAYER_H
#define PLAYER_H

#include "glm/glm.hpp"

#include "block.hpp"
#include "models.hpp"
#include "utils.hpp"

struct Player
{
    Player();

    State state = State::idle;
    
    glm::vec3 worldPosPrev { 0.0f, 0.0f, 0.0f };
    glm::vec3 worldPos { 0.0f, 0.0f, 0.0f };
    
    Vector2i currentCell { 0, 0 };
    Vector2i targetCell { 0, 0 };
    Vector2i direction { 0, 0 };

    float tMove = 0.0f;
    bool accelerating = false;
    
    CubeModel cube;

    void Update(float dt);
    void Draw(glm::mat4 renderTransform, float lag);

private:
    void ReceiveInput();
    bool Move(float dt);
};

#endif