#pragma once

class Point
{
public:
	Point(float x = 0, float y = 0);
	Point(const Point& point);

	float distance(const Point& point);
public:
	float x_;
	float y_;
};