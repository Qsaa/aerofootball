#include "globals.hpp"
#include "components/control.hpp"
#include "components/velocity.hpp"

void control(Entities& entities)
{
    for (auto& entity : entities)
    {
        if (!entity.hasComponent<Control, Velocity>())
        {
            continue;
        }
        Control* control = entity.getComponent<Control>();

        float horizontal = 0;
        float vertical = 0;

        if (input.keys[control->up_])
        {
            vertical = -1;
        }
        else if (input.keys[control->down_])
        {
            vertical = 1;
        }

        if (input.keys[control->left_])
        {
            horizontal = -1;
        }
        else if (input.keys[control->right_])
        {
            horizontal = 1;
        }

        Velocity* vel = entity.getComponent<Velocity>();

        vel->vx_ = horizontal * 20;
        vel->vy_ = vertical * 20;
        if (std::abs(horizontal) + std::abs(vertical) > 1.98)
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
}