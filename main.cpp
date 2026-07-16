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
#include <SDL3_ttf/SDL_ttf.h>

#include <cmath>
#include <iostream>
#include <utility>
#include <vector>
#include <array>
#include <format>

#include "entity.hpp"

#include "globals.hpp"

#include "components/texture.hpp"
#include "components/position.hpp"
#include "components/size.hpp"
#include "components/velocity.hpp"
#include "components/collider.hpp"
#include "components/control.hpp"
#include "components/debug.hpp"
#include "components/goal.hpp"
#include "components/ball.hpp"
#include "events/goal.hpp"


#include <set>

// ==========
void draw(Entities&);
void checkCollisions(Entities&);
void control(Entities&);
// =========

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    if(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
    {
        SDL_Log("Couldn't initialised SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    /* Create the window */
    if (!SDL_CreateWindowAndRenderer("Hello World", 800, 600, SDL_WINDOW_FULLSCREEN, &window, &renderer)) {
        SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!TTF_Init())
    {
        SDL_Log("Couldn't init TTF library: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    int w = 0, h = 0;
    SDL_GetRenderOutputSize(renderer, &w, &h);
    float h_float = h;
    float w_float = w;
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

    const int centerSize = 500;
    auto centerOfFieldSurface = IMG_Load("../center.png");
    if (!centerOfFieldSurface)
    {
        return SDL_APP_FAILURE;
    }
    SDL_Texture* centrOfFieldTexture = SDL_CreateTextureFromSurface(renderer, centerOfFieldSurface);
    SDL_DestroySurface(centerOfFieldSurface);
    entities[1].addComponent(Texture{centrOfFieldTexture});
    entities[1].addComponent(Position{w_float / 2.0f - centerSize / 2, h_float / 2.0f - centerSize / 2});
    entities[1].addComponent(Size{ centerSize, centerSize });


    SDL_Surface* ball_surface = IMG_Load("../ball.png");
    if (!ball_surface)
    {
        return SDL_APP_FAILURE;
    }
    SDL_Texture* ball_texture = SDL_CreateTextureFromSurface(renderer, ball_surface);
    SDL_DestroySurface(ball_surface);
    entities[2].addComponent(Texture{ball_texture});
    entities[2].addComponent(Position{w_float / 2.0f - 50.0f, h_float / 2.0f });
    entities[2].addComponent(Size{ 100, 100 });
    entities[2].addComponent(Velocity{0, 20 });
    entities[2].addComponent(Collider{true, false});
    entities[2].addComponent(Debug{ "BALL" });
    entities[2].addComponent(Ball{});


    SDL_Surface* playerRedSurface = IMG_Load("../playerRed.png");
    if (!playerRedSurface)
    {
        return SDL_APP_FAILURE;
    }
    SDL_Texture* playerRedTexture = SDL_CreateTextureFromSurface(renderer, playerRedSurface);
    SDL_DestroySurface(playerRedSurface);
    entities[3].addComponent(Texture{ playerRedTexture });
    entities[3].addComponent(Position{ w_float * 0.9f - 50, h_float / 2.0f - 50});
    entities[3].addComponent(Size{ 100, 100 });
    entities[3].addComponent(Velocity{ 0, 0 });
    entities[3].addComponent(Collider{});
    entities[3].addComponent(Debug{ "RED_PLAYER" });
    entities[3].addComponent(Control{SDLK_UP, SDLK_RIGHT, SDLK_DOWN, SDLK_LEFT});
    //entities[3].addComponent(Control{ 
    //    SDL_GAMEPAD_BUTTON_DPAD_UP, SDL_GAMEPAD_BUTTON_DPAD_RIGHT, SDL_GAMEPAD_BUTTON_DPAD_DOWN, SDL_GAMEPAD_BUTTON_DPAD_LEFT});


    SDL_Surface* playerBlueSurface = IMG_Load("../playerBlue.png");
    if (!playerBlueSurface)
    {
        return SDL_APP_FAILURE;
    }

    SDL_Texture* playerBlueTexture = SDL_CreateTextureFromSurface(renderer, playerBlueSurface);
    SDL_DestroySurface(playerBlueSurface);
    entities[4].addComponent(Texture{ playerBlueTexture });
    entities[4].addComponent(Position{ w_float * 0.1f - 50, h_float / 2.0f - 50 });
    entities[4].addComponent(Size{ 100, 100 });
    entities[4].addComponent(Velocity{ 0, 0 });
    entities[4].addComponent(Collider{});
    entities[4].addComponent(Control{SDLK_W, SDLK_D, SDLK_S, SDLK_A});

    SDL_Surface* wallSurface = IMG_Load("../wall.png");
    if (!wallSurface)
    {
        return SDL_APP_FAILURE;
    }
    SDL_Texture* wallTexture = SDL_CreateTextureFromSurface(renderer, wallSurface);
    // TODO: убрать костыли
    SDL_Texture* blueGoalTexture = SDL_CreateTextureFromSurface(renderer, wallSurface);
    SDL_Texture* redGoalTexture = SDL_CreateTextureFromSurface(renderer, wallSurface);
    SDL_DestroySurface(wallSurface);

    float k_size_goal = 1 / 2.2f;
    float k_size_wall = (1 - k_size_goal) / 2;
    
    entities[5].addComponent(Texture{ wallTexture });
    entities[5].addComponent(Position{0, 0});
    entities[5].addComponent(Size{w, 20 });
    entities[5].addComponent(Collider{ true, true });

    entities[6].addComponent(Texture{ wallTexture });
    entities[6].addComponent(Position{ w_float - 20 - 40, 0});
    entities[6].addComponent(Size{ 20.0f, h * k_size_wall});
    entities[6].addComponent(Collider{ true, true });

    SDL_SetTextureColorMod(redGoalTexture, 237, 28, 36);
    entities[7].addComponent(Texture{ redGoalTexture });
    entities[7].addComponent(Position{ w_float - 20, h_float * k_size_wall });
    entities[7].addComponent(Size{ 20.0f, h * k_size_goal });
    entities[7].addComponent(Collider{ true, true });
    entities[7].addComponent(Goal{Team::Red});

    entities[8].addComponent(Texture{ wallTexture });
    entities[8].addComponent(Position{ w_float - 20 - 40, h_float * (k_size_wall + k_size_goal)});
    entities[8].addComponent(Size{ 20.0f, h * k_size_wall });
    entities[8].addComponent(Collider{ true, true });

    entities[9].addComponent(Texture{ wallTexture });
    entities[9].addComponent(Position{0, h_float - 20});
    entities[9].addComponent(Size{w, 20 });
    entities[9].addComponent(Collider{true, true});

    entities[10].addComponent(Texture{ wallTexture });
    entities[10].addComponent(Position{40, 0 });
    entities[10].addComponent(Size{ 20.0f, h * k_size_wall});
    entities[10].addComponent(Collider{ true, true });

    SDL_SetTextureColorMod(blueGoalTexture, 63, 72, 204);
    entities[11].addComponent(Texture{ blueGoalTexture });
    entities[11].addComponent(Position{ 0, h_float * k_size_wall });
    entities[11].addComponent(Size{ 20.0f, h * k_size_goal});
    entities[11].addComponent(Collider{ true, true });
    entities[11].addComponent(Goal{Team::Blue});

    entities[12].addComponent(Texture{ wallTexture });
    entities[12].addComponent(Position{ 40, h_float * (k_size_goal + k_size_wall) });
    entities[12].addComponent(Size{ 20.0f, h * k_size_wall});
    entities[12].addComponent(Collider{ true, true });

    font = TTF_OpenFont("../uefa-nations-titling.ttf", 36.0f);
    if (!font)
    {
        return SDL_APP_FAILURE;
    }
    fontLarge = TTF_OpenFont("../uefa-nations-titling.ttf", 72.0f);
    if (!fontLarge)
    {
        return SDL_APP_FAILURE;
    }

    // Счёт синих
    std::string text = "Blue 0";
    SDL_Surface* blueTextSurface = TTF_RenderText_Solid(font, text.data(), text.length(), { 63, 72, 204 });
    SDL_Texture* blueTextTexture = SDL_CreateTextureFromSurface(renderer, blueTextSurface);
    SDL_DestroySurface(blueTextSurface);
    auto blueTextTextureProps = SDL_GetTextureProperties(blueTextTexture);
    entities[13].addComponent(Texture{blueTextTexture});
    entities[13].addComponent(Position{20, 20});
    entities[13].addComponent(Size
    {
        float(SDL_GetNumberProperty(blueTextTextureProps, SDL_PROP_TEXTURE_WIDTH_NUMBER, 0)),
        float(SDL_GetNumberProperty(blueTextTextureProps, SDL_PROP_TEXTURE_HEIGHT_NUMBER, 0))
    });

    // Счёт красных
    text = "Red 0";
    SDL_Surface* redTextSurface = TTF_RenderText_Solid(font, text.data(), text.length(), { 237, 28, 36 });
    SDL_Texture* redTextTexture = SDL_CreateTextureFromSurface(renderer, redTextSurface);
    SDL_DestroySurface(redTextSurface);
    auto redTextTextureProps = SDL_GetTextureProperties(redTextTexture);
    auto redTextWidth = float(SDL_GetNumberProperty(redTextTextureProps, SDL_PROP_TEXTURE_WIDTH_NUMBER, 0));
    entities[14].addComponent(Texture{redTextTexture});
    entities[14].addComponent(Position{w_float - 20 - redTextWidth, 20});
    entities[14].addComponent(Size
    {
        float(SDL_GetNumberProperty(redTextTextureProps, SDL_PROP_TEXTURE_WIDTH_NUMBER, 0)),
        float(SDL_GetNumberProperty(redTextTextureProps, SDL_PROP_TEXTURE_HEIGHT_NUMBER, 0))
    });

    text = "Game over";
    SDL_Surface* textSurface = TTF_RenderText_Solid(fontLarge, text.data(), text.length(), {0, 0, 0});
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_DestroySurface(textSurface);
    auto textTextureProps = SDL_GetTextureProperties(textTexture);
    float w_text = float(SDL_GetNumberProperty(textTextureProps, SDL_PROP_TEXTURE_WIDTH_NUMBER, 0));
    float h_text = float(SDL_GetNumberProperty(textTextureProps, SDL_PROP_TEXTURE_HEIGHT_NUMBER, 0));
    auto textWidth = float(SDL_GetNumberProperty(textTextureProps, SDL_PROP_TEXTURE_WIDTH_NUMBER, 0));
    // entities[15].addComponent(Texture{textTexture});
    entities[15].addComponent(Position{(w_float / 2.0f) - (w_text / 2.0f), (h_float / 2.0f) - (h_text / 2.0f),});
    entities[15].addComponent(Size{w_text, h_text});

    events.subscribe<GoalEvent>([textTexture, w_float, h_float](const GoalEvent& event)
    {
        auto velocity = entities[2].getComponent<Velocity>();
        velocity->vx_ = 0;
        velocity->vy_ = 20;

        auto position = entities[2].getComponent<Position>();
        position->x_ = w_float / 2.0f - 50.0f;
        position->y_ = h_float / 2.0f;

        switch (event.team)
        {
        case Team::Blue:
        {
            redScore++;

            // TODO: кошмар, переделать
            std::string str = std::format("Red {0}", redScore);
            SDL_Surface* surface = TTF_RenderText_Solid(font, str.data(), str.length(), { 237, 28, 36 });
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_DestroySurface(surface);
            auto props = SDL_GetTextureProperties(texture);
            entities[14].getComponent<Texture>()->texture_ = texture;
            entities[14].getComponent<Size>()->w_ = float(SDL_GetNumberProperty(props, SDL_PROP_TEXTURE_WIDTH_NUMBER, 0));
            
            if (redScore > 4)
            {
                entities[15].addComponent(Texture{textTexture});
                entities[2].delComponent<Velocity>();
            }
            break;
        }
        case Team::Red:
        {
            blueScore++;

            std::string str = std::format("Blue {0}", blueScore);
            SDL_Surface* surface = TTF_RenderText_Solid(font, str.data(), str.length(), { 63, 72, 204 });
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_DestroySurface(surface);
            auto props = SDL_GetTextureProperties(texture);
            entities[13].getComponent<Texture>()->texture_ = texture;
            entities[13].getComponent<Size>()->w_ = float(SDL_GetNumberProperty(props, SDL_PROP_TEXTURE_WIDTH_NUMBER, 0));

            if (blueScore > 4)
            {
                entities[15].addComponent(Texture{textTexture});
                entities[2].delComponent<Velocity>();
            }
            break;
        }
        default:
            break;
        }
    });

    return SDL_APP_CONTINUE;
}

SDL_JoystickID joystickFirst = NULL;
SDL_JoystickID joystickSecond = NULL;

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    if (event->type == SDL_EVENT_GAMEPAD_ADDED)
    {
        if (!joystickFirst)
        {
            joystickFirst = event->gdevice.which;
            SDL_OpenGamepad(joystickFirst);
        }
        else if (!joystickSecond)
        {
            joystickSecond = event->gdevice.which;
            SDL_OpenGamepad(joystickSecond);
        }
    }

    if (event->type == SDL_EVENT_QUIT || (event->type == SDL_EVENT_KEY_DOWN && event->key.key == SDLK_ESCAPE))
    {
        return SDL_APP_SUCCESS;
    }

    if (event->type == SDL_EVENT_KEY_DOWN)
    {
        input.keys[event->key.key] = true;
    }
    else if (event->type == SDL_EVENT_KEY_UP)
    {
        input.keys[event->key.key] = false;
    }
    
    else if (event->type == SDL_EVENT_GAMEPAD_BUTTON_DOWN)
    {
        input.keys[event->gbutton.button] = true;
    }
    else if (event->type == SDL_EVENT_GAMEPAD_BUTTON_UP)
    {
        input.keys[event->gbutton.button] = false;
    }
    else if (event->type == SDL_EVENT_GAMEPAD_AXIS_MOTION)
    {
        auto& axis = event->gaxis;
        if (axis.which == joystickFirst)
        {
            if (axis.axis == SDL_GAMEPAD_AXIS_LEFTX)
            {
                if (axis.value > -1000 && axis.value < 1000)
                {
                    input.keys[SDLK_A] = false;
                    input.keys[SDLK_D] = false;
                }
                else
                {
                    input.keys[SDLK_A] = axis.value < 10;
                    input.keys[SDLK_D] = axis.value > 10;
                }
            }
            if (axis.axis == SDL_GAMEPAD_AXIS_LEFTY)
            {
                if (axis.value > -1000 && axis.value < 1000)
                {
                    input.keys[SDLK_W] = false;
                    input.keys[SDLK_S] = false;
                }
                else
                {
                    input.keys[SDLK_W] = axis.value < 10;
                    input.keys[SDLK_S] = axis.value > 10;
                }
            }
        }
        else if (axis.which == joystickSecond)
        {
            if (axis.axis == SDL_GAMEPAD_AXIS_LEFTX)
            {
                if (axis.value > -1000 && axis.value < 1000)
                {
                    input.keys[SDLK_LEFT] = false;
                    input.keys[SDLK_RIGHT] = false;
                }
                else
                {
                    input.keys[SDLK_LEFT] = axis.value < 10;
                    input.keys[SDLK_RIGHT] = axis.value > 10;
                }
            }
            if (axis.axis == SDL_GAMEPAD_AXIS_LEFTY)
            {
                if (axis.value > -1000 && axis.value < 1000)
                {
                    input.keys[SDLK_UP] = false;
                    input.keys[SDLK_DOWN] = false;
                }
                else
                {
                    input.keys[SDLK_UP] = axis.value < 10;
                    input.keys[SDLK_DOWN] = axis.value > 10;
                }
            }
        }
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
    checkCollisions(entities);
    control(entities);

    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
