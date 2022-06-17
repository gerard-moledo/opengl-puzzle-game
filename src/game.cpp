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
        float tCurrent = (float)glfwGetTime();
        float dt = tCurrent - tPrev;
        tPrev = tCurrent;
        stackedTime += dt;
        
        CalculateFramerate(dt);
        
        glfwPollEvents();
        
        while (stackedTime > timestep)
        {
            stackedTime -= timestep;
            
            world.Update(timestep);
        }

        float lag = stackedTime / timestep;
        world.Render(lag);
        
        glfwSwapBuffers(System::window);
    }
}

Game::CalculateFramerate(float dt)
{
    fpsTime += dt;
    frames++;
    if (fpsTime > 1.0f)
    {
        fpsTime -= 1.0f;
        printf("%d\n", frames);
        frames = 0;
    }
}

void Game::Quit()
{
    
}