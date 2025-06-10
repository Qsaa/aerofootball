#pragma once

#include <vector>
#include <unordered_map>
#include <typeinfo>
#include <memory>

#include "components/component.hpp"

class Entity
{
public:
	//Entity() = default;
	template<typename U>
	void addComponent(U&& component);
	
	// TODO  подумать над динамической типизации
	template<typename U>
	U* getComponent();
private:
	std::unordered_map<size_t, std::unique_ptr<Component>> components_;

};

template<typename U>
inline U* Entity::getComponent()
{
	Component* tmp = components_.at(typeid(U).hash_code()).get();
	return static_cast<U*>(components_.at(typeid(U).hash_code()).get());
	//auto it = components_.find((typeid(U).hash_code()));
	//return it != components_.end() ? static_cast<U*>(it->second.get()) : nullptr;
}

template<typename U>
void Entity::addComponent(U&& component)
{
	components_[typeid(component).hash_code()] = std::make_unique<U>(std::forward<U>(component));
}

