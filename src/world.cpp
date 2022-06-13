#include "world.hpp"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

Floor::Floor(Render& render) :
    render(render),
    model(glm::mat4(1.0f))
{
    model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
}

void Floor::Draw(Renderer& renderer, glm::mat4 worldTransform)
{
    renderer.BindState(render);

    glActiveTexture(GL_TEXTURE0);
    glUniform1i(render.textureLocation, 0);
    glUniformMatrix4fv(render.transformLocation, 1, GL_FALSE, glm::value_ptr(worldTransform * model));
    
    renderer.DrawBuffer(render);
}

Cube::Cube(Render& render) :
    render(render),
    model(glm::mat4(1.0f)),
    color(glm::vec3(1.0f))
{
    
}

void Cube::Draw(Renderer& renderer, glm::mat4 worldTransform)
{
    renderer.BindState(render);

    glUniformMatrix4fv(render.transformLocation, 1, GL_FALSE, glm::value_ptr(worldTransform * model));
    glUniform4f(render.colorLocation, color.r, color.g, color.b, 1.0f);

    renderer.DrawBuffer(render);
}

World::World(Renderer& renderer) :
    floor(renderer.renderFloor),
    cubes(),
    projection(glm::mat4(1.0f)),
    view(glm::mat4(1.0f))
{
    projection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);
    view = glm::lookAt(glm::vec3(0.0f, 25.0f, 15.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void World::Initialize(Renderer& renderer)
{
    Cube cube = Cube(renderer.renderCube);
    cubes.emplace_back(cube);
}

void World::Update()
{
    
}

void World::Render(Renderer& renderer)
{
    glm::mat4 renderTransform = glm::mat4(1.0f);
    renderTransform = projection * view * renderTransform;

    floor.Draw(renderer, renderTransform);
    for (Cube cube : cubes)
    {
        cube.Draw(renderer, renderTransform);
    }
}