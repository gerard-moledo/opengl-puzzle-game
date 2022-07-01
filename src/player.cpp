#include "player.hpp"

#include "glfw/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"

#include "system.hpp"
#include "world.hpp"

Player::Player() :
    cube()
{
    cube.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
}

void Player::Update(float dt)
{
    worldPosPrev = worldPos;

    if (state == State::idle)
    {
        ReceiveInput();
        
        if (direction != Vector2i { 0, 0 })
        {
            accelerating = true;

            state = State::premove;
        }
    }
    
    if (state == State::moving)
    {
        bool done = Move(dt);

        if (done)
        {
            tMove = 0.0f;
            accelerating = false;
            currentCell = targetCell;

            state = State::postmove;
        }
    }
}

void Player::ReceiveInput()
{   
    direction.x = 0;
    direction.y = 0;

    if (glfwGetKey(System::window, GLFW_KEY_D) == GLFW_PRESS) { direction.x = 1; return; }
    if (glfwGetKey(System::window, GLFW_KEY_A) == GLFW_PRESS) { direction.x = -1; return; }
    if (glfwGetKey(System::window, GLFW_KEY_W) == GLFW_PRESS) { direction.y = 1; return; }
    if (glfwGetKey(System::window, GLFW_KEY_S) == GLFW_PRESS) { direction.y = -1; return; }
}

bool Player::Move(float dt)
{
    tMove += dt;

    float tSolved = sqrtf(1.0f / SPEED);
    float tLerp = accelerating ? tMove * tMove * SPEED : tMove * 2 * SPEED * tSolved;

    glm::vec3 glmCurrent = glm::vec3(currentCell.x, 0.0f, currentCell.y);
    glm::vec3 glmTarget = glm::vec3(targetCell.x, 0.0f, targetCell.y);
    worldPos = glm::mix(glmCurrent, glmTarget, glm::clamp(tLerp, 0.0f, 1.0f));

    return tLerp >= 1.0f;
}

void Player::Draw(glm::mat4 renderTransform, float lag)
{
    glm::vec3 renderPos = glm::mix(worldPosPrev, worldPos, lag);
    
    cube.model = glm::mat4(1.0f);
    cube.model = glm::translate(cube.model, glm::vec3(renderPos.x * 2.0f, 0.0f, renderPos.z * -2.0f));
    
    cube.Draw(renderTransform);
}