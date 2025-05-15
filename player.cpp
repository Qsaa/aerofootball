#include "player.hpp"

#include <SDL3_image/SDL_image.h>

#include "globals.hpp"

extern SDL_Window* window;
extern SDL_Renderer* renderer;

Player::Player(SDL_Surface* player_surface) : x_(0), y_(0), d_(100), vx_(5), vy_(5)
{
    player_texture = SDL_CreateTextureFromSurface(renderer, player_surface);
}

Player::~Player()
{
    SDL_DestroyTexture(player_texture);
}

float Player::getX()
{
    return x_;
}
float Player::getY()
{
    return y_;
}
float Player::getD()
{
    return d_;
}


std::tuple<float, float> Player::wanna_go_to(float t)
{
    return { vx_ * t + x_, vy_ * t + y_ };
}

void Player::set_new_position(float x, float y)
{
    x_ = x;
    y_ = y;
}

void Player::change_x_speed()
{
    vx_ *= -1;
}
void Player::change_y_speed()
{
    vy_ *= -1;
}

void Player::draw()
{
    SDL_FRect rect = SDL_FRect{ getX(), getY(), getD(), getD() };
    SDL_RenderTexture(renderer, player_texture, nullptr, &rect);

}
