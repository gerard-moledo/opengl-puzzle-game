#include "models.hpp"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

FloorModel::FloorModel(Renderer& renderer) :
    renderer(renderer),
    model(glm::mat4(1.0f))
{
    model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
}

void FloorModel::Draw(glm::mat4 worldTransform)
{
    renderer.active = System::renderFloor;
    
    renderer.BindState();

    renderer.SetUniforms(worldTransform * model, glm::vec4(0.0f), 0);

    renderer.DrawBuffer();
}

CubeModel::CubeModel(Renderer& renderer) :
    renderer(renderer),
    model(glm::mat4(1.0f)),
    color(glm::vec4(1.0f))
{

}

void CubeModel::Draw(glm::mat4 worldTransform)
{
    renderer.active = System::renderCube;

    renderer.BindState();

    renderer.SetUniforms(worldTransform * model, color, 0);

    renderer.DrawBuffer();
}