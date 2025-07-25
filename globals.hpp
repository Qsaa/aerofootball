#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <array>

#include "entity.hpp"
#include "events/event_processor.hpp"
#include "keyboard_state.hpp"

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern TTF_Font* font;
extern TTF_Font* fontLarge;

using Entities = std::array<Entity, 16>;
extern Entities entities;

extern KeyboardState input;

extern EventProcessor events;

extern int redScore;
extern int blueScore;
