#ifndef GAME_H
#define GAME_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"
 
#include "world.hpp"
#include "renderer.hpp"

struct Game
{
    GLFWwindow* window;

    Renderer renderer;
    World world;
    
    void Initialize();
    void Run();
    void Quit();

private:
    void Start();
};

#endif