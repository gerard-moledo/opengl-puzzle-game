#include "game.hpp"

#include <cstdio>

#include "system.hpp"

Game::Game() :
    renderer(),
    world(renderer)
{
    
}

void Game::Start()
{
    glViewport(0, 0, 1280, 720);
    glEnable(GL_DEPTH_TEST);

    world.Initialize(renderer);
}

void Game::Run()
{
    Start();

    while (!glfwWindowShouldClose(System::window))
    {
        glfwPollEvents();

        world.Update();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        world.Render(renderer);
        
        glfwSwapBuffers(System::window);
    }
}

void Game::Quit()
{
    
}