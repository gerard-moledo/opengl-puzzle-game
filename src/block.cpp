#include "block.hpp"

#include "glm/gtc/matrix_transform.hpp"

Block::Block(Renderer& renderer, BlockType type, Vector2i cell) :
    cube(renderer),
    currentCell(cell),
    targetCell(cell),
    type(type)
{
    worldPosPrev = glm::vec3(cell.x, 0.0f, cell.y);
    worldPos = worldPosPrev;

    switch (type)
    {
        case BlockType::wall:
            cube.color = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
            break;
        case BlockType::block:
            cube.color = glm::vec4(0.588f, 0.294f, 0.0f, 1.0f);
            break;
        default:
            cube.color = glm::vec4(1.0f);
            break;
    }
}

void Block::Update(float dt)
{
    worldPosPrev = worldPos;

    if (state == State::moving)
    {
        bool done = Move(dt);

        if (done)
        {
            tMove = 0.0f;
            currentCell = targetCell;

            state = State::postmove;
        }
    }
}

bool Block::Move(float dt)
{
    tMove += dt;

    float tSolved = sqrtf(1.0f / SPEED);
    float tLerp = tMove * 2 * SPEED * tSolved;

    glm::vec3 glmCurrent = glm::vec3(currentCell.x, 0.0f, currentCell.y);
    glm::vec3 glmTarget = glm::vec3(targetCell.x, 0.0f, targetCell.y);
    worldPos = glm::mix(glmCurrent, glmTarget, glm::clamp(tLerp, 0.0f, 1.0f));

    return tLerp >= 1.0f;
}

void Block::Draw(glm::mat4 renderTransform, float lag)
{
    glm::vec3 renderPos = glm::mix(worldPosPrev, worldPos, lag);

    cube.model = glm::mat4(1.0f);
    cube.model = glm::translate(cube.model, glm::vec3(renderPos.x * 2.0f, 0.0f, renderPos.z * -2.0f));

    cube.Draw(renderTransform);
}