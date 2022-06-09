#include "world.hpp"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

Entity::Entity(Renderer& renderer, std::string renderType) :
    render(renderer.renders[renderType]),
    model(glm::mat4(1.0f))
{
    
}

void Entity::Draw(glm::mat4 renderTransform)
{
    glBindVertexArray(render.vao);
    glUseProgram(render.program);

    glActiveTexture(GL_TEXTURE0);
    glUniform1i(render.textureLocation, 0);
    glUniformMatrix4fv(render.transformLocation, 1, GL_FALSE, glm::value_ptr(renderTransform * model));
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glUseProgram(0);
    glBindVertexArray(0);
}

void World::Initialize(Renderer& renderer)
{
    projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);

    view = glm::mat4(1.0f);
    view = glm::lookAt(glm::vec3(0.0f, 10.0f, 15.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    Entity floor = Entity(renderer, "floor");
    entities.emplace_back(floor);
}

void World::Update()
{
    
}

void World::Render(Renderer& renderer)
{
    glm::mat4 renderTransform = glm::mat4(1.0f);
    renderTransform = projection * view * renderTransform;

    for (Entity entity : entities)
    {
        entity.Draw(renderTransform);
    }
}