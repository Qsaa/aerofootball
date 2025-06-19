#include "globals.hpp"
#include "entity.hpp"
#include "components/velocity.hpp"
#include "components/position.hpp"
#include "components/size.hpp"
#include "components/collision.hpp"
#include "point.hpp"

Point getCenter(Entity& entity)
{
    if(!entity.hasComponent<Position, Size>())
    {
        return {-1, -1};
    }
    const auto& pos = entity.getComponent<Position>();
    const auto& size = entity.getComponent<Size>();

    return Point{pos->x_ + size->w_ / 2.0f, pos->y_ + size->h_ / 2.0f};
}


void engine(Entities& entities)
{
    float t = 0.05;
    int w = 0, h = 0;

    SDL_GetRenderOutputSize(renderer, &w, &h);
    
    for (auto& entity : entities)
    {
        if (!entity.hasComponent<Position, Size, Velocity, Collision>())
        {
            continue;
        }

        auto velocity = entity.getComponent<Velocity>();
        auto pos = entity.getComponent<Position>();
        auto size = entity.getComponent<Size>();
        auto collision = entity.getComponent<Collision>();
        
        float new_x = pos->x_ + velocity->vx_ * t;
        float new_y = pos->y_ + velocity->vy_ * t;

        if ((new_x + size->w_) > w || new_x < 0)
        {
            if (collision->bounce_)
            {
                velocity->invert_vx();
            }
            else
            {
                velocity->vx_ = 0;
            }
            new_x = pos->x_;
        }
        if ((new_y + size->h_) > h || new_y < 0)
        {
            if (collision->bounce_)
            {
                velocity->invert_vy();
            }
            else
            {
                velocity->vy_ = 0;
            }
            new_y = pos->y_;
        }

        for (auto& other : entities)
        {
            if (&entity == &other || !other.hasComponent<Collision, Position, Size>())
            {
                continue;
            }

            Point point {new_x + size->w_ / 2.0f, new_y + size->h_ / 2.0f};
            float distance = point.distance(getCenter(other));
            auto other_size = other.getComponent<Size>();
            if (distance <= (size->w_ + other_size->w_) / 2.0)
            {
                if (collision->bounce_)
                {
                    // TODO: запариться и сделать
                    velocity->invert_vy();
                    velocity->invert_vx();
                }
                else
                {
                    new_x = pos->x_;
                    new_y = pos->y_;
                }
            }
        }

        pos->x_ = new_x;
        pos->y_ = new_y;
    }
}
