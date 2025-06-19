#pragma once

#include "component.hpp"

class Collision : public Component
{
public:
	Collision(bool bounce = false);
public:
	bool bounce_ = false;
};