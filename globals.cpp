#include "globals.hpp"

#include <SDL3/SDL.h>
#include <array>

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

Entities entities;
KeyboardState input;
EventProcessor events;
