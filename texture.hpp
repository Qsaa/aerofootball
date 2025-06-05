#pragma once

#include <SDL3/SDL.h>
#include "component.hpp"

class Texture : public Component
{
public:
	Texture(SDL_Texture * texture);
	Texture(const Texture& texture);
	Texture(Texture&& texture) noexcept;
	~Texture();
public:
	SDL_Texture* texture_ = nullptr;
};