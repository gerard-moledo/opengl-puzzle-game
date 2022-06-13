#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <string>

#include "renderer.hpp"

struct Floor
{
    Floor(Render& render);

    Render& render;
    glm::mat4 model;
    
    void Draw(Renderer& renderer, glm::mat4 worldTransform);
};

struct Cube
{
    Cube(Render& render);

    Render& render;
    glm::mat4 model;
    glm::vec3 color;
    
    void Draw(Renderer& renderer, glm::mat4 worldTransform);
};

struct World
{
    World(Renderer& renderer);
    
    Floor floor;
    std::vector<Cube> cubes;
    
    glm::mat4 projection;
    glm::mat4 view;
    
    void Initialize(Renderer& renderer);
    void Update();
    void Render(Renderer& renderer);
};

#endif