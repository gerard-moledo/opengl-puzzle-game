#ifndef SHADER_H
#define SHADER_H

#include "glad/glad.h"
#include "glm/glm.hpp"

#include <vector>
#include <string>

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
    Render(BufferInfo info, GLuint p);
    
    GLuint vao;
    GLuint arrayBuffer;
    GLuint elementBuffer;
    GLuint program;
    
    GLint transformLocation;
    GLint colorLocation;
    GLint textureLocation;

    GLuint textureUnit;
};

struct Renderer
{
    std::vector<Render> renders;
    
    void AddRender(std::string type);
    void Draw(glm::mat4 transform, glm::mat4 model);

private:
    BufferInfo ExtractDataFromFile(std::string type);
    
    GLuint CreateShader(std::string filename);
    void ReadShaderFromFile(std::string& shader, std::string file);
};

#endif