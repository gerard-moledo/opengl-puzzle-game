#include "models.hpp"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "system.hpp"

FloorModel::FloorModel() :
    renderer(System::renderFloor),
    texture(System::textureFloor),
    model(glm::mat4(1.0f)),
    scale(glm::vec2(1.0f))
{
    model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
}

void FloorModel::Draw(glm::mat4 worldTransform)
{
    renderer.BindState(texture.ID);

    renderer.SetUniforms(worldTransform * model, glm::vec4(1.0f), 0, glm::vec2(0.0f), scale);

    renderer.DrawBuffer();
}

CubeModel::CubeModel() :
    renderer(System::renderCube),
    model(glm::mat4(1.0f)),
    color(glm::vec4(1.0f))
{
    
}

void CubeModel::Draw(glm::mat4 worldTransform)
{
    renderer.BindState(0);

    renderer.SetUniforms(worldTransform * model, color, 0, glm::vec2(0.0f), glm::vec2(1.0f));

    renderer.DrawBuffer();
}

UI::UI(Vector2i position, glm::vec2 size, glm::vec4 tint, Texture tex, int glyph) :
    renderer(System::renderUI),
    texture(tex),
    model(glm::mat4(1.0f)),
    offset(glm::vec2(0.0f)),
    scale(glm::vec2(1.0f)),
    color(tint),
    glyphIndex(glyph)
{
    model = glm::translate(model, glm::vec3((float) position.x, (float) position.y, 0.0f));
    model = glm::scale(model, glm::vec3(size.x, size.y, 1.0f));

    if (glyph >= 0)
    {
        offset = glm::vec2(glyph % 8 * 32 / 256.0f, glyph / 8 * 32 / 256.0f);
        scale = glm::vec2(32 / 256.0f);
    }
}

void UI::Draw(glm::mat4 uiTransform)
{
    renderer.BindState(texture.ID);

    renderer.SetUniforms(uiTransform * model, color, 0, offset, scale);
    
    renderer.DrawBuffer();
}