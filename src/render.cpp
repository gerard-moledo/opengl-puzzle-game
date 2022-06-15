#include "render.hpp"

#include "stb_image.h"

Render::Render() { }

Render::~Render() { }

Render::Render(BufferInfo info, GLuint p)
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &arrayBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, arrayBuffer);
    glBufferData(GL_ARRAY_BUFFER, info.arrayBufferData.size() * sizeof(float), info.arrayBufferData.data(), GL_STATIC_DRAW);

    if (info.elementBufferData.size() > 0)
    {
        glGenBuffers(1, &elementBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, info.elementBufferData.size() * sizeof(unsigned int), info.elementBufferData.data(), GL_STATIC_DRAW);
    }

    for (BufferLayout layout : info.layouts)
    {
        glVertexAttribPointer(
            layout.location, layout.size,
            GL_FLOAT, GL_FALSE,
            layout.stride, (void*)layout.offset
        );
        glEnableVertexAttribArray(layout.location);
    }

    glBindVertexArray(0);

    bufferInfo = info;
    program = p;

    if (glGetAttribLocation(program, "aTexCoord") != -1)
    {
        int width, height, channelCount;
        unsigned char* data = stbi_load("Assets/Tile.png", &width, &height, &channelCount, 0);
        if (data)
        {
            glGenTextures(1, &texture);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        stbi_image_free(data);
    }

    transformLocation = glGetUniformLocation(program, "transform");
    colorLocation = glGetUniformLocation(program, "color");
    textureLocation = glGetUniformLocation(program, "textureSampler");
}