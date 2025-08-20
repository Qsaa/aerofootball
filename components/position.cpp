#include "position.hpp"

Position::Position(float x, float y): x_(x), y_(y) {}

Position::Position(const Position& position)
{
	x_ = position.x_;
	y_ = position.y_;
}
