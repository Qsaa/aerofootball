#pragma once

class Point
{
public:
	Point(float x = 0, float y = 0);
	Point(const Point& point);

	float distance(const Point& point) const;
public:
	float x_;
	float y_;
};


class Line
{
public:
	Line(Point a, Point b, float width_);
	//Line(Point c, float h, float w);
	float distance(const Point& point) const;

public:
	Point a_;
	Point b_;
	float width_;
};