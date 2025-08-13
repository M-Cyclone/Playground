#include "core/math/math.h"

#include <SDL3/SDL.h>

#include <string>

int main(int argc, char** argv)
{
    if (!SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMEPAD | SDL_INIT_EVENTS))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL initialization failed: %s.\n", SDL_GetError());
        return -1;
    }

    SDL_Quit();

    Vec2 vec_test(3.0f, 4.0f);
    return 0;
}
