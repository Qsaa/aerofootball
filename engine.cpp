#include "globals.hpp"
#include "entity.hpp"
#include "components/velocity.hpp"
#include "components/position.hpp"
#include "components/size.hpp"
#include "components/collision.hpp"
#include "point.hpp"
#include "components/debug.hpp"

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

        // Collision to a border of the screen
        /*if ((new_x + size->w_) > w || new_x < 0)
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
        }*/

        for (auto& other : entities)
        {
            if (&entity == &other || !other.hasComponent<Collision, Position, Size>())
            {
                continue;
            }
            auto otherCollision = other.getComponent<Collision>();
            if (otherCollision->isRectangle_)
            {
                auto otherPos = other.getComponent<Position>();
                auto otherSize = other.getComponent<Size>();

                Line lineY{ Point{otherPos->x_ + otherSize->w_ / 2.0f, otherPos->y_},
                            Point{otherPos->x_ + otherSize->w_ / 2.0f, otherPos->y_ + otherSize->h_},
                            otherSize->w_ };
                Line lineX{ Point{otherPos->x_, otherPos->y_ + otherSize->h_ / 2.0f},
                            Point{otherPos->x_ + otherSize->w_, otherPos->y_ + otherSize->h_ / 2.0f},
                            otherSize->h_ };

                float distY = lineY.distance(Point{ new_x + size->w_ / 2.0f, new_y + size->w_ / 2.0f });
                float distX = lineX.distance(Point{ new_x + size->w_ / 2.0f, new_y + size->w_ / 2.0f });

                float deltaByY = distY - (lineY.width_ + size->w_) / 2.0;
                float deltaByX = distX - (lineX.width_ + size->w_) / 2.0;
                if (deltaByY < 0.0f && deltaByX < 0.0f)
                {
                    if (deltaByY < deltaByX)
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
                    else
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
                }
            }
            else
            {
                Point pointNew{ new_x + size->w_ / 2.0f, new_y + size->h_ / 2.0f };
                Point pointStatic = getCenter(other);
                float distance = pointNew.distance(pointStatic);
                auto other_size = other.getComponent<Size>();
                if (distance <= (size->w_ + other_size->w_) / 2.0)
                {
                    if (collision->bounce_)
                    {
                        // Есть две точки. Ищем угол к оси Х
                        float sinTetta = (pointStatic.y_ - pointNew.y_) / distance;
                        float cosTetta = (pointStatic.x_ - pointNew.x_) / distance;

                        // Преобразовываем систему координат
                        float velocity2vX = velocity->vx_ * cosTetta + velocity->vy_ * sinTetta;
                        float velosity2vY = -velocity->vx_ * sinTetta + velocity->vy_ * cosTetta;

                        //Отражение
                        float velocity2vX_reflect = -velocity2vX;
                        float velocity2vY_reflect = velosity2vY;

                        // Возвращаем систему координат // аккуратно 
                        sinTetta = -sinTetta;
                        cosTetta = cosTetta;

                        float vxNew = velocity2vX_reflect * cosTetta + velocity2vY_reflect * sinTetta;
                        float vyNew = -velocity2vX_reflect * sinTetta + velocity2vY_reflect * cosTetta;


                        velocity->vx_ = vxNew;
                        velocity->vy_ = vyNew;
                    }
                    new_x = pos->x_;
                    new_y = pos->y_;
                }
            }
        }

        pos->x_ = new_x;
        pos->y_ = new_y;
    }
}
