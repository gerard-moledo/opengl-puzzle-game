#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <string>

#include "renderer.hpp"
#include "models.hpp"
#include "player.hpp"

struct World
{
    World(Renderer& renderer);
    
    FloorModel floor;
    Player player;
    std::vector<CubeModel> cubes;
    
    glm::mat4 projection;
    glm::mat4 view;
    
    void Initialize(Renderer& renderer);
    void Update(float dt);
    void Render();
};

#endif