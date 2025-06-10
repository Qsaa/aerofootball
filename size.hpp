#pragma once

#include "component.hpp"

class Size : public Component
{
public:
	Size(float w = 0, float h = 0);
	Size(int w = 0, int h = 0);
	Size(const Size& size);
public:
	float w_;
	float h_;
};