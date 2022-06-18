#include "player.hpp"

#include "glfw/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"

#include "system.hpp"

Player::Player(Renderer& renderer) :
    cube(renderer)
{
    cube.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
}

void Player::Update(float dt)
{
    worldPosPrev = worldPos;

    if (currentCell == targetCell)
    {
        ReceiveInput();
        
        if (direction != Vector2i { 0, 0 })
        {
            CheckState(true);
        }
    }
    
    if (targetCell != currentCell)
    {
        bool done = Move(dt);

        if (done)
        {
            currentCell = targetCell;
            
            CheckState(false);
        }
    }
}

void Player::ReceiveInput()
{   
    direction.x = 0;
    direction.y = 0;

    if (glfwGetKey(System::window, GLFW_KEY_D) == GLFW_PRESS)   direction.x = 1;
    if (glfwGetKey(System::window, GLFW_KEY_A) == GLFW_PRESS)   direction.x = -1;
    if (glfwGetKey(System::window, GLFW_KEY_W) == GLFW_PRESS)   direction.y = 1;
    if (glfwGetKey(System::window, GLFW_KEY_S) == GLFW_PRESS)   direction.y = -1;
}

void Player::CheckState(bool starting)
{
    Vector2i nextCell = targetCell + direction;
    if (!(fabsf(nextCell.x) > 5 || fabsf(nextCell.y) > 5))
    {
        targetCell = nextCell;

        tMove = 0.0f;
        accelerating = starting;
    }
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