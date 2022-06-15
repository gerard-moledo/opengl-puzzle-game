#ifndef SYSTEM_H
#define SYSTEM_H

#include "glad/glad.h"
#include "glfw/glfw3.h"

#include <vector>
#include <string>
#include <unordered_map>

struct BufferLayout
{
    int location;
    GLint size;
    int stride;
    int offset;
};

struct BufferInfo
{
    std::vector<float> arrayBufferData;
    std::vector<unsigned int> elementBufferData;
    std::vector<BufferLayout> layouts;
};

struct Render
{
    Render();
    Render(BufferInfo info, GLuint p);
    ~Render();

    GLuint vao;
    GLuint arrayBuffer;
    GLuint elementBuffer;

    GLuint texture;

    BufferInfo bufferInfo;
    GLuint program;
    GLint transformLocation;
    GLint colorLocation;
    GLint textureLocation;
};

namespace System
{
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