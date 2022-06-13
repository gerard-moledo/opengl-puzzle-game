#ifndef SYSTEM_H
#define SYSTEM_H

#include "glad/glad.h"
#include "glfw/glfw3.h"

#include <string>
#include <unordered_map>

namespace System
{
    extern GLFWwindow* window;
    extern std::unordered_map<std::string, GLuint> programMap;
    
    void Initialize();

    GLuint CreateShader(std::string filename);
    void ReadShaderFromFile(std::string& shader, std::string file);
    
    void Quit();
};

#endif