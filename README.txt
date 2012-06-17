To compile, do:

  $ gcc rays.c sdlmain.c -lSDL -std=gnu99 -O3 -fopenmp

or something to that effect.

On OSX you'll need something more like

  $ gcc SDLmain.m rays.c sdlmain.c -framework SDL -framework Cocoa -std=gnu99 -O3 -fopenmp
