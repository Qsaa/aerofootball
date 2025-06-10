#pragma once

#include <SDL3/SDL.h>
#include <array>

#include "entity.hpp"

extern SDL_Window* window;
extern SDL_Renderer* renderer;

using Entities = std::array<Entity, 3>;
extern Entities entities;
