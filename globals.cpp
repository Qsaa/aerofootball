#include "globals.hpp"

#include <array>

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
TTF_Font* font = nullptr;

Entities entities;
KeyboardState input;
EventProcessor events;
int redScore = 0;
int blueScore = 0;
