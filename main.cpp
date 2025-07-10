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
#include "components/velocity.hpp"
#include "components/collision.hpp"
#include "components/control.hpp"

// ==========
void draw(Entities&);
void engine(Entities&);
// =========


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
    entities[2].addComponent(Position{300, 300});
    entities[2].addComponent(Size{ 100, 100 });
    entities[2].addComponent(Velocity{ 5, 5 });
    entities[2].addComponent(Collision{true});


    SDL_Surface* playerRedSurface = IMG_Load("../playerRed.png");
    if (!playerRedSurface)
    {
        return SDL_APP_FAILURE;
    }
    SDL_Texture* playerRedTexture = SDL_CreateTextureFromSurface(renderer, playerRedSurface);
    SDL_DestroySurface(playerRedSurface);
    entities[1].addComponent(Texture{ playerRedTexture });
    entities[1].addComponent(Position{}); 
    entities[1].addComponent(Size{ 100, 100 });
    entities[1].addComponent(Velocity{ 0, 0 });
    entities[1].addComponent(Collision{});
    entities[1].addComponent(Control{});


    SDL_Surface* playerBlueSurface = IMG_Load("../playerBlue.png");
    if (!playerBlueSurface)
    {
        return SDL_APP_FAILURE;
    }
    SDL_Texture* playerBlueTexture = SDL_CreateTextureFromSurface(renderer, playerBlueSurface);
    SDL_DestroySurface(playerBlueSurface);
    entities[3].addComponent(Texture{ playerBlueTexture });
    entities[3].addComponent(Position{ 1000, 500 });
    entities[3].addComponent(Size{ 100, 100 });
    entities[3].addComponent(Velocity{ 0, 0 });
    entities[3].addComponent(Collision{});


    return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    if (event->type == SDL_EVENT_QUIT || (event->type == SDL_EVENT_KEY_DOWN && event->key.key == SDLK_ESCAPE))
    {
        return SDL_APP_SUCCESS;
    }

    for (auto& entity : entities)
    {
        if (!entity.hasComponent<Control, Velocity>())
        {
            continue;
        }
        Control* control = entity.getComponent<Control>();
        switch (event->key.key)
        {
            case SDLK_UP:
                if (event->type == SDL_EVENT_KEY_DOWN)
                {
                    control->vertical_ = -1;
                }
                else if (event->type == SDL_EVENT_KEY_UP)
                {
                    control->vertical_ = 0;
                }
                break;
            case SDLK_RIGHT:
                if (event->type == SDL_EVENT_KEY_DOWN)
                {
                    control->horizontal_ = 1;
                }
                else if (event->type == SDL_EVENT_KEY_UP)
                {
                    control->horizontal_ = 0;
                }
                break;
            case SDLK_DOWN:
                if (event->type == SDL_EVENT_KEY_DOWN)
                {
                    control->vertical_ = 1;
                }
                else if (event->type == SDL_EVENT_KEY_UP)
                {
                    control->vertical_ = 0;
                }
                break;
            case SDLK_LEFT:
                if (event->type == SDL_EVENT_KEY_DOWN)
                {
                    control->horizontal_ = -1;
                }
                else if (event->type == SDL_EVENT_KEY_UP)
                {
                    control->horizontal_ = 0;
                }
                break;
        }

        Velocity* vel = entity.getComponent<Velocity>();
        float hor = control->horizontal_;
        float ver = control->vertical_;

        vel->vx_ = control->horizontal_ * control->speed_;
        vel->vy_ = control->vertical_ * control->speed_;
        if (std::abs(hor) + std::abs(ver) > 1.98)
        {
            vel->vx_ *= sqrt(2.0) / 2.0;
            vel->vy_ *= sqrt(2.0) / 2.0;
        }

        // TODO: Саня, соберись! Подумай, как сделать красиво
        //float hor = control->horizontal_;
        //float ver = control->vertical_;
        //float v_control = std::sqrt(hor * hor + ver * ver);

        //float k = 1/v_control
        //if (k_velocity > 1.0)
        //{
        //    vel->vx_ = control->horizontal_ * control->speed_;// / k_velocity;
        //    vel->vy_ = control->vertical_ * control->speed_;// / k_velocity;
        //}
    }

    return SDL_APP_CONTINUE;
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void* appstate)
{
    /* Draw the message */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    draw(entities);
    engine(entities);
   
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
