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

#include "ball.hpp"
#include "player.hpp"
#include "globals.hpp"
#include "texture.hpp"
#include "position.hpp"

#include "entity.hpp"

static Ball* ball = nullptr;
static Player* player = nullptr; // TODO убрать отсюда

std::array<Entity, 2> entities;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    /* Create the window */
    if (!SDL_CreateWindowAndRenderer("Hello World", 800, 600, SDL_WINDOW_FULLSCREEN, &window, &renderer)) {
        SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

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
    


    SDL_Surface* ball_surface = IMG_Load("../ball.png");
    if (!ball_surface)
    {
        return SDL_APP_FAILURE;
    }
    ball = new Ball{ ball_surface };
    SDL_DestroySurface(ball_surface);



    SDL_Surface* player_surface = IMG_Load("../playerRed.png");
    if (!player_surface)
    {
        return SDL_APP_FAILURE;
    }
    SDL_Texture* player_texture = SDL_CreateTextureFromSurface(renderer, player_surface);
    SDL_DestroySurface(player_surface);
    entities[1].addComponent(Texture{ player_texture });
    entities[1].addComponent(Position{ 0, 0 });    

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

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void* appstate)
{
    const char* message = "Aerofootball";
    int w = 0, h = 0;
    float x, y;
    const float scale = 1.0f;

    /* Center the message and scale it up */
    SDL_GetRenderOutputSize(renderer, &w, &h);
    SDL_SetRenderScale(renderer, scale, scale);
    x = (w / scale) / 2;
    y = (h / scale) / 2;

    /* Draw the message */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // ___ DRAW SYSTEM start
    for(auto& entity : entities)
    {
        auto tex = entity.getComponent<Texture>();
        auto pos = entity.getComponent<Position>();
        auto rect = SDL_FRect{ pos->x_, pos->y_, static_cast<float>(w) , static_cast<float>(h) };
        SDL_RenderTexture(renderer, tex->texture_, nullptr, &rect);
    }
    /// ___ DRAW SYSTEM finish

    // ___ working engine start
    
    float new_x;
    float new_y;

    std::tie(new_x, new_y) = ball->wanna_go_to();
      

    if ((new_x + ball->getD()) > w || new_x < 0)
    {
        ball->change_x_speed();
    }
    if ((new_y + ball->getD()) > h || new_y < 0)
    {
        ball->change_y_speed();
    }

    Position* pos = entities[1].getComponent<Position>();
    if ((std::abs(ball->getX() - pos->x_) < 20) && (std::abs(ball->getY() - pos->y_) < 20))
    {
        ball->set_new_position(new_x + 50, new_y + 50);
        ball->change_y_speed();
    
    }

    ball->set_new_position(new_x, new_y);
    
    // ___ working engine finish
    
    ball->draw();

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
