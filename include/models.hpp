#ifndef MODELS_H
#define MODELS_H

#include "glm/glm.hpp"

#include "renderer.hpp"

class FloorModel
{
public:
    FloorModel();

    Renderer renderer;
    glm::mat4 model;
    glm::vec3 scale;
    
    void Draw(glm::mat4 worldTransform);
};

class CubeModel
{
public:
    CubeModel();

    Renderer renderer;
    glm::mat4 model;
    glm::vec4 color;

    void Draw(glm::mat4 worldTransform);
};

class Text
{
public:
    Text();

    Renderer renderer;
    glm::mat4 model;

    void Draw();
};

#endif