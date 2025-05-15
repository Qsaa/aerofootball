#include "ball.hpp"

#include <SDL3_image/SDL_image.h>



Ball::Ball (SDL_Renderer* renderer, SDL_Surface* ball_surface): x_(0), y_(0), d_(100), vx_(5), vy_(5)
{
    ball_texture = SDL_CreateTextureFromSurface(renderer, ball_surface);
}

Ball::~Ball()
{
    SDL_DestroyTexture(ball_texture);
}

float Ball::getX()
{
    return x_;
}
float Ball::getY()
{
    return y_;
}
float Ball::getD()
{
    return d_;
}


std::tuple<float, float> Ball::wanna_go_to(float t)
{
    return { vx_ * t + x_, vy_ * t + y_ };
}

void Ball::set_new_position(float x, float y)
{
    x_ = x;
    y_ = y;
}

void Ball::change_x_speed()
{
    vx_ *= -1;
}
void Ball::change_y_speed()
{
    vy_ *= -1;
}

void Ball::draw(SDL_Renderer* renderer)
{
    SDL_FRect rect = SDL_FRect{ getX(), getY(), getD(), getD() };
    SDL_RenderTexture(renderer, ball_texture, nullptr, &rect);

}
