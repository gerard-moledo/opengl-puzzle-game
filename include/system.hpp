#ifndef SYSTEM_H
#define SYSTEM_H

#define GLFW_INCLUDE_NONE
#include "glad/glad.h"
#include "glfw/glfw3.h"

#include <string>
#include <unordered_map>

#include "render.hpp"

namespace System
{
    extern int width;
    extern int height;

    extern GLFWwindow* window;
    extern std::unordered_map<std::string, GLuint> programMap;

    extern Render renderFloor;
    extern Render renderCube;
    
    void Initialize();

    GLuint CreateShader(std::string filename);
    void ReadShaderFromFile(std::string& shader, std::string file);

    BufferInfo ExtractDataFromFile(std::string type);
    
    void Quit();
};

#endif