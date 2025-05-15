#pragma once
#include <tuple>

#include <SDL3/SDL.h>


// TODO
// SDL_Renderer* renderer - сделать глобальной переменной или занести в конструктор.

class Player
{
public:
    Player(SDL_Surface* player_surface);
    ~Player();


    float getX();
    float getY();
    float getD();

    std::tuple<float, float> wanna_go_to(float t = 0.2);
    void set_new_position(float x, float y);
    void change_x_speed();
    void change_y_speed();

    void draw();
private:
    float x_;
    float y_;
    float d_;

    float vx_;
    float vy_;

    SDL_Texture* player_texture;
};