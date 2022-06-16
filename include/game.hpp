#ifndef GAME_H
#define GAME_H

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

    void Run();
    void Quit();

private:
    void Start();
};

#endif