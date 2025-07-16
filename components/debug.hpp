#pragma once

#include <string>

#include "component.hpp"

class Debug : public Component
{
public:
	Debug(std::string name);
public:
	std::string name_;
};
