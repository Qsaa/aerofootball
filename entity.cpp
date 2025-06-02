#include "entity.hpp"

void Entity::addComponent(const Component& component)
{
	components_.emplace_back(component);
}

