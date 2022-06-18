#ifndef BLOCK_H
#define BLOCK_H

#include "glm/glm.hpp"

#include "models.hpp"
#include "renderer.hpp"
#include "utils.hpp"

enum class BlockType { wall, block, goal };

typedef std::pair<BlockType, Vector2i> BlockState;
typedef std::vector<BlockState> BlockStates;

struct Block
{
    Block(Renderer& renderer, Vector2i cell);

    Vector2i currentCell;
    Vector2i targetCell;
    Vector2i direction { 0, 0 };
    
    glm::vec3 worldPosPrev;
    glm::vec3 worldPos;
    
    float tMove = 0.0f;

    BlockType type = BlockType::wall;
    
    CubeModel cube;

    void Update(float dt);
    void Draw(glm::mat4 renderTransform, float lag);
};

#endif