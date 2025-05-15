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

#include "ball.hpp"
#include "player.hpp"


static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static SDL_Texture* field_texture = nullptr;
static Ball* ball = nullptr;
static Player* player = nullptr; // TODO убрать отсюда


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
    field_texture = SDL_CreateTextureFromSurface(renderer, png_surface);
    SDL_DestroySurface(png_surface);
    
    SDL_Surface* ball_surface = IMG_Load("../ball.png");
    if (!ball_surface)
    {
        return SDL_APP_FAILURE;
    }
    ball = new Ball{renderer, ball_surface};
    SDL_DestroySurface(ball_surface);

    SDL_Surface* player_surface = IMG_Load("../playerRed.png");
    if (!player_surface)
    {
        return SDL_APP_FAILURE;
    }
    player = new Player{ renderer, player_surface };
    SDL_DestroySurface(player_surface);
    
    

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
        // TODO одновременное нажатие двух клавиш
        switch (event->key.key)
        {
        case SDLK_ESCAPE:
            return SDL_APP_SUCCESS;
        case SDLK_UP:
            player->set_new_position(player->getX(), player->getY() - 15);
            break;
        case SDLK_RIGHT:
            player->set_new_position(player->getX() + 15, player->getY());
            break;
        case SDLK_DOWN:
            player->set_new_position(player->getX(), player->getY() + 15);
            break;
        case SDLK_LEFT:
            player->set_new_position(player->getX() - 15, player->getY());
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

    SDL_RenderTexture(renderer, field_texture, nullptr, nullptr);

    float new_x;
    float new_y;

    std::tie(new_x, new_y) = ball->wanna_go_to();

    // ___ working engine start

    if ((new_x + ball->getD()) > w || new_x < 0)
    {
        ball->change_x_speed();
    }
    if ((new_y + ball->getD()) > h || new_y < 0)
    {
        ball->change_y_speed();
    }

    if ((std::abs(ball->getX() - player->getX()) < 20) && (std::abs(ball->getY() - player->getY()) < 20))
    {
        ball->set_new_position(new_x + 50, new_y + 50);
        ball->change_y_speed();
    
    }

    ball->set_new_position(new_x, new_y);
    
    // ___ working engine finish
    
    ball->draw(renderer);
    player->draw(renderer);

    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    SDL_DestroyTexture(field_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
