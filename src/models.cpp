#include "models.hpp"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "system.hpp"

FloorModel::FloorModel() :
    renderer(System::renderFloor),
    model(glm::mat4(1.0f)),
    scale(glm::vec3(1.0f))
{
    model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
}

void FloorModel::Draw(glm::mat4 worldTransform)
{
    renderer.BindState();

    renderer.SetUniforms(worldTransform * model, glm::vec4(0.0f), 0, scale);

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
    renderer.BindState();

    renderer.SetUniforms(worldTransform * model, color, 0, glm::vec3(1.0f));

    renderer.DrawBuffer();
}

Text::Text() :
    renderer(System::renderText),
    model(glm::mat4(1.0f))
{
    
}

void Text::Draw()
{
    
}