#include "world.hpp"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

World::World(Renderer& renderer) :
    floor(renderer),
    player(renderer),
    blocks(0, Block(renderer, BlockType::goal, { 0, 0 })),
    projection(glm::mat4(1.0f)),
    view(glm::mat4(1.0f))
{
    projection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);
    view = glm::lookAt(glm::vec3(0.0f, 25.0f, 15.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void World::Initialize(Renderer& renderer)
{
    blocks.emplace_back(Block(renderer, BlockType::wall, { 5, 2 }));
    blocks.emplace_back(Block(renderer, BlockType::block, { -5, 0 }));
}

void World::Update(float dt)
{
    player.Update(dt);

    for (Block& block : blocks)
    {
        block.Update(dt);
    }
    
    ModifyState();
}

void World::ModifyState()
{
    if (player.state == State::premove || player.state == State::postmove)
    {
        if (glm::abs(player.currentCell.x + player.direction.x) > 5 || 
            glm::abs(player.currentCell.y + player.direction.y) > 5)
        {
            player.state = State::idle;
        }
        for (Block& block : blocks)
        {
            if (player.currentCell + player.direction == block.currentCell)
            {
                if (player.state == State::premove)
                {
                    player.state = State::idle;
                }
                if (player.state == State::postmove)
                {
                    player.state = State::idle;

                    if (block.type == BlockType::block)
                    {
                        block.state = State::postmove;
                        block.direction = player.direction;
                    }
                }
                break;
            }
        }

        if (player.state == State::premove || player.state == State::postmove)
        {
            player.state = State::moving;
            player.targetCell = player.currentCell + player.direction;
        }
    }

    for (Block& blockActive : blocks)
    {
        if (blockActive.state == State::postmove)
        {   
            if (glm::abs(blockActive.currentCell.x + blockActive.direction.x) > 5 ||
                glm::abs(blockActive.currentCell.y + blockActive.direction.y) > 5)
            {
                blockActive.state = State::idle;
                continue;
            }
            
            for (Block& blockChecked : blocks)
            {
                if (blockActive.currentCell + blockActive.direction == blockChecked.currentCell)
                {
                    blockActive.state = State::idle;

                    if (blockChecked.type == BlockType::block)
                    {
                        blockChecked.state = State::postmove;
                        blockChecked.direction = blockActive.direction;
                    }

                    break;
                }
            }

            if (blockActive.state == State::postmove)
            {
                blockActive.targetCell = blockActive.currentCell + blockActive.direction;
                blockActive.state = State::moving;
            }
        }
    }
}

void World::Render(float lag)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glm::mat4 renderTransform = glm::mat4(1.0f);
    renderTransform = projection * view * renderTransform;

    floor.Draw(renderTransform);
    player.Draw(renderTransform, lag);
    for (Block& block : blocks)
    {
        block.Draw(renderTransform, lag);
    }
}