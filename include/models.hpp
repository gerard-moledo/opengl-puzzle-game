#ifndef MODELS_H
#define MODELS_H

#include "glm/glm.hpp"

#include "renderer.hpp"
#include "utils.hpp"

class FloorModel
{
public:
    FloorModel();

    Renderer renderer;
    Texture texture;
    
    glm::mat4 model;
    glm::vec2 scale;
    
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

class UI
{
public:
    UI(Vector2i position, glm::vec2 size, glm::vec4 tint, Texture tex, int glyph);

    Renderer renderer;
    Texture texture;
    
    glm::mat4 model;
    glm::vec2 offset;
    glm::vec2 scale;
    glm::vec4 color;

    int glyphIndex;

    void Draw(glm::mat4 uiTransform);
};

#endif