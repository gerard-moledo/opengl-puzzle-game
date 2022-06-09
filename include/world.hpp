#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <string>

#include "renderer.hpp"

struct Entity
{
    Entity(Renderer& renderer, std::string renderType);

    Render& render;
    glm::mat4 model;

    void Draw(glm::mat4 renderTransform);
};

struct World
{
    std::vector<Entity> entities;
    
    glm::mat4 projection;
    glm::mat4 view;
    
    void Initialize(Renderer& renderer);
    void Update();
    void Render(Renderer& renderer);
};

#endif