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
	
	template<typename U>
	U* getComponent();

	template<typename... Types >
	bool hasComponent();

private:
	bool hasComponent();

private:
	std::unordered_map<size_t, std::unique_ptr<Component>> components_;

};

template<typename U>
inline U* Entity::getComponent()
{
	size_t hash_code = typeid(U).hash_code();
	return components_.contains(hash_code) ? static_cast<U*>(components_.at(hash_code).get()) : nullptr;
}

inline bool Entity::hasComponent() {
	return true;
}

template<typename... Types>
inline bool Entity::hasComponent() {
	return ((components_.contains(typeid(Types).hash_code())   ) && ...);  // Fold expression
}

template<typename U>
void Entity::addComponent(U&& component)
{
	components_[typeid(component).hash_code()] = std::make_unique<U>(std::forward<U>(component));
}
