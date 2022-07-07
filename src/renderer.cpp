#include "renderer.hpp"

#include <fstream>

#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"

#include "system.hpp"

//===========================================
// RENDERER
//===========================================

Renderer::Renderer(Render render) :
    active(render)
{
    
}

void Renderer::BindState(GLuint texture)
{
    glBindVertexArray(active.vao);
    glUseProgram(active.program);
    if (texture)
    {
        glBindTexture(GL_TEXTURE_2D, texture);
    }
}

void Renderer::SetUniforms(glm::mat4 transform, glm::vec4 color, GLuint textureUnit, glm::vec2 texOffset, glm::vec2 texScale)
{
    glUniformMatrix4fv(active.transformLocation, 1, GL_FALSE, glm::value_ptr(transform));

    glUniform4f(active.colorLocation, color.r, color.g, color.b, color.a);
    
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glUniform1i(active.samplerLocation, textureUnit);
    glUniform2f(active.texOffsetLocation, texOffset.x, texOffset.y);
    glUniform2f(active.texScaleLocation, texScale.x, texScale.y);
}

void Renderer::DrawBuffer()
{
    GLint buffer;
    glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &buffer);
    
    if (buffer == 0)
    {
        glDrawArrays(GL_TRIANGLES, 0, active.bufferInfo.arrayBufferData.size() / (active.bufferInfo.layouts[0].stride / sizeof(float)));
    }
    else
    {
        glDrawElements(GL_TRIANGLES, active.bufferInfo.elementBufferData.size(), GL_UNSIGNED_INT, 0);
    }
}