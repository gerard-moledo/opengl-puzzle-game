#include "render.hpp"

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
        if (layout.size > 0)
        {
            glVertexAttribPointer(
                layout.location, layout.size,
                GL_FLOAT, GL_FALSE,
                layout.stride, (void*)layout.offset
            );
            glEnableVertexAttribArray(layout.location);
        }
    }

    glBindVertexArray(0);

    bufferInfo = info;
    program = p;

    transformLocation = glGetUniformLocation(program, "transform");
    colorLocation = glGetUniformLocation(program, "color");
    textureLocation = glGetUniformLocation(program, "textureSampler");
    texScaleLocation = glGetUniformLocation(program, "texScale");
}