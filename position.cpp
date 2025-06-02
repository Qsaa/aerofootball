#include "position.hpp"

Position::Position(float x, float y): x_(0), y_(0) {}

Position::Position(const Position& position)
{
	x_ = position.x_;
	y_ = position.y_;
}
