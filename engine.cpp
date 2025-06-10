#include "globals.hpp"
#include "entity.hpp"
#include "components/velocity.hpp"
#include "components/position.hpp"
#include "components/size.hpp"

void engine(Entities& entities)
{
    float t = 0.05;
    int w = 0, h = 0;

    SDL_GetRenderOutputSize(renderer, &w, &h);

    float new_x;
    float new_y;

    auto position_ball = entities[2].getComponent<Position>();
    auto size_ball = entities[2].getComponent<Size>();
    auto velocity = entities[2].getComponent<Velocity>();

    new_x = position_ball->x_ + velocity->vx_ * t;
    new_y = position_ball->y_ + velocity->vy_ * t;


    if ((new_x + size_ball->w_) > w || new_x < 0)
    {
        velocity->invert_vx();
    }
    if ((new_y + size_ball->h_) > h || new_y < 0)
    {
        velocity->invert_vy();
    }

    Position* pos_player = entities[1].getComponent<Position>();
    if ((std::abs(position_ball->x_ - pos_player->x_) < 20) && (std::abs(position_ball->y_ - pos_player->y_) < 20))
    {
        position_ball->x_ = new_x + 50;
        position_ball->y_ = new_y + 50;
        velocity->invert_vy();
    }

    position_ball->x_ = new_x;
    position_ball->y_ = new_y;

}