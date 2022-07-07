#include "game.hpp"

#include <cstdio>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "system.hpp"

void GameLoop(void* instance);

Game::Game() :
    world()
{
    
}

void Game::Start()
{
#ifndef __EMSCRIPTEN__
   // glfwSwapInterval(1);
#endif
    
    world.Initialize();
}

void Game::Run()
{
    Start();

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(GameLoop, this, 0, 1);
#else
    while (!glfwWindowShouldClose(System::window))
    {
        GameLoop(this);
    }
#endif
    
}

void GameLoop(void* instance)
{
    Game* GAME = (Game*)instance;

#ifdef __EMSCRIPTEN__
    double time = emscripten_get_now() / 1000;
#else
    double time = glfwGetTime();
#endif

    float tCurrent = (float)time;
    float dt = tCurrent - GAME->tPrev;
    GAME->tPrev = tCurrent;
    GAME->stackedTime += dt;
    GAME->CalculateFramerate(dt);

    glfwPollEvents();

    while (GAME->stackedTime > GAME->timestep)
    {
        GAME->stackedTime -= GAME->timestep;

        GAME->world.Update(GAME->timestep);
    }

    float lag = GAME->stackedTime / GAME->timestep;
    GAME->world.Render(lag);

    glfwSwapBuffers(System::window);
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