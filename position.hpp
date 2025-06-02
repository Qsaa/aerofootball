#pragma once

#include "component.hpp"

class Position : public Component
{
public:
	Position(float x = 0, float y = 0);
	Position(const Position& position);
public:
	float x_ = 0;
	float y_ = 0;
};