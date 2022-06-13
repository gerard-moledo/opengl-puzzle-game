#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "system.hpp"
#include "game.hpp"

int main()
{
    System::Initialize();
    
    Game game = Game();
    game.Run();

    System::Quit();
    
    return 0;
}