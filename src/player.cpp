#include "player.hpp"

Player::Player(Renderer& renderer) :
    cube(renderer)
{
    
}

void Player::Draw(glm::mat4 renderTransform)
{
    cube.Draw(renderTransform);
}