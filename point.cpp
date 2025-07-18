#include "point.hpp"

#include <cmath>

Point::Point(float x, float y): x_(x), y_(y) {}

Point::Point(const Point& point)
{
	x_ = point.x_;
	y_ = point.y_;
}

float Point::distance(const Point& point) const
{
	return std::sqrt((point.x_ - x_) * (point.x_ - x_) + (point.y_ - y_) * (point.y_ - y_));
}

Line::Line(Point a, Point b, float width) : a_(a), b_(b), width_(width){}

//Line::Line(Point c, float h, float w)
//{
//    a_.x_ = c.x_ + w / 2.0;
//    a_.y_ = c.y_ + w / 2.0;
//    b_.x_ = c.x_ + w / 2.0;
//    b_.y_ = c.y_ + h - w / 2.0; 
//    width_ = w;
//}

float Line::distance(const Point& point) const
{       
        //float lenghtLine = a_.distance(b_);
        //float distanceAtoP = point.distance(a_);
        //float distanceBtoP = point.distance(b_);


        //if (distanceAtoP > lenghtLine || distanceBtoP > lenghtLine)
        //{
        //    return (distanceAtoP > distanceBtoP) ? distanceAtoP : distanceBtoP;
        //}

        // Числитель: |(Bx - Ax)(Ay - Py) - (Ax - Px)(By - Ay)|
        float numerator = std::abs(
            (b_.y_ - a_.y_) * point.x_ - (b_.x_ - a_.x_) * point.y_ + b_.x_ * a_.y_ - b_.y_ * a_.x_
            //(b_.x_ - a_.x_) * (a_.y_ - point.y_) - (a_.x_ - point.x_) * (b_.y_ - a_.y_)
        );
        // Знаменатель: √((Bx - Ax)² + (By - Ay)²)
        float denominator = std::sqrt(
            (b_.x_ - a_.x_) * (b_.x_ - a_.x_) + (b_.y_ - a_.y_) * (b_.y_ - a_.y_)
        );

        //float dist = numerator / denominator;
        //float distMin = std::min(distanceAtoP, distanceBtoP);


        //return std::min(dist, distMin);
        return numerator / denominator;
}
