#ifndef GAME_H
#define GAME_H

#define GLFW_INCLUDE_NONE
#include "glad/glad.h"
#include "GLFW/glfw3.h"
 
#include "world.hpp"
#include "renderer.hpp"

struct Game
{
    Game();

    Renderer renderer;
    World world;

    float tPrev = 0.0f;
    float stackedTime = 0.0f;
    float timestep = 0.02f;
    int frames = 0;
    float fpsTime = 0.0f;
    
    void Run();
    void Quit();

    void CalculateFramerate(float dt);
    
private:
    void Start();
};

#endif