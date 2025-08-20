#pragma once

#include "component.hpp"

enum class Team
{
    Blue = 0,
    Red = 1,
};

class Goal : public Component
{
public:
    Goal(Team team = Team::Blue);
public:
    Team team_ = Team::Blue;
};
