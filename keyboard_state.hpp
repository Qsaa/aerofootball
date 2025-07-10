#pragma once

#include <unordered_map>

struct KeyboardState
{
	std::unordered_map<unsigned int, bool> keys;
};
