#include <SDL/SDL.h>
#include <stdio.h>
#include "rays.h"
#include <math.h>


int main(int argc, char *argv[])
{
  SDL_Surface *screen;
  SDL_Event e;

  sphere spheres[] = {
    {{-3*sin(M_PI/6), 11-3*cos(M_PI/6), 0}, 1.0},
    {{0, 11, 0}, 3.0},
    {{0, 11-3*cos(M_PI/6), 3*sin(M_PI/6)}, 1.0},
    {{0, 11, -2005}, 2000},
    {{-5, 11, -4}, 2}
  };
  
  light lights[] = {
    {{-2, 6, 8}, 7},
    {{0, 4, -1}, 1}
  };


  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
  screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);

  printf("%08x\n", screen->format->Rmask);
  printf("%08x\n", screen->format->Gmask);
  printf("%08x\n", screen->format->Bmask);

  SDL_LockSurface(screen);
  dotrace(640, 480, spheres, 5, lights, 2,
          screen->pixels);
  SDL_UnlockSurface(screen);
  SDL_Flip(screen);

  while (1) {
    int changes = 0;
    SDL_WaitEvent(&e);
    do {
      if (e.type==SDL_QUIT) return 0;
      else if (e.type==SDL_MOUSEMOTION) {
        if (e.motion.state & SDL_BUTTON(1)) {
          spheres[4].o.x += (e.motion.xrel / 50.0);
          spheres[4].o.z -= (e.motion.yrel / 50.0);
          changes = 1;
        }
      }
    } while (SDL_PollEvent(&e));
    if (!changes) continue;

    SDL_LockSurface(screen);
    dotrace(640, 480, spheres, 5, lights, 2,
            screen->pixels);
    SDL_UnlockSurface(screen);
    SDL_Flip(screen);
  }
  return 0;
}
