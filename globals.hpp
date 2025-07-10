#pragma once

#include <SDL3/SDL.h>
#include <array>

#include "entity.hpp"
#include "keyboard_state.hpp"

extern SDL_Window* window;
extern SDL_Renderer* renderer;

using Entities = std::array<Entity, 4>;
extern Entities entities;

extern KeyboardState input;
