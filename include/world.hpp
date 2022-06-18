#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <string>
#include <utility>

#include "renderer.hpp"
#include "models.hpp"
#include "player.hpp"
#include "block.hpp"
#include "utils.hpp"

struct WorldState
{
    Vector2i playerCell;
    BlockStates blockStates;
};

struct World
{
    World(Renderer& renderer);

    WorldState state;
    
    FloorModel floor;
    Player player;
    std::vector<Block> blocks;
    
    glm::mat4 projection;
    glm::mat4 view;
    
    void Initialize(Renderer& renderer);
    void Update(float dt);
    void Render(float lag);

private:
    void GetState(WorldState& state);
};

#endif