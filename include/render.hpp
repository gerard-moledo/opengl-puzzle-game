#ifndef RENDER_H
#define RENDER_H

#include "glad/glad.h"
#include "glfw/glfw3.h"

#include <vector>

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
    // Default constructor necessary since System namespace
    // requires default member
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
    GLint texScaleLocation;
};

#endif