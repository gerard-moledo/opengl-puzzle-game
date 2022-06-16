#include "player.hpp"

#include "glfw/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"

#include "system.hpp"

Player::Player(Renderer& renderer) :
    cube(renderer),
    currentPos({ 0, 0 }),
    targetPos(currentPos)
{
    
}

void Player::Update()
{
    if (state == State::idle)
    {
        if (glfwGetKey(System::window, GLFW_KEY_D) == GLFW_PRESS)
        {
            targetPos.x += 1;
            state = State::moving;
        }
    }
    if (state == State::moving)
    {
        if (glfwGetKey(System::window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            currentPos.x = targetPos.x;
            state = State::idle;
        }
    }
}

void Player::Draw(glm::mat4 renderTransform)
{
    UpdateModel();
    
    cube.Draw(renderTransform);
}

void Player::UpdateModel()
{
    cube.model = glm::mat4(1.0f);
    cube.model = glm::translate(cube.model, glm::vec3(currentPos.x * 2.0f, 0.0f, currentPos.y * -2.0f));
}