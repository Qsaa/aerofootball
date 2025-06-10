#pragma once

#include "component.hpp"

class Velocity : public Component
{
public:
	Velocity(float vx = 0, float vy = 0);
	Velocity(const Velocity& velocity);

	void invert_vx();
	void invert_vy();
public:
	float vx_;
	float vy_;
};