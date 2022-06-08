#include "world.hpp"

#include "glm/gtc/matrix_transform.hpp"

void World::Initialize()
{
    projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);

    view = glm::mat4(1.0f);
    view = glm::lookAt(glm::vec3(0.0f, 10.0f, 15.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    cubeModel = glm::mat4(1.0f);
}

void World::Update()
{
    
}

void World::Render(Renderer& renderer)
{
    glm::mat4 renderTransform = glm::mat4(1.0f);
    renderTransform = projection * view * renderTransform;

    renderer.Draw(renderTransform, cubeModel);
}