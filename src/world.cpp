#include "world.hpp"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

World::World(Renderer& renderer) :
    floor(renderer),
    player(renderer),
    blocks(0, Block(renderer, { 0, 0 })),
    projection(glm::mat4(1.0f)),
    view(glm::mat4(1.0f))
{
    projection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);
    view = glm::lookAt(glm::vec3(0.0f, 25.0f, 15.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void World::Initialize(Renderer& renderer)
{
    blocks.emplace_back(Block(renderer, { 5, 2 }));
    blocks.emplace_back(Block(renderer, { -5, 0 }));
}

void World::Update(float dt)
{
    GetState(state);
    
    player.Update(dt, state.blockStates);
}

void World::GetState(WorldState& state)
{
    state.playerCell = player.currentCell;

    for (Block block : blocks)
    {
        state.blockStates.emplace_back(std::make_pair(block.type, block.currentCell));
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