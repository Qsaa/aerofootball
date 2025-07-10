
#include "component.hpp"

class Control : public Component
{
public:
	float horizontal_ = 0.0f;
	float vertical_ = 0.0f;
	float speed_ = 20.0f;
};
