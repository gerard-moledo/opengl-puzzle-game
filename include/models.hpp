#ifndef MODELS_H
#define MODELS_H

#include "glm/glm.hpp"

#include "renderer.hpp"

struct FloorModel
{
    FloorModel(Renderer& renderer);

    Renderer renderer;
    glm::mat4 model;

    void Draw(glm::mat4 worldTransform);
};

struct CubeModel
{
    CubeModel(Renderer& renderer);

    Renderer renderer;
    glm::mat4 model;
    glm::vec4 color;

    void Draw(glm::mat4 worldTransform);
};

#endif