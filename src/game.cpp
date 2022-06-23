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
    glViewport(0, 0, System::width, System::height);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    world.Initialize();
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

void Game::CalculateFramerate(float dt)
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