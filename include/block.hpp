#ifndef BLOCK_H
#define BLOCK_H

#include "glm/glm.hpp"

#include "models.hpp"
#include "renderer.hpp"
#include "utils.hpp"

class Block
{
public:
    Block(BlockType type, Vector2i cell);

    MoveState state = MoveState::idle;

    bool onGoal = false;
    
    Vector2i currentCell;
    Vector2i targetCell;
    Vector2i direction { 0, 0 };
    
    glm::vec3 worldPosPrev;
    glm::vec3 worldPos;
    
    float tMove = 0.0f;

    BlockType type;
    
    CubeModel cube;

    void Update(float dt);
    void Draw(glm::mat4 renderTransform, float lag);

private:
    bool Move(float dt);
};

#endif