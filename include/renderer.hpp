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
    Render(BufferInfo info, GLuint p);
    
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

struct Renderer
{
    Renderer();
    
    Render renderCube;
    Render renderFloor;
    
    void AddRender(std::string type);

    void BindState(Render& render);
    void DrawBuffer(Render& render);

private:
    BufferInfo ExtractDataFromFile(std::string type);
};

#endif