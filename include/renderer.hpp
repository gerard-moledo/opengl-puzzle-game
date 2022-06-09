#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <unordered_map>
#include <string>

#include "glad/glad.h"
#include "glm/glm.hpp"

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
    
    GLuint vao;
    GLuint arrayBuffer;
    GLuint elementBuffer;

    GLuint texture;
    
    GLuint program;
    GLint transformLocation;
    GLint colorLocation;
    GLint textureLocation;
};

struct Renderer
{
    std::unordered_map<std::string, Render> renders;
    
    void AddRender(std::string type);
    void Draw(glm::mat4 transform, glm::mat4 model);

private:
    BufferInfo ExtractDataFromFile(std::string type);
    
    GLuint CreateShader(std::string filename);
    void ReadShaderFromFile(std::string& shader, std::string file);
};

#endif