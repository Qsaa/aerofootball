#pragma once

#include <vector>
#include <typeinfo>
#include "component.hpp"

#include "texture.hpp"

class Entity
{
public:
	Entity() = default;
	void addComponent(const Component& component);
	
	// TODO  подумать над динамической типизации
	template<typename U>
	U* getComponent();
private:
	std::vector<Component*> components_;
};

template<typename U>
inline U* Entity::getComponent()
{
	for (const auto &c : components_)
	{
		if (auto comp = dynamic_cast<U*>(c))
		{
			return comp;
		}
	}
	return nullptr;
}
