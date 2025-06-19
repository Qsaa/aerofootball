#include "point.hpp"

#include <cmath>

Point::Point(float x, float y): x_(x), y_(y) {}

Point::Point(const Point& point)
{
	x_ = point.x_;
	y_ = point.y_;
}

float Point::distance(const Point& point)
{
	return std::sqrt((point.x_ - x_) * (point.x_ - x_) + (point.y_ - y_) * (point.y_ - y_));
}
