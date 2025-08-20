#pragma once

#include "component.hpp"

class Collider : public Component
{
public:
	Collider(bool bounce = false, bool isRectangle = false);
public:
	bool bounce_ = false;
	bool isRectangle_ = false;
};
