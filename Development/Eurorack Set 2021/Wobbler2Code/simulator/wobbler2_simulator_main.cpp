#include "SDL.h" 

int main(int argc, char *argv[])
{
	AllocConsole();
  
  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window *window = SDL_CreateWindow(
    "Wobbler2 Simulator",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    1280,
    1024,
    0
  );

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);

  SDL_Delay(3000);

  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}