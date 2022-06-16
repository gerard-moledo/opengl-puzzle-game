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

void Player::Update(float dt)
{
    if (state == State::idle)
    {
        Vector2i direction{ 0, 0 };
        
        if (glfwGetKey(System::window, GLFW_KEY_D) == GLFW_PRESS)   direction.x = 1;
        if (glfwGetKey(System::window, GLFW_KEY_A) == GLFW_PRESS)   direction.x = -1;
        if (glfwGetKey(System::window, GLFW_KEY_W) == GLFW_PRESS)   direction.y = 1;
        if (glfwGetKey(System::window, GLFW_KEY_S) == GLFW_PRESS)   direction.y = -1;

        if (direction.x != 0 || direction.y != 0)
        {
            targetPos.x += direction.x;
            targetPos.y += direction.y;
            
            state = State::moving;
            tMove = 0.0f;
        }
    }
    if (state == State::moving)
    {
        tMove += dt;

        if (tMove >= 1.0f)
        {
            currentPos.x = targetPos.x;
            currentPos.y = targetPos.y;
            state = State::idle;

            tMove = 0.0f;
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
    glm::vec3 glmCurrent = glm::vec3(currentPos.x, 0.0f, currentPos.y);
    glm::vec3 glmTarget = glm::vec3(targetPos.x, 0.0f, targetPos.y);
    glm::vec3 lerpPos = glm::mix(glmCurrent, glmTarget, tMove);
    
    cube.model = glm::mat4(1.0f);
    cube.model = glm::translate(cube.model, glm::vec3(lerpPos.x * 2.0f, 0.0f, lerpPos.z * -2.0f));
}