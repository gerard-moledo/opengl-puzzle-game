#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "game.hpp"

int main()
{
    Game game;
    
    game.Initialize();
    game.Run();
    game.Quit();
    
    return 0;
}