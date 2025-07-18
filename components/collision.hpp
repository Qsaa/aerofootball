#pragma once

#include "component.hpp"

class Collision : public Component
{
public:
	Collision(bool bounce = false, bool isRectangle = false);
public:
	bool bounce_ = false;
	bool isRectangle_ = false;
};
