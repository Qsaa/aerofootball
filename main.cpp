/*
  Copyright (C) 1997-2025 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely.
*/
#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL_main.h>

#include <cmath>
#include <utility>
#include <vector>
#include <array>

#include "entity.hpp"

#include "globals.hpp"

#include "components/texture.hpp"
#include "components/position.hpp"
#include "components/size.hpp"


std::array<Entity, 3> entities;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    /* Create the window */
    if (!SDL_CreateWindowAndRenderer("Hello World", 800, 600, SDL_WINDOW_FULLSCREEN, &window, &renderer)) {
        SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    
    int w = 0, h = 0;
    SDL_GetRenderOutputSize(renderer, &w, &h);
    // load the PNG
    auto png_surface = IMG_Load("../fieldScreen.png");
    if (!png_surface)
    {
        return SDL_APP_FAILURE;
    }
    SDL_Texture* field_texture = SDL_CreateTextureFromSurface(renderer, png_surface);
    SDL_DestroySurface(png_surface);
    entities[0].addComponent(Texture{field_texture});
    entities[0].addComponent(Position{0, 0});
    entities[0].addComponent(Size{w, h});
    


    SDL_Surface* ball_surface = IMG_Load("../ball.png");
    if (!ball_surface)
    {
        return SDL_APP_FAILURE;
    }
    SDL_Texture* ball_texture = SDL_CreateTextureFromSurface(renderer, ball_surface);
    SDL_DestroySurface(ball_surface);
    entities[2].addComponent(Texture{ball_texture});
    entities[2].addComponent(Position{});
    entities[2].addComponent(Size{ 100, 100 });



    SDL_Surface* player_surface = IMG_Load("../playerRed.png");
    if (!player_surface)
    {
        return SDL_APP_FAILURE;
    }
    SDL_Texture* player_texture = SDL_CreateTextureFromSurface(renderer, player_surface);
    SDL_DestroySurface(player_surface);
    entities[1].addComponent(Texture{ player_texture });
    entities[1].addComponent(Position{ 0, 0 }); 
    entities[1].addComponent(Size{ 100, 100 });

    return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    if (event->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS;
    }
    if (event->type == SDL_EVENT_KEY_DOWN)
    {
        Position* pos = entities[1].getComponent<Position>();
        // TODO одновременное нажатие двух клавиш
        switch (event->key.key)
        {
        case SDLK_ESCAPE:
            return SDL_APP_SUCCESS;
        case SDLK_UP:
            pos->y_ = pos->y_ - 15;
            break;
        case SDLK_RIGHT:
            pos->x_ += 15;
            break;
        case SDLK_DOWN:
            pos->y_ += 15;
            break;
        case SDLK_LEFT:
            pos->x_ -= 15;
            break;
        }
    }
    return SDL_APP_CONTINUE;
}

// ___ working engine start. part 1
float vx = 5;
float vy = 5;

float t = 0.05;
// ___ working engine finish. part 1

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void* appstate)
{
    /* Draw the message */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // ___ DRAW SYSTEM start 
    for(auto& entity : entities)
    {
        auto tex = entity.getComponent<Texture>();
        auto pos = entity.getComponent<Position>();
        auto size = entity.getComponent<Size>();
        auto rect = SDL_FRect{ pos->x_, pos->y_, size->w_, size->h_};
        SDL_RenderTexture(renderer, tex->texture_, nullptr, &rect);
    }
    /// ___ DRAW SYSTEM finish

    // ___ working engine start.  part2
    int w = 0, h = 0;

    SDL_GetRenderOutputSize(renderer, &w, &h);
    
    float new_x;
    float new_y;
    
    // нужно подумать над скоростью, возможно надо засунуть в компоненты

    auto position_ball = entities[2].getComponent<Position>();
    auto size_ball = entities[2].getComponent<Size>();

    new_x = position_ball->x_ + vx * t;
    new_y = position_ball->y_ + vy * t;
      

    if ((new_x + size_ball->w_) > w || new_x < 0)
    {
        vx *= -1;
    }
    if ((new_y + size_ball->h_) > h || new_y < 0)
    {
        vy *= -1;
    }

    Position* pos_player = entities[1].getComponent<Position>();
    if ((std::abs(position_ball->x_ - pos_player->x_) < 20) && (std::abs(position_ball->y_ - pos_player->y_) < 20))
    {
        position_ball->x_ = new_x + 50;
        position_ball->y_ = new_y + 50;
        vy *= -1;
    
    }

    position_ball->x_ = new_x;
    position_ball->y_ = new_y;
    
    // ___ working engine finish. part 2

    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
