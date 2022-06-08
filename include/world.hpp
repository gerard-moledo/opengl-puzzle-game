#ifndef WORLD_H
#define WORLD_H

#include "renderer.hpp"

struct World
{
    glm::mat4 cubeModel;

    glm::mat4 projection;
    glm::mat4 view;
    
    void Initialize();
    void Update();
    void Render(Renderer& renderer);
};

#endif