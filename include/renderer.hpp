#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <unordered_map>
#include <string>

#include "glad/glad.h"
#include "glm/glm.hpp"

#include "system.hpp"

struct Renderer
{
    Renderer();

    Render active;

    void BindState();
    void SetUniforms(glm::mat4 transform, glm::vec4 color, GLuint texture, glm::vec3 scale);
    void DrawBuffer();
};

#endif