
#include "component.hpp"

class Control : public Component
{
public:
	Control(unsigned int up, unsigned int right, unsigned int down, unsigned int left);
public:
	unsigned int up_;
	unsigned int right_;
	unsigned int down_;
	unsigned int left_;
};
