#include "block.hpp"

#include "glm/gtc/matrix_transform.hpp"

Block::Block(Renderer& renderer, Vector2i cell) :
    cube(renderer),
    currentCell(cell),
    targetCell(cell)
{
    worldPosPrev = glm::vec3(cell.x, 0.0f, cell.y);
    worldPos = worldPosPrev;

    cube.color = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
}

void Block::Update(float dt)
{
    
}

void Block::Draw(glm::mat4 renderTransform, float lag)
{
    glm::vec3 renderPos = glm::mix(worldPosPrev, worldPos, lag);

    cube.model = glm::mat4(1.0f);
    cube.model = glm::translate(cube.model, glm::vec3(renderPos.x * 2.0f, 0.0f, renderPos.z * -2.0f));

    cube.Draw(renderTransform);
}