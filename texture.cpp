#include "texture.hpp"

Texture::Texture(SDL_Texture* texture): texture_(texture) {}

Texture::Texture(const Texture& texture)
{
	texture_ = texture.texture_;
}

Texture::Texture(Texture&& texture) noexcept
{
	texture_ = texture.texture_;
	texture.texture_ = nullptr;
}
