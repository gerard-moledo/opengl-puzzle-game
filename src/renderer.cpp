#include "renderer.hpp"

#include <fstream>

#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"
#include "stb_image.h"

#include "system.hpp"

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
            layout.stride, (void*) layout.offset
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

//===========================================
// RENDERER
//===========================================

Renderer::Renderer() :
    renderFloor(ExtractDataFromFile("floor"), System::programMap["floor"]),
    renderCube(ExtractDataFromFile("cube"), System::programMap["cube"])
{
    
}

void Renderer::BindState(Render& render)
{
    glBindVertexArray(render.vao);
    glUseProgram(render.program);
}

void Renderer::DrawBuffer(Render& render)
{
    GLint buffer;
    glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &buffer);
    
    if (buffer == 0)
    {
        glDrawArrays(GL_TRIANGLES, 0, render.bufferInfo.arrayBufferData.size() / (render.bufferInfo.layouts[0].stride / sizeof(float)));
    }
    else
    {
        glDrawElements(GL_TRIANGLES, render.bufferInfo.elementBufferData.size(), GL_UNSIGNED_INT, 0);
    }
}

BufferInfo Renderer::ExtractDataFromFile(std::string type)
{
    BufferInfo data;

    std::ifstream filestream {};
    filestream.open(type + ".data");

    std::string bufferType;
    std::vector<int> layoutSizes;
    int size;
    float arrayBufferDatum;
    unsigned int elementBufferDatum;

    while (filestream.peek() != EOF)
    {
        filestream >> bufferType;

        if (bufferType[0] == 'A')
        {
            int totalSize = 0;
            while (filestream.peek() != '\n')
            {
                filestream >> size;
                layoutSizes.emplace_back(size);
                totalSize += size;
            }

            int offset = 0;
            for (int sizeIndex = 0; sizeIndex < layoutSizes.size(); sizeIndex++)
            {
                BufferLayout layout;
                layout.location = sizeIndex;
                layout.size = layoutSizes[sizeIndex];
                layout.stride = sizeof(float) * totalSize;
                layout.offset = sizeof(float) * offset;
                data.layouts.emplace_back(layout);

                offset += layout.size;
            }

            filestream.clear();
            filestream.get();

            while (filestream >> arrayBufferDatum)
            {
                data.arrayBufferData.emplace_back(arrayBufferDatum);
            }
            filestream.clear();
        }
        if (bufferType[0] == 'E')
        {
            while (filestream >> elementBufferDatum)
            {
                data.elementBufferData.emplace_back(elementBufferDatum);
            }
            filestream.clear();
        }
    }

    return data;
}