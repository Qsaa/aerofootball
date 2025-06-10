#include "velocity.hpp"

Velocity::Velocity(float vx, float vy): vx_(vx), vy_(vy) {}

Velocity::Velocity(const Velocity& velocity) : vx_(velocity.vx_), vy_(velocity.vy_) {}

void Velocity::invert_vx()
{
	vx_ *= -1;
}

void Velocity::invert_vy()
{
	vy_ *= -1;
}